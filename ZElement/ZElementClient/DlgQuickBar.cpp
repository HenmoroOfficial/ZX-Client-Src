// Filename	: DlgQuickBar.cpp
// Creator	: Tom Zhou
// Date		: October 11, 2005

#include "AFI.h"
#include "AUI\\AUIDef.h"
#include "AUI\\AUICheckBox.h"
#include "AUI\\AUIClockIcon.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIProgress.h"
#include "AUI\\AUILabel.h"
#include "aui\\AUISubDialog.h"
#include "DlgQuickBar.h"
#include "DlgSkill.h"
#include "DlgItemLock.h"
#include "DlgQShop.h"
#include "DlgWebShop.h"
#include "DlgDragDrop.h"
#include "globaldataman.h"
#include "LuaEvent.h"

#include "EC_GameUIMan.h"
#include "EC_UIManager.h"
#include "EC_Shortcut.h"
#include "EC_ShortcutSet.h"
#include "EC_Skill.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrEquip.h"
#include "EC_IvtrTalismanMain.h"
#include "EC_IvtrRune.h"
#include "EC_IvtrTaskItem.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_Global.h"
#include "EC_Configs.h"
#include "EC_PetCorral.h"
#include "EC_SendC2SCmds.h"
#include "EC_World.h"
#include "EC_ManPlayer.h"
#include "elementdataman.h"
#include "EC_HostInputFilter.h"
#include "EC_InputCtrl.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgQuickBar, CDlgBase)

AUI_ON_COMMAND("Btn_Combine",	OnCommand_combine)
AUI_ON_COMMAND("Btn_Out",		OnCommand_out)
AUI_ON_COMMAND("Btn_Up",		OnCommand_add)
AUI_ON_COMMAND("Btn_Down",		OnCommand_minus)
AUI_ON_COMMAND("switch",		OnCommand_switch)
AUI_ON_COMMAND("lock",			OnCommand_lock)
AUI_ON_COMMAND("Btn_QShop",		OnCommand_QShop)
AUI_ON_COMMAND("Btn_Talisman",	OnCommand_Talisman)
AUI_ON_COMMAND("Btn_Skill_Xp",  OnCommand_XpSkill)
AUI_ON_COMMAND("Btn_QuickBar_Setting",  OnCommand_QuickBarSetting)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)


AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgQuickBar, CDlgBase)

AUI_ON_EVENT("Item_*",		WM_LBUTTONDOWN,		OnEventLButtonDown_Item)

AUI_END_EVENT_MAP()

int CDlgQuickBar::m_nCurPanel = 1;
int CDlgQuickBar::m_nExtraPanels = 0;
int CDlgQuickBar::m_iTeleportNum = 0;
int CDlgQuickBar::m_iTeleportSkillId = 0;
CDlgQuickBar::CDlgQuickBar()
: m_pXpSkill(NULL), m_dwXpBtnLastUpdateTime(0)
{
}

CDlgQuickBar::~CDlgQuickBar()
{
	m_nExtraPanels = 0;
}

bool CDlgQuickBar::OnInitDialog()
{
	SetHintDelay(1000);
	return true;
}
void CDlgQuickBar::OnShowDialog()
{
	CECGameUIMan::SQuickBarLayOut* pLayOut = GetGameUIMan()->GetQuickBarLayout();
	if(!pLayOut)
		return;
}
void CDlgQuickBar::OnTick()
{
	if (0 == stricmp(GetName(), "Win_QuickBar8H_1"))
	{
		CECHostPlayer *pPlayer = GetHostPlayer();
		if( -1 != pPlayer->GetPetCombinedType() )
		{
			GetDlgItem("Btn_Combine")->Show(true);
			GetDlgItem("Btn_Out")->Show(false);
		}
		else
		{
			GetDlgItem("Btn_Combine")->Show(false);
			GetDlgItem("Btn_Out")->Show(true);
		}
		CDlgWebShopOrder* pDlgWebShopOrder = (CDlgWebShopOrder*)GetGameUIMan()->GetDialog("Win_Qshop_Web");
		if(NULL != GetDlgItem("Gfx_Qshop") && pDlgWebShopOrder)
			GetDlgItem("Gfx_Qshop")->Show(pDlgWebShopOrder->IsHasOrder());
		FlashXpSkillButton();
	}

}

void CDlgQuickBar::OnCommand_combine(const char * szCommand)
{
	GetHostPlayer()->CmdPetCombine1();
}

void CDlgQuickBar::OnCommand_out(const char * szCommand)
{
	GetHostPlayer()->CmdPetCall();
}
void CDlgQuickBar::OnCommand_QuickBarSetting(const char * szComand)
{
    bool bShow = m_pAUIManager->GetDialog("Win_QuickBar_Setting")->IsShow();
	m_pAUIManager->GetDialog("Win_QuickBar_Setting")->Show(!bShow);
}
void CDlgQuickBar::OnCommand_add(const char * szCommand)
{
	EC_VIDEO_SETTING setting = GetGame()->GetConfigs()->GetVideoSettings();

	if (0 == stricmp(GetName(), "Win_QuickBar10V_1"))
	{
		m_pAUIManager->GetDialog("Win_QuickBar10V_2")->Show(true);
		return;

	}
	else if (0 == stricmp(GetName(), "Win_QuickBar8H_1"))
	{
		PAUIDIALOG pShow1, pShow2, pShow3, pShow4;
		m_nExtraPanels = (m_nExtraPanels + 1) % 4;
		pShow1 = m_pAUIManager->GetDialog("Win_QuickBar8H_1");
		pShow2 = m_pAUIManager->GetDialog("Win_QuickBar8H_2");
		pShow3 = m_pAUIManager->GetDialog("Win_QuickBar8H_3");
		pShow4 = m_pAUIManager->GetDialog("Win_QuickBar8H_4");
		
		pShow2->Show(false);
		pShow3->Show(false);
		pShow4->Show(false);
		setting.nQuickbarNum = 1;
		
		if( m_nExtraPanels > 0 )
		{
			pShow2->Show(true);
			pShow2->SetCanMove(false);
			setting.nQuickbarNum = 2;
			
			if( m_nExtraPanels > 1 )
			{
				pShow3->Show(true);
				pShow3->SetCanMove(false);
				setting.nQuickbarNum = 3;

				if( m_nExtraPanels > 2 )
				{
					pShow4->Show(true);
					pShow4->SetCanMove(false);
					setting.nQuickbarNum = 4;
				}
			}
		}
	}
	else if (0 == stricmp(GetName(), "Win_QuickBar8H_2"))
	{
		PAUIDIALOG pShow2, pShow3, pShow4;
		if (3==m_nExtraPanels)
		{
			m_nExtraPanels = 1;
		}
		else
			m_nExtraPanels++;
		pShow2 = m_pAUIManager->GetDialog("Win_QuickBar8H_2");
		pShow3 = m_pAUIManager->GetDialog("Win_QuickBar8H_3");
		pShow4 = m_pAUIManager->GetDialog("Win_QuickBar8H_4");
		
		pShow3->Show(false);
		pShow4->Show(false);
		setting.nQuickbarNum = 2;
		
		
		if( m_nExtraPanels > 1 )
		{
			pShow3->Show(true);
			pShow3->SetCanMove(false);
			setting.nQuickbarNum = 3;
			
			if( m_nExtraPanels > 2 )
			{
				pShow4->Show(true);
				pShow4->SetCanMove(false);
				setting.nQuickbarNum = 4;
			}
		}
	}
	else if (0 == stricmp(GetName(), "Win_QuickBar8H_3"))
	{
		PAUIDIALOG pShow = m_pAUIManager->GetDialog("Win_QuickBar8H_4");
		pShow->Show(!pShow->IsShow());
		setting.nQuickbarNum = 3+pShow->IsShow();
	}
	
	GetGame()->GetConfigs()->SetVideoSettings(setting);
	GetGameUIMan()->CommonMessage("Win_BodyChangeSkill", "align_skill_bar", 0, 0);
}

void CDlgQuickBar::OnCommand_minus(const char * szCommand)
{
	EC_VIDEO_SETTING setting = GetGame()->GetConfigs()->GetVideoSettings();
	if (0 == stricmp(GetName(), "Win_QuickBar10V_1"))
	{
		m_pAUIManager->GetDialog("Win_QuickBar10V_2")->Show(false);
		return;

	}
	else if (0 == stricmp(GetName(), "Win_QuickBar8H_4"))
	{
		Show(false);
		setting.nQuickbarNum = 3;
	}
	else if (0 == stricmp(GetName(), "Win_QuickBar8H_3"))
	{
		m_pAUIManager->GetDialog("Win_QuickBar8H_3")->Show(false);
		m_pAUIManager->GetDialog("Win_QuickBar8H_4")->Show(false);
		setting.nQuickbarNum = 2;
		m_nExtraPanels = 1;
	}
	else
	{
		m_pAUIManager->GetDialog("Win_QuickBar8H_2")->Show(false);
		m_pAUIManager->GetDialog("Win_QuickBar8H_3")->Show(false);
		m_pAUIManager->GetDialog("Win_QuickBar8H_4")->Show(false);
		setting.nQuickbarNum = 1;
		m_nExtraPanels = 0;
	}
	
	GetGame()->GetConfigs()->SetVideoSettings(setting);
	GetGameUIMan()->CommonMessage("Win_BodyChangeSkill", "align_skill_bar", 0, 0);
}

void CDlgQuickBar::OnCommand_switch(const char * szCommand)
{
	char szName[80] = "";
	static int nNumBakupPanels = 3;
	
	m_nCurPanel++;
	m_nExtraPanels = 0;
	if( m_nCurPanel > nNumBakupPanels )
		m_nCurPanel = 1;
	m_pAUIManager->GetDialog("Win_QuickBar8H_2")->Show(false);
	m_pAUIManager->GetDialog("Win_QuickBar8H_3")->Show(false);
}

void CDlgQuickBar::OnCommand_lock(const char * szCommand)
{
	PAUICHECKBOX pCheck = (PAUICHECKBOX)GetDlgItem("Chk_Lock");
// 	if( pCheck )
// 		pCheck->Check(pCheck->IsChecked());
}

void CDlgQuickBar::OnCommand_QShop(const char * szCommand)
{
	CDlgQShop* pDlg;
	if (GetHostPlayer()->GetSrcServerID()==0)
	{
		pDlg = GetGameUIMan()->m_pDlgQShop;
		if (GetGameUIMan()->GetDialog("Win_QShopPop")->IsShow())
			pDlg = (CDlgQShop*)GetGameUIMan()->GetDialog("Win_QShopPop");
	}
	else
		pDlg = (CDlgQShop*)GetGameUIMan()->GetDialog("Win_QShopPop");

	if( pDlg )
	{
		if( pDlg->IsShow() )
			pDlg->OnCommand_CANCEL("");
		else
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
					c2s_SendCmdQueryMallSaleTimeItem();
			}
			else
				CDlgQShop::SetShopType(CDlgQShop::SHOP_TYPE_CROSSSERVER);
			pDlg->Show(true);
			LuaEvent::FirstOpenQshop();
		}
	}
}

void CDlgQuickBar::OnCommand_Talisman(const char * szCommand)
{
	EC_GAME_SETTING setting = GetGame()->GetConfigs()->GetGameSettings();
	setting.bShowTalismanDlg = !setting.bShowTalismanDlg;
	GetGame()->GetConfigs()->SetGameSettings(setting);
}

void CDlgQuickBar::OnCommand_XpSkill(const char * szCommand)
{
	AUIDialog* pDlg = GetGameUIMan()->GetDialog("Win_Skill_Xp");
	if (pDlg)
	{
		pDlg->Show(!pDlg->IsShow());
		CECConfigs* pConfig = g_pGame->GetConfigs();
		EC_LOCAL_SETTING ls = pConfig->GetLocalSettings();
		ls.bAutoShowSkillXp = pDlg->IsShow();
		pConfig->SetLocalSettings(ls);
		pConfig->SaveLocalSetting();
	}
}

void CDlgQuickBar::OnCommand_CANCEL(const char * szCommand)
{
	m_pAUIManager->RespawnMessage();
}

bool CDlgQuickBar::IsQuickbarLocked()
{
	if (0 == stricmp(GetName(), "Win_QuickBar10V_1") || 
		0 == stricmp(GetName(), "Win_QuickBar8H_1")  ||
		0 == stricmp(GetName(), "Win_QuickBar8H_2")  ||
		0 == stricmp(GetName(), "Win_QuickBar8H_3")  ||
		0 == stricmp(GetName(), "Win_QuickBar8H_4")  )
	{
		return ((PAUICHECKBOX)GetDlgItem("Chk_Lock"))->IsChecked();
	}
	else
	   return false;
}

int CDlgQuickBar::GetCurPanel()
{
	return m_nCurPanel;
}

void CDlgQuickBar::OnEventLButtonDown_Item(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	void *ptr;
	AString szType;
	pObj->ForceGetDataPtr(ptr,szType);
	if( !ptr ) return;

	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();

	GetGameUIMan()->m_ptLButtonDown.x = GET_X_LPARAM(lParam) - p->X;
	GetGameUIMan()->m_ptLButtonDown.y = GET_Y_LPARAM(lParam) - p->Y;
	GetGameUIMan()->InvokeDragDrop(this, pObj, GetGameUIMan()->m_ptLButtonDown);
}
static const char *szQuickBarPanel[] =
{
	"Win_QuickBar8H_1",
	"Win_QuickBar8H_2",
	"Win_QuickBar8H_3",
	"Win_QuickBar8H_4",

	"Win_QuickBar10V_1",
	"Win_QuickBar10V_2",

	"Win_QuickBar12C1",
	"Win_QuickBar12C2",
	"Win_QuickBar12C3",
	"Win_QuickBar12C4",

	"Win_QuickBar12C5",
	"Win_QuickBar12C6",
	"Win_QuickBar12C7",
	"Win_QuickBar12C8",
};
bool CDlgQuickBar::UpdateShortcuts()
{
	char szName[40];
	AString strFile;
	CECShortcut *pSC;
	PAUIIMAGEPICTURE pCell;
	int i, j;
	PAUIDIALOG pDlg;
	int nCurPanel = GetCurPanel();
	CECHostPlayer *pHost = GetHostPlayer();
	CECShortcutSet *a_pSCS[] =
	{
		pHost->GetShortcutSet2(0),
		pHost->GetShortcutSet1(0),
		pHost->GetShortcutSet1(1),
		pHost->GetShortcutSet2(1),

		pHost->GetShortcutSet3(0),
		pHost->GetShortcutSet3(1),

		pHost->GetShortcutSet3(2),
		pHost->GetShortcutSet3(2),
		pHost->GetShortcutSet3(2),
		pHost->GetShortcutSet3(2),
       
		pHost->GetShortcutSet3(3),
		pHost->GetShortcutSet3(3),
		pHost->GetShortcutSet3(3),
		pHost->GetShortcutSet3(3),


	};
	/*char *a_pszPanel[] =
	{
		"Win_QuickBar8H_1",
		"Win_QuickBar8H_2",
		"Win_QuickBar8H_3",
		"Win_QuickBar8H_4",

		"Win_QuickBar10V_1",
		"Win_QuickBar10V_2",

		"Win_QuickBar12C1",
		"Win_QuickBar12C2",
		"Win_QuickBar12C3",
		"Win_QuickBar12C4",

        "Win_QuickBar12C5",
		"Win_QuickBar12C6",
		"Win_QuickBar12C7",
		"Win_QuickBar12C8",
	};*/

	for( i = 0; i < sizeof(a_pSCS) / sizeof(CECShortcutSet *); i++ )
	{
		if( !a_pSCS[i] ) continue;

		pDlg = GetGameUIMan()->GetDialog(szQuickBarPanel[i]);
		if( !pDlg->IsShow() ) continue;

		if (i>0 && i<4)
		{
			pDlg->AlignTo(GetGameUIMan()->GetDialog(szQuickBarPanel[i-1]), AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
				AUIDIALOG_ALIGN_OUTSIDE,AUIDIALOG_ALIGN_TOP);
			if (i==1)
			{
				pDlg->SetPosEx( GetGameUIMan()->GetDialog(szQuickBarPanel[i-1])->GetDlgItem("Item_1")->GetPos().x, pDlg->GetPos().y);
			}
		}

		for( j = 0; j < 12; j++ )
		{
			sprintf(szName, "Item_%d", j + 1);
			pCell = (PAUIIMAGEPICTURE)pDlg->GetDlgItem(szName);

			if(pCell)
			{
			     pSC = a_pSCS[i]->GetShortcut(j);
			     UpdateShortcut(pSC, pCell);
			}
		}
	}

	pDlg = GetGameUIMan()->GetDialog("Win_QuickBar8H_1");
	PAUIPROGRESS pPrg = (PAUIPROGRESS)pDlg->GetDlgItem("Prgs_AP");
	PAUILABEL pLabel = (PAUILABEL)pDlg->GetDlgItem("Txt_AP");
	ROLEBASICPROP &rbp = GetHostPlayer()->GetBasicProps();
	double maxEXP = GetHostPlayer()->GetLevelUpExp(rbp.iLevel + 1, GetHostPlayer()->GetRebornCount());
	pPrg->SetProgress(int(AUIPROGRESS_MAX * (rbp.exp / maxEXP)));
	ACHAR szText[100];
	a_sprintf(szText, _AL("%.2f%%"), rbp.exp / maxEXP * 100.0);
	if( a_stricmp(szText, _AL("100.00%")) == 0 )
		a_strcpy(szText, _AL("99.99%"));

	pLabel->SetText(szText);
	pLabel->SetAlign(AUIFRAME_ALIGN_CENTER);


	{
		CECGameUIMan::SQuickBarLayOut* pLayOut = GetGameUIMan()->GetQuickBarLayout();
		bool bShowAll = true;
		if(pLayOut)
		{
			bShowAll = pLayOut->bShowAllGrid;

			PAUIDIALOG pDlgDragDrop = GetGameUIMan()->GetDialog("DragDrop");
			if(pDlgDragDrop->IsShow() )
			{  
				bShowAll = true;
			}
		}
		for (int i=0;i<sizeof(szQuickBarPanel)/sizeof(const char*);++i)
		{
			CDlgQuickBar* pDlgQuickBar = (CDlgQuickBar*)(GetGameUIMan()->GetDialog(szQuickBarPanel[i]));
			if(pDlgQuickBar && pDlgQuickBar->IsShow())
               pDlgQuickBar->UpdateGridState(bShowAll);
		}

		
	}

	return true;
}

void CDlgQuickBar::UpdateShortcut(CECShortcut *pSC, AUIImagePicture *pCell)
{
	CECSkill *pSkill;
	CECIvtrItem *pItem;
	CECInventory *pIvtr;
	CECSCItem *pSCItem;
	CECSCSkill *pSCSkill;
	AUIClockIcon *pClock;
	AString strFile;
	CECHostPlayer *pHost = g_pGame->GetGameRun()->GetHostPlayer();
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	int nMax, iIconFile;
	pClock = pCell->GetClockIcon();
	pClock->SetProgressRange(0, 1);
	pClock->SetProgressPos(1);
	pCell->SetText(_AL(""));
	pCell->SetCover(NULL, -1, 1);


	if( pSC && pSC->GetType() != CECShortcut::SCT_SKILLDUMB )
	{
		if( pSC->GetType() == CECShortcut::SCT_SKILL )
		{
			iIconFile = CECGameUIMan::ICONS_SKILL;
			pSCSkill = (CECSCSkill *)pSC;
			pSkill = pSCSkill->GetSkill();
			if (pSkill->IsCategory(GNET::CATEGORY_TRIGGERED) && pHost->GetSkill(pSkill->GetSkillID()))
			{
				pSkill = pHost->GetSkill(pSkill->GetSkillID());
			}
			if( pSkill->GetSkillLevel() <= 0 && !pSkill->IsCategory(GNET::CATEGORY_TRIGGERED) )
			{
				pCell->SetCover(NULL, -1);
				pCell->SetText(_AL(""));
				pCell->SetDataPtr(NULL);
				pCell->SetColor(A3DCOLORRGB(255, 255, 255));
				return;
			}

			if( pSkill && pSkill->ReadyToCast() && pHost->GetPrepSkill() != pSkill )
			{
				if( !pHost->CheckSkillCastCondition(pSkill) )
					pCell->SetColor(A3DCOLORRGB(255, 255, 255));
				else
					pCell->SetColor(A3DCOLORRGB(128, 128, 128));
			}
			else
				pClock->SetColor(A3DCOLORRGBA(0, 0, 0, 128));
			if( pSkill && (pSkill->GetCoolingTime() > 0 ||
				pHost->GetPrepSkill() == pSkill ))
			{
				pClock->SetProgressRange(0, pSkill->GetCoolingTime());
				if( pHost->GetPrepSkill() == pSkill )
					pClock->SetProgressPos(0);
				else
					pClock->SetProgressPos(pSkill->GetCoolingTime() - pSkill->GetCoolingCnt());
			}

			if(m_iTeleportSkillId == pSkill->GetSkillID() && m_iTeleportNum != 0)
			{
				ACHAR szText[40];
				a_sprintf(szText, _AL("%d"), m_iTeleportNum);
				pCell->SetText(szText);
			}
		}
		else if( pSC->GetType() == CECShortcut::SCT_ITEM )
		{
			iIconFile = CECGameUIMan::ICONS_INVENTORY;
			pCell->SetColor(A3DCOLORRGB(255, 255, 255));
			pSCItem = (CECSCItem *)pSC;
			pIvtr = pHost->GetPack(pSCItem->GetInventory());
			pItem = pIvtr->GetItem(pSCItem->GetIvtrSlot());
			if( pItem )
			{
				if( pItem->GetClassID() == CECIvtrItem::ICID_TASKDICE && 
					(((CECIvtrTaskDice*)pItem)->GetDBEssence()->type == 2 || ((CECIvtrTaskDice*)pItem)->GetDBEssence()->type == 3))
				{
					if(((CECIvtrTaskDice*)pItem)->IsUsing())
						pItem->Freeze(true);
				}
				pCell->SetColor(pGameUI->GetItemColor(pItem));
				if( pItem->GetCoolTime(&nMax) > 0 )
				{
					pClock->SetProgressRange(0, nMax);
					pClock->SetProgressPos(nMax - pItem->GetCoolTime());
					pClock->SetColor(A3DCOLORRGBA(0, 0, 0, 128));
				}
				ACHAR szText[100];
				if( pItem->GetPileLimit() > 1 && pIvtr->GetItemTotalNum(pItem->GetTemplateID()) > 1 )
				{
					a_sprintf(szText, _AL("%d"), pIvtr->GetItemTotalNum(pItem->GetTemplateID()));
					pCell->SetText(szText);
				}
				if (pItem->IsLocked())
				{
					pCell->SetCover(CDlgItemLock::m_pA2DSpriteLock, 0, 1);
				}
				
				
				if( ( pItem->GetClassID()==CECIvtrItem::ICID_EQUIP && ((CECIvtrEquip*)pItem)->GetEssence().num_estone>=9 ) ||
					( pItem->GetClassID()==CECIvtrItem::ICID_TALISMAN_MAINPART && ((CECIvtrTalismanMain*)pItem)->GetRebornCount()>0 ) ||
					( pItem->GetClassID()==CECIvtrItem::ICID_RUNE && ((CECIvtrRune*)pItem)->GetEssence().quality >= 3) )
				{
					if (pItem->IsLocked())
					{
						pCell->SetCover(CDlgSkill::m_pA2DSpriteSkillSpecEquipLock, (GetTickCount() / 200) % 6, 1);
					}
					else
					{
						pCell->SetCover(CDlgSkill::m_pA2DSpriteSkillSpecEquip, (GetTickCount() / 200) % 6, 1);
					}
				}
			}
			if( pSCItem->GetInventory() == IVTRTYPE_EQUIPPACK )
				pCell->SetColor(A3DCOLORRGBA(128, 128, 255, 128));
		}
		else
		{
			iIconFile = CECGameUIMan::ICONS_ACTION;
			pCell->SetColor(A3DCOLORRGB(255, 255, 255));
			if( pSC->GetCoolTime(&nMax) > 0 )
			{
				pClock->SetProgressRange(0, nMax);
				pClock->SetProgressPos(nMax - pSC->GetCoolTime());
				pClock->SetColor(A3DCOLORRGBA(0, 0, 0, 128));
			}
		}
		
		pCell->SetDataPtr(pSC,"ptr_CECShortcut");
		CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

		if( pSC->GetType() == CECShortcut::SCT_SKILLGRP )
		{
			EC_VIDEO_SETTING setting = g_pGame->GetConfigs()->GetVideoSettings();
			int nIcon = 0;
			if (((CECSCSkillGrp*)pSC)->GetGroupIndex() < EC_COMBOSKILL_NUM )
				nIcon = setting.comboSkill[((CECSCSkillGrp*)pSC)->GetGroupIndex()].nIcon;
			if( nIcon > 0 && nIcon <= EC_COMBOSKILL_ICONNUM )
			{
				pCell->SetCover(pGameUI->m_pA2DSpriteIcons[CECGameUIMan::ICONS_SKILLGRP], nIcon + 1);
			}
		}
		else if( pSC->GetType() == CECShortcut::SCT_TARGET )
		{
			CECSCTarget* pSCTarget = (CECSCTarget*)pSC;
			int idTarget = pSCTarget->GetTargetID();
			if(pSCTarget->GetTargetType() == CECSCTarget::TT_PLAYER)
			{
				AWString name = pSCTarget->GetTargetName();
				if(name.GetLength() == 0)
				{
					pSCTarget->BuildDesc();
				}

				// 获取player的职业
				int idProf = pSCTarget->GetTargetProfession();
				pCell->SetCover(pGameUI->m_pA2DSpriteProfs[idProf], 0);	
				pCell->SetHint(pSCTarget->GetDesc());
			}
			else if(pSCTarget->GetTargetType() == CECSCTarget::TT_MONSTER)
			{
				// 获取monster的类型
				elementdataman* pDB = g_pGame->GetElementDataMan();
				DATA_TYPE DataType;
				//	Get database data
				const MONSTER_ESSENCE* pMonEssence = (MONSTER_ESSENCE*)pDB->get_data_ptr(idTarget, ID_SPACE_ESSENCE, DataType);
				if(!pMonEssence)
					return;

				int iFraction = pMonEssence->monster_faction;
				int iF=0;
				while (iFraction>0)
				{
					if (iFraction&1)
					{
						if (iFraction>1)
						{
							iFraction = 15;
						}
						break;
					}
					iFraction>>=1;
					iF++;
				}

				pCell->SetCover(pGameUI->m_pA2DSpriteMonsterProfs[iF], 0);	
				pCell->SetHint(pSCTarget->GetDesc());
			}

			if(pSCTarget->CanExecute())
				pCell->SetColor(A3DCOLORRGB(255, 255, 255));
			else
				pCell->SetColor(A3DCOLORRGB(128, 128, 128));
		}
		else
		{
			af_GetFileTitle(pSC->GetIconFile(), strFile);
			strFile.MakeLower();
			pCell->SetCover(pGameUI->m_pA2DSpriteIcons[iIconFile], pGameUI->m_IconMap[iIconFile][strFile]);
		}
	}
	else
	{
		pCell->SetCover(NULL, -1);
		pCell->SetDataPtr(NULL);
		pCell->SetColor(A3DCOLORRGB(255, 255, 255));
	}
}

// xp技能可释放时，按钮闪烁
void CDlgQuickBar::FlashXpSkillButton()
{
	static const DWORD FLASH_INTERVER = 500;

	if (!m_pXpSkill)
	{
		if (0 == stricmp(GetName(), "Win_QuickBar8H_1"))
		{
			int idProf = GetHostPlayer()->GetProfession();
			elementdataman* pDataman = g_pGame->GetElementDataMan();
			DATA_TYPE dtType;
			
			// 获取职业模板
			int id = pDataman->get_first_data_id(ID_SPACE_CONFIG , dtType);
			while(id)
			{
				if(dtType == DT_CHARACTER_CLASS_CONFIG)
				{
					const CHARACTER_CLASS_CONFIG* pTempConfig = (const CHARACTER_CLASS_CONFIG*)pDataman->get_data_ptr(id , ID_SPACE_CONFIG , dtType);
					if((int)pTempConfig->character_class_id == idProf)
					{
						if (CECSkill* pSkill = GetHostPlayer()->GetSkill(pTempConfig->xp_skill))
							m_pXpSkill = pSkill;
						break;
					}
				}
				id = pDataman->get_next_data_id(ID_SPACE_CONFIG , dtType);
			}
		}
	}
	
	if (m_pXpSkill)
		GetDlgItem("Gfx_01")->Show(m_pXpSkill->GetCoolingCnt() == 0);
}

void CDlgQuickBar::SetTeleportInfo(int iTeleportSkillId, int iTeleportNum)
{
	m_iTeleportSkillId = iTeleportSkillId;
	m_iTeleportNum = iTeleportNum;
}


static const char* szBarSettingUI[] =
{
	"Win_QuickBar_SettingSubL01",
	"Win_QuickBar_SettingSubL02",
	"Win_QuickBar_SettingSubL03",
	"Win_QuickBar_SettingSubL04",
	"Win_QuickBar_SettingSubL05",
	"Win_QuickBar_SettingSubL06",
	"Win_QuickBar_SettingSubL07",
	"Win_QuickBar_SettingSubL08",
	"Win_QuickBar_SettingSubR",
};

static AString  GetBarSettingNameByQuickBarName(AString strQuickBar)
{
	int indexQuickBar = -1;
    int indexBarSetting = -1;
	AString strBarSetting ;
	for (int i =0; i<sizeof(szQuickBarPanel)/sizeof(const char*);++i)
	{
		if(stricmp(strQuickBar,szQuickBarPanel[i]) == 0)
		{
             indexQuickBar = i;
			 break;
		}
	}
	if(indexQuickBar != -1)
	{
	    if(indexQuickBar>=0 && indexQuickBar<=5)
		{
		  indexBarSetting = indexQuickBar;
		}
		else if(indexQuickBar>=6 && indexQuickBar<=9)
		{
		  indexBarSetting = 6;
		}
		else if(indexQuickBar>=10 && indexQuickBar<=13)
		{
		  indexBarSetting = 7;
		}
	}

	if(indexBarSetting>=0 && indexBarSetting<=7)
		strBarSetting = szBarSettingUI[indexBarSetting];

	return strBarSetting;
}
void CDlgQuickBar::UpdateGridState(bool bShowAll)
{
   if(!this->IsShow())
	   return;
   AString strItem;
   AString strLab;
   AString strShape;
   AUIObject* pItem = NULL;
   AUIObject* pLab  = NULL;
   AUIObject* pShape = GetDlgItem("Shape");

   AString strBarSetting =  GetBarSettingNameByQuickBarName(GetName());

   for (int i = 0; i<12;++i)
   {
       strItem.Format("%s%d","Item_",i+1);
	   strLab.Format("%s%d","Lab_",i+1);
	   pItem = GetDlgItem(strItem);
	   pLab = GetDlgItem(strLab);

	   if(pItem)
	   {
	       bool bShow = (bShowAll || (NULL != pItem->GetDataPtr("ptr_CECShortcut")));
		   if(pItem->IsShow() != bShow)
		   {
			   pItem->Show(bShow);
			   if(pLab)
		          pLab->Show(bShow);
			   if(pShape)
				  pShape->Show(bShow);
		   }
		   if(bShow)
		   {
			   pLab->SetText(CDlgQuickBarSettingDetail::GetLKNameByQuickBarItem(strBarSetting,i));
		   }
	   }

   }

}

AUI_BEGIN_COMMAND_MAP(CDlgQuickBarSetting, CDlgBase)
AUI_ON_COMMAND("Btn_Default",	OnCommand_Default)
AUI_ON_COMMAND("Rdo_Show*",	OnCommand_ShowQuickBarGrid)
AUI_ON_COMMAND("Chk_QuickBarShow*",	OnCommand_ShowAdditionalQuickBar)
AUI_ON_COMMAND("Rdo_Custom*",	OnCommand_ChangeCustomQuickBarType)
AUI_ON_COMMAND("IDCANCEL",	OnCommand_Close)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgQuickBarSetting, CDlgBase)
AUI_END_EVENT_MAP()

void CDlgQuickBarSetting::OnShowDialog()
{
	m_iCurrentPage = -1;
	CECGameUIMan::SQuickBarLayOut* pLayOut = GetGameUIMan()->GetQuickBarLayout();
	if(!pLayOut)
		return;
	CheckRadioButton(2,pLayOut->bShowAllGrid ? 1 : 2);
    CheckRadioButton(3,pLayOut->nCutomBar1_Type+1);
	CheckRadioButton(4,pLayOut->nCutomBar2_Type+1);
	((PAUICHECKBOX)(GetDlgItem("Chk_QuickBarShow01")))->Check(pLayOut->bShowRightBar);
	((PAUICHECKBOX)(GetDlgItem("Chk_QuickBarShow02")))->Check(pLayOut->bShowCustom_Bar1);
	((PAUICHECKBOX)(GetDlgItem("Chk_QuickBarShow03")))->Check(pLayOut->bShowCustom_Bar2);
}

void CDlgQuickBarSetting::OnCommand_Default(const char* szCommand)
{
	CECHostInputFilter* pFilter = (CECHostInputFilter*)GetGameRun()->GetInputCtrl()->GetKBFilter();
	if (!pFilter)
		return;
	pFilter->ResetLogicMap();
	ClearHotKey();
}
void CDlgQuickBarSetting::OnCommand_ShowQuickBarGrid(const char* szCommand)
{
    CECGameUIMan::SQuickBarLayOut* pLayOut = GetGameUIMan()->GetQuickBarLayout();
	if(!pLayOut)
		return;
	bool bShowAll = (GetCheckedRadioButton(2) == 1 ? true : false);
	if(bShowAll != pLayOut->bShowAllGrid )
	{
	    pLayOut->bShowAllGrid  = bShowAll;
	}

}
void CDlgQuickBarSetting::OnCommand_ShowAdditionalQuickBar(const char* szCommand)
{
	CECGameUIMan::SQuickBarLayOut* pLayOut = GetGameUIMan()->GetQuickBarLayout();
	if(!pLayOut)
		return;
	bool bShowRight= ((PAUICHECKBOX)(GetDlgItem("Chk_QuickBarShow01")))->IsChecked();
	bool bShowCustom1= ((PAUICHECKBOX)(GetDlgItem("Chk_QuickBarShow02")))->IsChecked();
	bool bShowCustom2= ((PAUICHECKBOX)(GetDlgItem("Chk_QuickBarShow03")))->IsChecked();
	if(pLayOut->bShowRightBar != bShowRight ||
	   pLayOut->bShowCustom_Bar1 != bShowCustom1 ||
	   pLayOut->bShowCustom_Bar2 != bShowCustom2 )
	{
	   pLayOut->bShowRightBar = bShowRight;
	   pLayOut->bShowCustom_Bar1 = bShowCustom1;
	   pLayOut->bShowCustom_Bar2 = bShowCustom2;
	   GetGameUIMan()->UpdateQuickBarState();

	}
    
}
void CDlgQuickBarSetting::OnCommand_ChangeCustomQuickBarType(const char* szCommand)
{
	CECGameUIMan::SQuickBarLayOut* pLayOut = GetGameUIMan()->GetQuickBarLayout();
	if(!pLayOut)
		return;
	int nCutomBar1_Type = GetCheckedRadioButton(3)-1;
	int nCutomBar2_Type = GetCheckedRadioButton(4)-1;
	if(pLayOut->nCutomBar1_Type != nCutomBar1_Type ||
	   pLayOut->nCutomBar2_Type != nCutomBar2_Type)
	{
	    pLayOut->nCutomBar1_Type = nCutomBar1_Type;
	    pLayOut->nCutomBar2_Type = nCutomBar2_Type;
	    GetGameUIMan()->UpdateQuickBarState();
	}
}
void CDlgQuickBarSetting::OnCommand_Close(const char * szCommand)
{
   this->Show(false);
}
void CDlgQuickBarSetting::ClearHotKey()
{
	for (int i =0;i<sizeof(szBarSettingUI)/sizeof(const char*);++i)
	{
		GetGameUIMan()->CommonMessage(szBarSettingUI[i],"unselect",0,0);
	}
	m_iCurrentPage = -1;
	GetGameUIMan()->SettingHotKey(false);
}
void CDlgQuickBarSetting::SetHotKey(int nHotKey, DWORD dwCtrl)
{
   if(m_iCurrentPage != -1)
   {
      GetGameUIMan()->CommonMessage(szBarSettingUI[m_iCurrentPage],"sethotkey",nHotKey,dwCtrl);
   }
}

int  CDlgQuickBarSetting::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
   if(strMsg == "setpage")
   {
      int page = (int)wParam;
	  m_iCurrentPage = page;
   }
   else if("clearhotkey")
   {
      ClearHotKey();
   }
   return 0;
}

AUI_BEGIN_COMMAND_MAP(CDlgQuickBarSettingDetailFrame, CDlgBase)
AUI_ON_COMMAND("Btn_Sub*",		OnCommand_TogglePage)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgQuickBarSettingDetailFrame, CDlgBase)
AUI_ON_EVENT("*",				WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT(NULL,				WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_END_EVENT_MAP()


bool CDlgQuickBarSettingDetailFrame::OnInitDialog()
{
	PAUIOBJECT pBtnSub =  GetDlgItem("Btn_SubL01");;
	POINT pos = pBtnSub->GetPos(true);
    m_srcPosY = pos.y;
	return true;
}
void CDlgQuickBarSettingDetailFrame::OnShowDialog()
{
   AString  strName;
   for (int i=1;i<=8;++i)
   {
	   strName.Format("%s%02d","Sub_",i);
	   PAUIOBJECT  pSubDlg = GetDlgItem(strName);
	   pSubDlg->Show(true);

	   if(pSubDlg && !pSubDlg->IsShow())
		   pSubDlg->Show(true);

   }


   PAUISUBDIALOG pSubLDlg = (PAUISUBDIALOG)(GetGameUIMan()->GetDialog("Win_QuickBar_Setting")->GetDlgItem("Sub_L"));
   SIZE size = pSubLDlg->GetSize();
   pSubLDlg->SetSubDialogOrgSize(size.cx,size.cy);


   PAUIOBJECT pBtnSub =  GetDlgItem("Btn_SubL01");
   POINT pos = pBtnSub->GetPos(true);
   pBtnSub->SetPos(pos.x,m_srcPosY);


   

   ReArrangedPages();
}
void CDlgQuickBarSettingDetailFrame::OnCommand_TogglePage(const char * szCommand)
{
	 AString  strName;
	 int index = atoi(szCommand + strlen("Btn_SubL"));
     strName.Format("%s%02d","Sub_",index);
	 PAUIOBJECT pSubDlg = GetDlgItem(strName);
	 bool bShow = pSubDlg->IsShow();
	 pSubDlg->Show(!bShow);
	 ReArrangedPages();

}
void CDlgQuickBarSettingDetailFrame::ReArrangedPages()
{
 
   AString    strBtn;
   AString    strSub;
   int        nSizeY  = 0;
   PAUIOBJECT pSubDlg = NULL;
   PAUIDIALOG  pDlg = NULL;
   PAUIOBJECT pBtnSub =  GetDlgItem("Btn_SubL01");;
   POINT pos = pBtnSub->GetPos(true);
   
   
   A3DRECT  rc  = pBtnSub->GetRect();
   int posY = pos.y + (rc.bottom - rc.top);
   nSizeY += (rc.bottom - rc.top);


   for (int i = 1;i<=8;++i)
   {
	   strSub.Format("%s%02d","Sub_",i);
	   pSubDlg = GetDlgItem(strSub);
	   strSub.Format("%s%02d","Win_QuickBar_SettingSubL",i);
	   pDlg = GetGameUIMan()->GetDialog(strSub);
	   if(pSubDlg && pDlg->IsShow())
	   {
		   pos = pSubDlg->GetPos(true);
		   rc  = pSubDlg->GetRect();
		   pSubDlg->SetPos(pos.x,posY);
		   posY += (rc.bottom - rc.top);
		   nSizeY += pDlg->GetSize().cy;
	   }

	   strBtn.Format("%s%02d","Btn_SubL",i+1);
	   pBtnSub = GetDlgItem(strBtn);
	   if(pBtnSub)
	   {
		   pos = pBtnSub->GetPos(true);
		   rc  = pBtnSub->GetRect();
		   pBtnSub->SetPos(pos.x,posY);
		   posY += (rc.bottom - rc.top);
		   nSizeY += (rc.bottom - rc.top);
	   }
   }

 
   PAUISUBDIALOG pSubLDlg = (PAUISUBDIALOG)(GetGameUIMan()->GetDialog("Win_QuickBar_Setting")->GetDlgItem("Sub_L"));
   SIZE size = pSubLDlg->GetSize();
   int vBarOldPos = pSubLDlg->GetVBarPos();
   pSubLDlg->SetSubDialogOrgSize(size.cx,nSizeY,false);
   int vBarNewPos = pSubLDlg->GetVBarPos();
   if(vBarOldPos > vBarNewPos)
   {
      this->MoveAllControls(0,vBarOldPos-vBarNewPos);
   }
   pSubLDlg->UpdateViewport();

}
void CDlgQuickBarSettingDetailFrame::OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	A3DRECT rcWindow = GetGameUIMan()->GetRect();
	int x = GET_X_LPARAM(lParam) - rcWindow.left;
	int y = GET_Y_LPARAM(lParam) - rcWindow.top;
	POINT ptClient = { 0, 0 };
	ClientToScreen(GetGameUIMan()->GetHWnd() ? GetGameUIMan()->GetHWnd() : g_pGame->GetA3DDevice()->GetDeviceWnd(), &ptClient);
	x -= ptClient.x;
	y -= ptClient.y;

	PAUIOBJECT pSubD = ((GetGameUIMan()->GetDialog("Win_QuickBar_Setting")->GetDlgItem("Sub_L")));
	A3DRECT rect = pSubD->GetRect();
	if(pSubD && rect.PtInRect(x,y))
		pSubD->OnDlgItemMessage(WM_MOUSEWHEEL, wParam, lParam);
}

void CDlgQuickBarSettingDetailFrame::OnTick()
{
	return;
	AString    strSub;
	int        nSizeY  = 0;
	PAUIOBJECT pSubDlg = NULL;
	PAUIDIALOG  pDlg = NULL;
	for (int i = 1;i<=8;++i)
	{
		strSub.Format("%s%02d","Sub_",i);
		pSubDlg = GetDlgItem(strSub);
		strSub.Format("%s%02d","Win_QuickBar_SettingSubL",i);
		pDlg = GetGameUIMan()->GetDialog(strSub);
		if(pSubDlg && pDlg)
		{
            POINT newPos = pSubDlg->GetPos();
			POINT oldPos = pDlg->GetPos();
			if(newPos.x != oldPos.x || newPos.y != oldPos.y)
				pDlg->SetPosEx(newPos.x,newPos.y);
		}

	}

}
#define MAX_CHK_COUNT 25
AUI_BEGIN_COMMAND_MAP(CDlgQuickBarSettingDetail, CDlgBase)
AUI_ON_COMMAND("Chk_*",		OnCommand_SelectQuickItem)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgQuickBarSettingDetail, CDlgBase)
AUI_ON_EVENT("*",				WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT(NULL,				WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_END_EVENT_MAP()
bool CDlgQuickBarSettingDetail::OnInitDialog()
{
   m_iCurrentKey = -1;
   if (0 == stricmp(GetName(), szBarSettingUI[0]) )
   {
        for (int i= LKEY_HP_SHORTCUT_1;i<=LKEY_HP_SHORTCUT_12;++i)
        {
			m_vecLK.push_back(i);
        }
   }
   else if(0 == stricmp(GetName(), szBarSettingUI[1]) )
   {
	   for (int i= LKEY_HP_SHORTCUT_13;i<=LKEY_HP_SHORTCUT_24;++i)
	   {
		   m_vecLK.push_back(i);
	   }
   }
   else if(0 == stricmp(GetName(), szBarSettingUI[2]) )
   {
	   for (int i= LKEY_HP_SHORTCUT_25;i<=LKEY_HP_SHORTCUT_36;++i)
	   {
		   m_vecLK.push_back(i);
	   }
   }
   else if(0 == stricmp(GetName(), szBarSettingUI[3]) )
   {
	   for (int i= LKEY_HP_SHORTCUT_37;i<=LKEY_HP_SHORTCUT_48;++i)
	   {
		   m_vecLK.push_back(i);
	   }
   }
   else if(0 == stricmp(GetName(), szBarSettingUI[4]) )
   {
	   for (int i= LKEY_HP_SHORTCUT_49;i<=LKEY_HP_SHORTCUT_58;++i)
	   {
		   m_vecLK.push_back(i);
	   }
   }
   else if(0 == stricmp(GetName(), szBarSettingUI[5]) )
   {
	   for (int i= LKEY_HP_SHORTCUT_59;i<=LKEY_HP_SHORTCUT_68;++i)
	   {
		   m_vecLK.push_back(i);
	   }
   }
   else if(0 == stricmp(GetName(), szBarSettingUI[6]) )
   {
	   for (int i= LKEY_HP_SHORTCUT_69;i<=LKEY_HP_SHORTCUT_80;++i)
	   {
		   m_vecLK.push_back(i);
	   }
   }
   else if(0 == stricmp(GetName(), szBarSettingUI[7]) )
   {
	   for (int i= LKEY_HP_SHORTCUT_81;i<=LKEY_HP_SHORTCUT_92;++i)
	   {
		   m_vecLK.push_back(i);
	   }

   }
   else if(0 == stricmp(GetName(),szBarSettingUI[8]))
   {
       m_vecLK.push_back(LKEY_UI_SHOWDLG_CHARACTER);
	   m_vecLK.push_back(LKEY_UI_SHOWDLG_INVENTORY);
	   m_vecLK.push_back(LKEY_UI_SHOWDLG_ACTION);
	   m_vecLK.push_back(LKEY_UI_SHOWDLG_FRIEND);
	   m_vecLK.push_back(LKEY_UI_SHOWDLG_FACTION);

	   m_vecLK.push_back(LKEY_UI_SHOWDLG_HELP);
	   m_vecLK.push_back(LKEY_UI_SHOWDLG_RANKLIST);
	   m_vecLK.push_back(LKEY_UI_SHOWDLG_CASHSHOP);
	   m_vecLK.push_back(LKEY_UI_SHOWDLG_DAILY);
	   m_vecLK.push_back(LKEY_UI_SHOWDLG_MAPMID);

	   m_vecLK.push_back(LKEY_UI_SHOWDLG_CIRCLE);
	   m_vecLK.push_back(LKEY_UI_SHOWDLG_PRODUCE);
	   m_vecLK.push_back(LKEY_UI_SHOWDLG_PET);
	   m_vecLK.push_back(LKEY_UI_SHOWDLG_TASK);
	   m_vecLK.push_back(LKEY_UI_SHOWDLG_SKILL);

	   m_vecLK.push_back(LKEY_UI_SHOWDLG_TEAM);
	   m_vecLK.push_back(LKEY_UI_SHOWDLG_BROADCAST);
	   m_vecLK.push_back(LKEY_UI_SHOWDLG_ZOARIUM);
	   m_vecLK.push_back(LKEY_UI_SHOWDLG_TEACHER);
   }

   return true;
}
void CDlgQuickBarSettingDetail::OnShowDialog()
{
	AString strName;
	PAUICHECKBOX  pCkeckBox = NULL;
	for (int i=0;i<MAX_CHK_COUNT;++i)
	{
	    CheckItem(i,false);
		SetItemString(i);
	}
}
void CDlgQuickBarSettingDetail::SetHotKey(int nHotKey, DWORD dwCtrl)
{
    if(m_iCurrentKey == -1)
	   return;
	if( GetVKName(nHotKey) == _AL(""))
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(17941), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	CECInputFilter* pFilter = GetGameRun()->GetInputCtrl()->GetKBFilter();
	if(!pFilter)
		return;
	int i, vk1, vk2;
	DWORD dwCtrl1, dwCtrl2;
	ACString strKey1, strKey2, strText;
	for(i = LKEY_GHOSTMODE; i < NUM_LKEY; i++)
	{
		if(i == m_vecLK[m_iCurrentKey])
			continue;

		pFilter->GetLogicKeyInfo(i, vk1, dwCtrl1, vk2, dwCtrl2);

		if( (vk1 == nHotKey && dwCtrl1 == dwCtrl) || (vk2 == nHotKey && dwCtrl2 == dwCtrl) )
		{
			GetGameUIMan()->MessageBox("", GetStringFromTable(17942), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			return;
		}
	}
	vk1 = nHotKey;
	dwCtrl1 = dwCtrl;
    pFilter->RemapLogicKey(m_vecLK[m_iCurrentKey], vk1, dwCtrl1, vk2, dwCtrl2);
	GetGameUIMan()->CommonMessage("Win_QuickBar_Setting","clearhotkey",0,0);

}
void CDlgQuickBarSettingDetail::OnCommand_SelectQuickItem(const char * szCommand)
{
	for (int i =0;i<sizeof(szBarSettingUI)/sizeof(const char*);++i)
	{
        GetGameUIMan()->CommonMessage(szBarSettingUI[i],"unselect",0,0);
	}
	AString strName;
    int index = atoi(szCommand + strlen("Chk_"))-1;
	CheckItem(index,true);
	PAUICHECKBOX  pCkeckBox = NULL;
	strName.Format("%s%02d","Chk_",index+1);
	pCkeckBox = (PAUICHECKBOX)(GetDlgItem(strName));
	assert(NULL != pCkeckBox);
    pCkeckBox->SetText(GetGameUIMan()->GetStringFromTable(17940));
	m_iCurrentKey = index;

	for (int i =0;i<sizeof(szBarSettingUI)/sizeof(const char*);++i)
	{
		 if(0 == stricmp(GetName(), szBarSettingUI[i]))
		 {
			 GetGameUIMan()->CommonMessage("Win_QuickBar_Setting","setpage",i,0);
			 GetGameUIMan()->SettingHotKey(true);
		     break;
		 }
	}

	
}
void CDlgQuickBarSettingDetail::OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	A3DRECT rcWindow = GetGameUIMan()->GetRect();
	int x = GET_X_LPARAM(lParam) - rcWindow.left;
	int y = GET_Y_LPARAM(lParam) - rcWindow.top;
	POINT ptClient = { 0, 0 };
	ClientToScreen(GetGameUIMan()->GetHWnd() ? GetGameUIMan()->GetHWnd() : g_pGame->GetA3DDevice()->GetDeviceWnd(), &ptClient);
	x -= ptClient.x;
	y -= ptClient.y;

	PAUIOBJECT pSubD = NULL;

	pSubD = GetGameUIMan()->GetDialog("Win_QuickBar_Setting")->GetDlgItem("Sub_R");

	A3DRECT rect = pSubD->GetRect();
	if(pSubD && rect.PtInRect(x,y))
		pSubD->OnDlgItemMessage(WM_MOUSEWHEEL, wParam, lParam);

	pSubD = ((GetGameUIMan()->GetDialog("Win_QuickBar_Setting")->GetDlgItem("Sub_L")));
	rect = pSubD->GetRect();
	if(pSubD && rect.PtInRect(x,y))
		pSubD->OnDlgItemMessage(WM_MOUSEWHEEL, wParam, lParam);
}
int  CDlgQuickBarSettingDetail::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if(strMsg == "unselect")
	{
		for (int i=0;i<MAX_CHK_COUNT;++i)
		{
			CheckItem(i,false);
			SetItemString(i);
		}
		m_iCurrentKey = -1;
	}
	else if(strMsg == "sethotkey")
	{
		int nHotkey = (int)wParam;
		DWORD dwCtrl = (DWORD)lParam;
	    SetHotKey(nHotkey, dwCtrl);
	}
	return 0;
}
void CDlgQuickBarSettingDetail::CheckItem(int index,bool bCheck)
{
	AString strName;
	PAUICHECKBOX  pCkeckBox = NULL;
	strName.Format("%s%02d","Chk_",index+1);
	pCkeckBox = (PAUICHECKBOX)(GetDlgItem(strName));
	if(NULL == pCkeckBox)
		return;
	 pCkeckBox->Check(bCheck);
}
void CDlgQuickBarSettingDetail::SetItemString(int index)
{
	if(index<0 || index >= (int)m_vecLK.size())
		return;
	AString strName;
	strName.Format("%s%02d","Chk_",index+1);
	PAUIOBJECT  pObject = GetDlgItem(strName);
	if(NULL == pObject)
		return;
	int iLogicKey = m_vecLK[index];
	CECInputFilter* pFilter = GetGameRun()->GetInputCtrl()->GetKBFilter();
	if(!pFilter)
		return;
	int vk1, vk2;
	DWORD dwCtrl1, dwCtrl2;
	pFilter->GetLogicKeyInfo(iLogicKey, vk1, dwCtrl1, vk2, dwCtrl2);
	ACString strKey = _AL("");
	if(vk1 > 0)
	{
		strKey = GetVKName(vk1);
		if( dwCtrl1 & EC_KSF_SHIFT )
			strKey = _AL("Shift+") + strKey;
		if( dwCtrl1 & EC_KSF_ALT )
			strKey = _AL("Alt+") + strKey;
		if( dwCtrl1 & EC_KSF_CONTROL )
			strKey = _AL("Ctrl+") + strKey;
	}
	pObject->SetText(strKey);

	 
}
ACString CDlgQuickBarSettingDetail::GetLKNameByQuickBarItem(AString strDlgName,int index)
{
	ACString strLKName;
    CDlgQuickBarSettingDetail* pBarSettingDetail = (CDlgQuickBarSettingDetail*)(g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog(strDlgName));
	if(pBarSettingDetail)
	{
	   strLKName = pBarSettingDetail->GetLKNameByIndex(index);
	}
	return strLKName;
}
ACString CDlgQuickBarSettingDetail::GetLKNameByIndex(int index)
{
	ACString strLKName;
    if(index>=0 && index<m_vecLK.size())
	{
	   strLKName = GetLKName(m_vecLK[index]);
	}
	return strLKName;
}
ACString CDlgQuickBarSettingDetail::GetLKName(int iLogicKey)
{
	ACString strKey = _AL("");
	CECInputFilter* pFilter = g_pGame->GetGameRun()->GetInputCtrl()->GetKBFilter();
	if(pFilter)
	{
		int vk1, vk2;
		DWORD dwCtrl1, dwCtrl2;
		pFilter->GetLogicKeyInfo(iLogicKey, vk1, dwCtrl1, vk2, dwCtrl2);
		
		if(vk1 > 0)
		{
			strKey = GetVKName(vk1);
			if( dwCtrl1 & EC_KSF_SHIFT )
				strKey = _AL("Shift+") + strKey;
			if( dwCtrl1 & EC_KSF_ALT )
				strKey = _AL("Alt+") + strKey;
			if( dwCtrl1 & EC_KSF_CONTROL )
				strKey = _AL("Ctrl+") + strKey;
		}

	}

	return strKey;
}
ACString CDlgQuickBarSettingDetail::GetVKName(int vk)
{
	ACString strText = _AL("");
	if( vk >= 'A' && vk <= 'Z' || vk >= '0' && vk <= '9')
		strText.Format(_AL("%c"), vk);
	else if( vk >= VK_F1 && vk <= VK_F24 )
		strText.Format(_AL("F%d"), vk - VK_F1 + 1);
	else if( vk >= VK_NUMPAD0 && vk <= VK_NUMPAD9 )
		strText.Format(_AL("NumPad%d"), vk - VK_NUMPAD0);
	{
		switch(vk)
		{
		case VK_BACK:
			strText = _AL("Backspace");
			break;

		case VK_TAB:
			strText = _AL("Tab");
			break;

		case VK_CAPITAL:
			strText = _AL("CapsLock");
			break;

		case VK_RETURN:
			strText = _AL("Enter");
			break;

		case VK_PAUSE:
			strText = _AL("Pause");
			break;

		case VK_SPACE:
			strText = _AL("Space");
			break;

		case VK_PRIOR:
			strText = _AL("PageUp");
			break;

		case VK_NEXT:
			strText = _AL("PageDown");
			break;

		case VK_END:
			strText = _AL("End");
			break;

		case VK_HOME:
			strText = _AL("Home");
			break;

		case VK_LEFT:
			strText = _AL("←");
			break;

		case VK_UP:
			strText = _AL("↑");
			break;

		case VK_RIGHT:
			strText = _AL("→");
			break;

		case VK_DOWN:
			strText = _AL("↓");
			break;

		case VK_SCROLL:
			strText = _AL("Scroll Lock");
			break;

		case VK_SNAPSHOT:
			strText = _AL("PrintScr");
			break;

		case VK_INSERT:
			strText = _AL("Insert");
			break;

		case VK_DELETE:
			strText = _AL("Delete");
			break;

		case VK_NUMLOCK:
			strText = _AL("Num Lock");
			break;

		case VK_MULTIPLY:
			strText = _AL("NumPad *");
			break;

		case VK_ADD:
			strText = _AL("NumPad +");
			break;

		case VK_SEPARATOR:
			strText = _AL("NumPad Enter");
			break;

		case VK_SUBTRACT:
			strText = _AL("NumPad -");
			break;

		case VK_DECIMAL:
			strText = _AL("NumPad .");
			break;

		case VK_DIVIDE:
			strText = _AL("NumPad /");
			break;

		case VK_OEM_1:
			strText = _AL(";");
			break;

		case VK_OEM_PLUS:
			strText = _AL("=");
			break;

		case VK_OEM_COMMA:
			strText = _AL(",");
			break;

		case VK_OEM_MINUS:
			strText = _AL("-");
			break;

		case VK_OEM_PERIOD:
			strText = _AL(".");
			break;

		case VK_OEM_2:
			strText = _AL("/");
			break;

		case VK_OEM_3:
			strText = _AL("`");
			break;

		case VK_OEM_4:
			strText = _AL("[");
			break;

		case VK_OEM_5:
			strText = _AL("\\");
			break;

		case VK_OEM_6:
			strText = _AL("]");
			break;

		case VK_OEM_7:
			strText = _AL("\"");
			break;

		}
	}
	return strText;
}

