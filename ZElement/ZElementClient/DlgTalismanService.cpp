// File		: DlgTalismanService.cpp
// Creator	: Xiao Zhou
// Date		: 2007/4/19

#include "AFI.h"
#include "DlgTalismanService.h"
#include "DlgInventory.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrTalismanMain.h"
#include "EC_IvtrConsume.h"
#include "AUI\\AUICTranslate.h"
#include "AUI\\AUIListBox.h"
#include "elementdataman.h"
#include "DlgItemLock.h"
#include "EC_Skill.h"
#include "LuaEvent.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgTalismanService, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",		OnCommandCANCEL)
AUI_ON_COMMAND("start",			OnCommandStart)
AUI_ON_COMMAND("Btn_Medium",	OnCommandShopExchange)
AUI_ON_COMMAND("help",			OnCommandHelp)
AUI_ON_COMMAND("Btn_Start", OnCommandEnchantStart)
AUI_ON_COMMAND("Btn_Stop", OnCommandEnchantStop)
AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgTalismanService, CDlgBase)

AUI_ON_EVENT("Item_a", WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("Item_a", WM_LBUTTONDBLCLK,OnEventLButtonDown)
AUI_ON_EVENT("Item_a", WM_RBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("Item_b", WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("Item_b", WM_LBUTTONDBLCLK,OnEventLButtonDown)
AUI_ON_EVENT("Item_b", WM_RBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("Item_c", WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("Item_c", WM_LBUTTONDBLCLK,OnEventLButtonDown)
AUI_ON_EVENT("Item_c", WM_RBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("Item_d", WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("Item_d", WM_LBUTTONDBLCLK,OnEventLButtonDown)
AUI_ON_EVENT("Item_d", WM_RBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("Item_e", WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("Item_e", WM_LBUTTONDBLCLK,OnEventLButtonDown)
AUI_ON_EVENT("Item_e", WM_RBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("Item_f", WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("Item_f", WM_LBUTTONDBLCLK,OnEventLButtonDown)
AUI_ON_EVENT("Item_f", WM_RBUTTONDOWN,	OnEventLButtonDown)

AUI_END_EVENT_MAP()


//聚气等级限制公式
static int GetMaxLevelForRefine()
{
	int maxlevel = 0;
    maxlevel = min(g_pGame->GetGameRun()->GetHostPlayer()->GetBasicProps().iLevel, 140) / 5 + 2 + 5 * g_pGame->GetGameRun()->GetHostPlayer()->GetRebornCount();
    if(g_pGame->GetGameRun()->GetHostPlayer()->GetRebornCount() > 0)
	{
		if(g_pGame->GetGameRun()->GetHostPlayer()->GetBasicProps().iLevel >= 160) 
			maxlevel = maxlevel + 2;
		else if(g_pGame->GetGameRun()->GetHostPlayer()->GetBasicProps().iLevel >= 155)
			maxlevel = maxlevel + 1;
	}

	return maxlevel;
}

CDlgTalismanService::CDlgTalismanService()
{
}

CDlgTalismanService::~CDlgTalismanService()
{
	m_ItemID.clear();
}

bool CDlgTalismanService::OnInitDialog()
{
	m_pItema = (PAUIIMAGEPICTURE)GetDlgItem("Item_a");
	m_pItemb = (PAUIIMAGEPICTURE)GetDlgItem("Item_b");
	m_pItemc = (PAUIIMAGEPICTURE)GetDlgItem("Item_c");
	m_pItemd = (PAUIIMAGEPICTURE)GetDlgItem("Item_d");
	m_pIteme = (PAUIIMAGEPICTURE)GetDlgItem("Item_e");
	m_pItemf = (PAUIIMAGEPICTURE)GetDlgItem("Item_f");
	m_pBtn_Enchase = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Enchase");
	m_pBtn_Exchange = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Medium");
	m_pTxt_Money1 = (PAUILABEL)GetDlgItem("Txt_Money1");
	m_pTxt_Money2 = (PAUILABEL)GetDlgItem("Txt_Money2");
	m_pTxt_Money3 = (PAUILABEL)GetDlgItem("Txt_Money3");
	if(m_pTxt_Money1)	m_pTxt_Money1->SetAlign(AUIFRAME_ALIGN_RIGHT);
	if(m_pTxt_Money2)	m_pTxt_Money2->SetAlign(AUIFRAME_ALIGN_RIGHT);
	if(m_pTxt_Money3)	m_pTxt_Money3->SetAlign(AUIFRAME_ALIGN_RIGHT);


	m_pBtn_EnchantStart = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Start");
	m_pBtn_EnchantStop  = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Stop");
	m_bEnchangting      = false;
	m_bEnchangtStart    = false;
	m_ItemID.clear();

	m_bLock = false;
	return true;
}

void CDlgTalismanService::CheckItem(PAUIOBJECT pObj)
{
	if( pObj && pObj->GetDataPtr("ptr_CECIvtrItem") )
	{
		CECInventory *pPack  = GetHostPlayer()->GetPack();
		CECIvtrItem *pItem   = pPack->GetItem(pObj->GetData());
		AString      strName = pObj->GetName(); 
	    AUICTranslate trans;

        //此处应验证当前信息的一致性，判断当前包裹中指定位置的物品是否有效
		if(pItem && pItem->GetTemplateID() == m_ItemID[strName])
		{
			const ACHAR * szDesc = pItem->GetDesc();
			if( szDesc )
				pObj->SetHint(trans.Translate(szDesc));
			else
				pObj->SetHint(_AL(""));
            
			//重置
			{
               PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)pObj;
			   pImage->SetDataPtr(pItem, "ptr_CECIvtrItem");
			   pImage->SetCover(NULL,0,1);
			   if (pItem->IsLocked())
		       pImage->SetCover(CDlgItemLock::m_pA2DSpriteLock, 0, 1);
			   pItem->Freeze(true);
			   
			}
			return;
		}

		ClearImagePicControl(pObj);

	}
}
bool CDlgTalismanService::IsEnchantGreen()
{
    if(m_pItema->GetDataPtr("ptr_CECIvtrItem") != NULL && m_pItemd->GetDataPtr("ptr_CECIvtrItem") != NULL)
	{
	   CECInventory *pPack = GetHostPlayer()->GetPack();
	   CECIvtrItem *pItem;
	   pItem = pPack->GetItem(m_pItemd->GetData());
	   if(pItem && (pItem->GetTemplateID() >= 23860 && pItem->GetTemplateID() <= 23862))
		  return true;
	}
	return false;
}
void CDlgTalismanService::OnTick()
{
	if (m_pTxt_Money1)
	{
		int nMoney = m_pTxt_Money1->GetData();
		ACHAR szMoney[20];
		a_sprintf(szMoney, _AL("%d"), nMoney % 100);
		m_pTxt_Money3->SetText(szMoney);
		a_sprintf(szMoney, _AL("%d"), (nMoney % 10000) / 100);
		m_pTxt_Money2->SetText(szMoney);
		a_sprintf(szMoney, _AL("%d"), nMoney / 10000);
		m_pTxt_Money1->SetText(szMoney);
		if( nMoney > GetHostPlayer()->GetMoneyAmount() )
		{
			m_pTxt_Money1->SetColor(A3DCOLORRGB(255, 0, 0));
			m_pTxt_Money2->SetColor(A3DCOLORRGB(255, 0, 0));
			m_pTxt_Money3->SetColor(A3DCOLORRGB(255, 0, 0));
		}
		else
		{
			m_pTxt_Money1->SetColor(A3DCOLORRGB(255, 255, 255));
			m_pTxt_Money2->SetColor(A3DCOLORRGB(255, 255, 255));
			m_pTxt_Money3->SetColor(A3DCOLORRGB(255, 255, 255));
		}
	}
	CheckItem(m_pItema);
	CheckItem(m_pItemb);
	CheckItem(m_pItemc);
	CheckItem(m_pItemd);
	CheckItem(m_pIteme);
	CheckItem(m_pItemf);
	if( stricmp(m_szName, "Win_TalismanClean") == 0 )
		m_pBtn_Enchase->Enable(m_pItema->GetDataPtr("ptr_CECIvtrItem") != NULL);
	else if( stricmp(m_szName, "Win_TalismanCombine") == 0 )
	{
		m_pBtn_Exchange->Enable(m_pItema->GetDataPtr("ptr_CECIvtrItem") != NULL);
		m_pBtn_Enchase->Enable(m_pItema->GetDataPtr("ptr_CECIvtrItem") != NULL && m_pItemb->GetDataPtr("ptr_CECIvtrItem") && m_pItemd->GetDataPtr("ptr_CECIvtrItem"));
	}
	else if( stricmp(m_szName, "Win_TalismanRefine") == 0 )
		m_pBtn_Enchase->Enable(m_pItema->GetDataPtr("ptr_CECIvtrItem") != NULL && m_pItemc->GetDataPtr("ptr_CECIvtrItem") != NULL);
	else if( stricmp(m_szName, "Win_TalismanUpgrade") == 0 )
		m_pBtn_Enchase->Enable(m_pItema->GetDataPtr("ptr_CECIvtrItem") != NULL);
	else if( stricmp(m_szName, "Win_TalismanEnchanting") == 0 )
	{

		m_pBtn_EnchantStart->Show(true);
		m_pBtn_EnchantStart->Enable(false);
		m_pBtn_EnchantStop->Show(false);
		m_pBtn_Enchase->Enable(false);

		if(m_bEnchangtStart && IsEnchantGreen())
		{
		    m_pBtn_EnchantStart->Show(false);
			m_pBtn_EnchantStop->Show(true);
			if(!m_bEnchangting)
               m_pBtn_EnchantStop->Enable(false);
			else
			   m_pBtn_EnchantStop->Enable(true);

			if(NULL != m_pItema->GetDataPtr("ptr_CECIvtrItem"))
			{
			      CECIvtrItem *pIvtrSrc = GetHostPlayer()->GetPack()->GetItem(m_pItema->GetData());
				  if(NULL != pIvtrSrc)
					 pIvtrSrc->Freeze(true);
			}
			return;
		}
		if(!m_bEnchangtStart && IsEnchantGreen())
		{
		    m_pBtn_EnchantStart->Enable(true);
		}
		else
		{
		   m_pBtn_Enchase->Enable(!m_bEnchangtStart && m_pItema->GetDataPtr("ptr_CECIvtrItem") != NULL && m_pItemd->GetDataPtr("ptr_CECIvtrItem") != NULL);
		}
	}
	else if( 0 == stricmp(GetName(), "Win_TalismanLevelUp") )
		m_pBtn_Enchase->Enable(m_pItema->GetDataPtr("ptr_CECIvtrItem") != NULL && m_pItemd->GetDataPtr("ptr_CECIvtrItem") != NULL);
	else if( 0 == stricmp(GetName(), "Win_TalismanCombineSkill") )
	{
		CECIvtrTalismanMain *pItem = (CECIvtrTalismanMain*)m_pItema->GetDataPtr("ptr_CECIvtrItem");
		m_pBtn_Enchase->Enable(pItem != NULL && m_pItemb->GetDataPtr("ptr_CECIvtrItem") != NULL
		&& m_pItemd->GetDataPtr("ptr_CECIvtrItem") != NULL);

		if (pItem)
		{
			int index = m_pItema->GetData();
			int id = pItem->GetTemplateID();
			PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_TalismanCombineSkill_Info");
			pDlg->SetData(m_pItema->GetData(), "item_index");
			pDlg->SetDataPtr((void*)pItem->GetTemplateID(), "item_id");
			if (!GetGameUIMan()->CommonMessage("Win_TalismanCombineSkill_Info", "start", m_pItema->GetData(), pItem->GetTemplateID()))
			{
				Show(true);
				OnEventLButtonDown(0, 0, m_pItema);
				GetGameUIMan()->CommonMessage("Win_TalismanCombineSkill_Info", "start", index, id);
			}
		}
	}
	else if( 0 == stricmp(GetName(), "Win_TalismanEnchaseSkill") )
		m_pBtn_Enchase->Enable(m_pItema->GetDataPtr("ptr_CECIvtrItem") != NULL && m_pItemb->GetDataPtr("ptr_CECIvtrItem") != NULL
		&& m_pIteme->GetDataPtr("ptr_CECIvtrItem") != NULL && m_pItemf->GetDataPtr("ptr_CECIvtrItem") != NULL);
}

void CDlgTalismanService::OnCommandCANCEL(const char * szCommand)
{
	Show(false);
	GetHostPlayer()->EndNPCService();
	GetGameUIMan()->m_pCurNPCEssence = NULL;
	GetGameUIMan()->m_pDlgInventory->Show(false);
	m_bEnchangting = false;
	m_bEnchangtStart = false;
	if( m_pItema )
		OnEventLButtonDown(0, 0, m_pItema);
	if( m_pItemb )
		OnEventLButtonDown(0, 0, m_pItemb);
	if( m_pItemc )
		OnEventLButtonDown(0, 0, m_pItemc);
	if( m_pItemd )
		OnEventLButtonDown(0, 0, m_pItemd);
	GetGameUIMan()->UnfreezeAllItems();
	if(m_pTxt_Money1)
		m_pTxt_Money1->SetData(0);
	m_ItemID.clear();
	
}

void CDlgTalismanService::OnCommandHelp(const char * szCommand)
{
	char szName[100];
	sprintf(szName, "%s_Help", GetName());
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog(szName);
	if( pDlg )
		pDlg->Show(!pDlg->IsShow());
}
void CDlgTalismanService::OnShowDialog() 
{
	m_bLock = false;
	if( 0 == stricmp(GetName(), "Win_TalismanEnchanting") )
	{
		OnEventLButtonDown(0,0,m_pItema);
		OnEventLButtonDown(0,0,m_pItemd);
	}
}
void CDlgTalismanService::OnCommandEnchantStart(const char* szCommand)
{
	 if( 0 == stricmp(GetName(), "Win_TalismanEnchanting") )
	 {
		 OnCommandStart("");
		 m_bEnchangting = false;
		 if(m_bEnchangtStart)
		 {
			 m_bEnchangting = true;
		 }
	 }

}
void CDlgTalismanService::OnCommandEnchantStop(const char* szCommand)
{
	 m_bEnchangting = false;

}
void CDlgTalismanService::ContinueEnchant()
{
	if( 0 == stricmp(GetName(), "Win_TalismanEnchanting") )
	 {

		bool needClear = true;
        if(m_bEnchangting)
		{
		  if(NULL != m_pItemd->GetDataPtr("ptr_CECIvtrItem"))
		  {

			CECInventory *pPack = GetHostPlayer()->GetPack();
			CECIvtrItem *pItem;
	        pItem = pPack->GetItem(m_pItemd->GetData());
			if(pItem && pItem->GetCount() >= 1 &&  (pItem->GetTemplateID() >= 23860 && pItem->GetTemplateID() <= 23862))
			{
	            
                CECIvtrItem *pIvtrSrc = pPack->GetItem(m_pItema->GetData());
				assert(NULL != pIvtrSrc);
                if(NULL != pIvtrSrc) 
				{
				     m_bEnchangtStart = false;
		             DragDropItem(m_pItema->GetData(),m_pItema);
					 DragDropItem(m_pItemd->GetData(),m_pItemd);
                       //检查m_pItema是否有效
					 if(NULL != m_pItema->GetDataPtr("ptr_CECIvtrItem") && NULL != m_pItemd->GetDataPtr("ptr_CECIvtrItem"))
					 {
                         needClear = false;
						 OnCommandStart("");
		                 if(m_bEnchangtStart)
						 { 
	                        return;
						 }
					 }

				}
			}


		  }
			

			m_bEnchangting = false;
		}

		if(needClear)
		{
			m_bEnchangtStart = false;
			OnEventLButtonDown(0, 0, m_pItema);
			if(NULL != m_pItemd->GetDataPtr("ptr_CECIvtrItem"))
			{
				CECInventory *pPack = GetHostPlayer()->GetPack();
				CECIvtrItem *pItem;
	            pItem = pPack->GetItem(m_pItemd->GetData());
				if(pItem && pItem->GetCount() >= 1)
					return;
			}
			OnEventLButtonDown(0,0,m_pItemd);

			
		}

	}
}
void CDlgTalismanService::OnCommandStart(const char * szCommand)
{
	m_bEnchangtStart = false;
	PAUIDIALOG pMsgBox;
	CECHostPlayer *pHost = GetHostPlayer();

	if( !m_pItema->GetDataPtr("ptr_CECIvtrItem") ) return;

	if (m_pTxt_Money1)
	{
		int nMoney = m_pTxt_Money1->GetData();
		if( nMoney > pHost->GetMoneyAmount() )
		{
			GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(656), MB_OK,
				A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
			return;
		}
	}
	
	CECIvtrItem *pIvtrA = (CECIvtrItem *)m_pItema->GetDataPtr("ptr_CECIvtrItem");

	CECIvtrTalismanMain *pEquipA = (CECIvtrTalismanMain*)pIvtrA;
	if( 0 == stricmp(GetName(), "Win_TalismanClean") )
	{
		if( pEquipA->GetEssence().ess.master_id == 0 )
		{
			GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(878), MB_OK,
				A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
			return;
		}
		else if( pEquipA->GetEssence().ess.master_id == GetHostPlayer()->GetCharacterID() )
		{
			GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(879), MB_OK,
				A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
			return;
		}
		else if( GetHostPlayer()->GetPack()->GetItemTotalNum(4679) == 0 )
		{
			GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(880), MB_OK,
				A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
			return;
		}
		if( pIvtrA )
		{
			GetGameSession()->c2s_CmdNPCSevTalismanRemoveOwner(m_pItema->GetData(), pEquipA->GetTemplateID());
			LuaEvent::FirstCleanTalisman();
			OnEventLButtonDown(0, 0, m_pItema);
		}
	}
	else if( 0 == stricmp(GetName(), "Win_TalismanCombine") )
	{
		CECIvtrItem *pIvtrB = (CECIvtrItem *)m_pItemb->GetDataPtr("ptr_CECIvtrItem");
		CECIvtrTalismanMain *pEquipB = (CECIvtrTalismanMain*)pIvtrB;
		CECIvtrItem *pIvtrD = (CECIvtrItem *)m_pItemd->GetDataPtr("ptr_CECIvtrItem");
		if( pIvtrA && pIvtrB )
		{
			if( pIvtrD )
			{
				GetGameSession()->c2s_CmdNPCSevTalismanCombine(m_pItema->GetData(), pEquipA->GetTemplateID(),
					m_pItemb->GetData(), pEquipB->GetTemplateID(), pIvtrD->GetTemplateID());
			}
			else
			{
				GetGameSession()->c2s_CmdNPCSevTalismanCombine(m_pItema->GetData(), pEquipA->GetTemplateID(),
					m_pItemb->GetData(), pEquipB->GetTemplateID(), 0);
			}
			LuaEvent::FirstCombineTalisman();
			OnEventLButtonDown(0, 0, m_pItema);
			OnEventLButtonDown(0, 0, m_pItemb);
			//OnEventLButtonDown(0, 0, m_pItemd);
		}
	}
	else if( 0 == stricmp(GetName(), "Win_TalismanEnchanting") )
	{
		CECIvtrItem *pIvtrD = (CECIvtrItem *)m_pItemd->GetDataPtr("ptr_CECIvtrItem");
		if( pIvtrA && pIvtrD )
		{
			if (GetHostPlayer()->IsSafeLock())
			{
				GetGameUIMan()->AddChatMessage(GetStringFromTable(16036), GP_CHAT_SYSTEM);
				return;
			}
			GetGameSession()->c2s_CmdNPCSevTalismanEnchant(m_pItema->GetData(), pEquipA->GetTemplateID(),
				m_pItemd->GetData(), pIvtrD->GetTemplateID());
			LuaEvent::FirstEnchantTalisman();
			m_bEnchangtStart    = true;
		}
	}
	else if( 0 == stricmp(GetName(), "Win_TalismanRefine") )
	{
		CECIvtrTalismanExpFood *pIvtrC = (CECIvtrTalismanExpFood *)m_pItemc->GetDataPtr("ptr_CECIvtrItem");
		if( pIvtrA && pIvtrC )
		{
			if( pEquipA->GetEssence().ess.master_id != GetHostPlayer()->GetCharacterID() )
			{
				GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(892), MB_OK,
					A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
				return;
			}
			if( pEquipA->GetEssence().ess.exp >= GetHostPlayer()->GetTalismanLevelUpExp(pEquipA->GetEssence().ess.level + 1) )
			{
				GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(883), MB_OK,
					A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
				return;
			}
			{
				if( !pEquipA->GetDBEssence()->is_aircraft && pEquipA->GetEssence().ess.level >=GetMaxLevelForRefine())
				{
					GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(891), MB_OK,
						A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
					return;
				}

			}
			if( pEquipA->GetDBEssence()->is_aircraft && pIvtrC->GetDBEssence()->food_type == 0 || 
				!pEquipA->GetDBEssence()->is_aircraft && pIvtrC->GetDBEssence()->food_type == 1 )
			{
				GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(996), MB_OK,
					A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
				return;
			}
			GetGameSession()->c2s_CmdNPCSevTalismanRefine(m_pItema->GetData(), pEquipA->GetTemplateID(),
				pIvtrC->GetTemplateID());
			LuaEvent::FirstRefineTalisman();
		}
	}
	else if( 0 == stricmp(GetName(), "Win_TalismanUpgrade") )
	{
		if( pIvtrA )
		{
			if( pEquipA->GetEssence().ess.master_id != GetHostPlayer()->GetCharacterID() )
			{
				GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(893), MB_OK,
					A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
				return;
			}
			if( !pEquipA->GetDBEssence()->is_aircraft && pEquipA->GetEssence().ess.exp < GetHostPlayer()->GetTalismanLevelUpExp(pEquipA->GetEssence().ess.level + 1) )
			{
				GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(881), MB_OK,
					A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
				return;
			}
			if( pEquipA->GetEssence().ess.level >= ((TALISMAN_MAINPART_ESSENCE*)pEquipA->GetDBEssence())->max_level2 )
			{
				GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(882), MB_OK,
					A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
				return;
			}
			GetGameSession()->c2s_CmdNPCSevTalismanUpgrade(m_pItema->GetData(), pEquipA->GetTemplateID());
		}
	}
	else if( 0 == stricmp(GetName(), "Win_TalismanLevelUp") )
	{
		CECIvtrItem *pItemD = (CECIvtrItem *)m_pItemd->GetDataPtr("ptr_CECIvtrItem");
		GetGameSession()->c2s_CmdNPCSevTalismanHoleyLevelUp(m_pItema->GetData(), pEquipA->GetTemplateID(), pItemD->GetTemplateID());
        OnEventLButtonDown(0, 0, m_pItema);
	}
	else if( 0 == stricmp(GetName(), "Win_TalismanCombineSkill") )
	{
		CECIvtrTalismanMain *pItemB = (CECIvtrTalismanMain*)m_pItemb->GetDataPtr("ptr_CECIvtrItem");
		if (pItemB->GetFixSkillLevel()>1 || ((pItemB->GetEssence().ess.quality >> 26)&0x3)==2)
		{
			ACString strText;
			if (pItemB->GetFixSkillLevel()>1 && ((pItemB->GetEssence().ess.quality >> 26)&0x3)==2)
				strText = GetGameUIMan()->GetStringFromTable(15038);
			else
				strText = GetGameUIMan()->GetStringFromTable(pItemB->GetFixSkillLevel()>1 ? 15036:15037);
			GetGameUIMan()->MessageBox("Game_TalismanComboSkill", strText, MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160));
		}
		else
			CommonMessage("comboskill", 0, 0);
	}
	else if( 0 == stricmp(GetName(), "Win_TalismanEnchaseSkill") )
	{
		CECIvtrItem *pItemB = (CECIvtrItem *)m_pItemb->GetDataPtr("ptr_CECIvtrItem");
		CECIvtrItem *pItemE = (CECIvtrItem *)m_pIteme->GetDataPtr("ptr_CECIvtrItem");
		CECIvtrItem *pItemF = (CECIvtrItem *)m_pItemf->GetDataPtr("ptr_CECIvtrItem");
		GetGameSession()->c2s_CmdNPCSevTalismanSkillEmbed(m_pItema->GetData(), pEquipA->GetTemplateID(),
			m_pItemb->GetData(), pItemB->GetTemplateID(), m_pIteme->GetData(), pItemE->GetTemplateID(), m_pItemf->GetData(), pItemF->GetTemplateID());
		OnEventLButtonDown(0, 0, m_pItema);
		OnEventLButtonDown(0, 0, m_pItemb);
		OnEventLButtonDown(0, 0, m_pIteme);
		OnEventLButtonDown(0, 0, m_pItemf);
	}
}

int CDlgTalismanService::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="comboskill")
	{
		CECIvtrTalismanMain *pEquipA = (CECIvtrTalismanMain*)(CECIvtrItem *)m_pItema->GetDataPtr("ptr_CECIvtrItem");
		CECIvtrTalismanMain *pItemB = (CECIvtrTalismanMain*)m_pItemb->GetDataPtr("ptr_CECIvtrItem");
		CECIvtrItem *pItemD = (CECIvtrItem *)m_pItemd->GetDataPtr("ptr_CECIvtrItem");
		GetGameSession()->c2s_CmdNPCSevTalismanSkillRefine(m_pItema->GetData(), pEquipA->GetTemplateID(),
			m_pItemb->GetData(), pItemB->GetTemplateID(), pItemD->GetTemplateID());
		OnEventLButtonDown(0, 0, m_pItema);
		OnEventLButtonDown(0, 0, m_pItemb);
		OnEventLButtonDown(0, 0, m_pItemd);
	}
	else if (strMsg=="combo_finish")
	{
		m_bLock = wParam>0;
	}

	return 0;
}

void CDlgTalismanService::OnCommandShopExchange(const char * szCommand)
{
	if (m_pItema->GetDataPtr("ptr_CECIvtrItem"))
	{
		CECIvtrItem *pIvtrA = (CECIvtrItem *)m_pItema->GetDataPtr("ptr_CECIvtrItem");
		GetGameUIMan()->GetDialog("Win_TalismanMedium")->SetData(pIvtrA->GetTemplateID(), "talisman_id");
		GetGameUIMan()->GetDialog("Win_TalismanMedium")->Show(true);
	}
}

void CDlgTalismanService::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{

	if (m_bEnchangtStart &&  0 == stricmp(GetName(), "Win_TalismanEnchanting"))
    {
    	  return;
	}
	if( !pObj || !pObj->GetDataPtr("ptr_CECIvtrItem") ) return;
	AString       strName = pObj->GetName();
	CECInventory *pPack = GetHostPlayer()->GetPack();
	CECIvtrItem * pItem = pPack->GetItem(pObj->GetData());
	if(NULL != pItem && pItem->GetTemplateID() == m_ItemID[strName])
	{
	   pItem->Freeze(false);
	   GetGameUIMan()->PlayItemSound(pItem, false);
	}
	ClearImagePicControl(pObj);
	
}
void CDlgTalismanService::ClearImagePicControl(AUIObject * pObj)
{
   	PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)pObj;
	pImage->SetDataPtr(NULL);
	pImage->SetData(0);
	pImage->SetHint(_AL(""));
	pImage->SetCover(NULL, -1);
	pImage->SetCover(NULL, 0, 1);
}
void CDlgTalismanService::DragDropItem(int iSrc, PAUIOBJECT pObjOver)
{
	if (m_bLock)
		return;

	CECIvtrItem *pIvtrSrc = GetHostPlayer()->GetPack()->GetItem(iSrc);

	if( !pObjOver )
		return;
	
	if( 0 == stricmp(pObjOver->GetName(), "Item_a") && pIvtrSrc->GetClassID() != CECIvtrItem::ICID_TALISMAN_MAINPART )
		return;

	if ((m_szName=="Win_TalismanCombineSkill" || m_szName=="Win_TalismanEnchaseSkill") && pIvtrSrc->GetExpireDate()!=0)
		return;
	
	if( 0 == stricmp(pObjOver->GetName(), "Item_b") && pIvtrSrc->GetClassID() != CECIvtrItem::ICID_TALISMAN_MAINPART )
		return;

	if( 0 == stricmp(pObjOver->GetName(), "Item_c") && pIvtrSrc->GetClassID() != CECIvtrItem::ICID_TALISMAN_EXPFOOD )
		return;

	if( 0 == stricmp(pObjOver->GetName(), "Item_d") && pIvtrSrc->GetClassID() != CECIvtrItem::ICID_TALISMAN_MERGEKATALYST )
		return;

	if( 0 == stricmp(pObjOver->GetName(), "Item_e") )
	{
		if ( pIvtrSrc->GetClassID() != CECIvtrItem::ICID_EQUIP )
			return;
		CECIvtrEquip *pEquip = (CECIvtrEquip*)pIvtrSrc;
		if ( (pEquip->GetDBEssence()->equip_mask&EQUIP_MASK_SUB_TALISMAN1)==0 )
			return;
	}

	if( 0 == stricmp(pObjOver->GetName(), "Item_f") )
	{
		if ( pIvtrSrc->GetClassID() != CECIvtrItem::ICID_EQUIP )
			return;
		CECIvtrEquip *pEquip = (CECIvtrEquip*)pIvtrSrc;
		if ( (pEquip->GetDBEssence()->equip_mask&EQUIP_MASK_SUB_TALISMAN2)==0 )
			return;
	}

	if( 0 == stricmp(pObjOver->GetName(), "Item_e") || 0 == stricmp(pObjOver->GetName(), "Item_f") )
	{
		if (!m_pItemb->GetDataPtr("ptr_CECIvtrItem"))
			return;

		CECIvtrTalismanMain *pSubItem = (CECIvtrTalismanMain*)m_pItemb->GetDataPtr("ptr_CECIvtrItem");
		int idSuite = pSubItem->GetSuiteID();
		DATA_TYPE DataType = DT_INVALID;
		elementdataman* pDataMan = GetGame()->GetElementDataMan();
		const void* pData = pDataMan->get_data_ptr(idSuite, ID_SPACE_ESSENCE, DataType);
		if (DataType != DT_SUITE_ESSENCE)
		{
			ASSERT(DataType == DT_SUITE_ESSENCE);
			return;
		}
		const SUITE_ESSENCE* pSuiteEss = (const SUITE_ESSENCE*)pData;
		int i;
		for (i=0; i<14; ++i)
		{
			if (pIvtrSrc->GetTemplateID()==pSuiteEss->equipments[i].id)
			{
				break;
			}
		}
		if (i==14)
			return;
	}

	if ( 0 == stricmp(pObjOver->GetName(), "Item_a") && m_szName=="Win_TalismanEnchaseSkill" )
	{
		CECIvtrTalismanMain *pSubItem = (CECIvtrTalismanMain*)pIvtrSrc;
		if (pSubItem->GetRebornCount()==0)
			return;
	}
    if ( 0 == stricmp(GetName(), "Win_TalismanEnchanting"))
    {
         if(m_bEnchangtStart)
		 {
                 if(  0 == stricmp(pObjOver->GetName(), "Item_a"))
				 {
					return;	 
				 }
				 else if( 0 == stricmp(pObjOver->GetName(), "Item_d"))
				 {
					 return;
				 }
		 }
    }
	OnEventLButtonDown(0, 0, pObjOver);
	AString strFile;
	af_GetFileTitle(pIvtrSrc->GetIconFile(), strFile);
	strFile.MakeLower();
	PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)pObjOver;
	pImage->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
		GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
	if (pIvtrSrc->IsLocked())
		pImage->SetCover(CDlgItemLock::m_pA2DSpriteLock, 0, 1);

	GetGameUIMan()->PlayItemSound(pIvtrSrc, true);
	pIvtrSrc->Freeze(true);
	pObjOver->SetData(iSrc);
	pObjOver->SetDataPtr(pIvtrSrc, "ptr_CECIvtrItem");
	AString strName = pObjOver->GetName();
	m_ItemID[strName] = pIvtrSrc->GetTemplateID();
	if( 0 == stricmp(pObjOver->GetName(), "Item_a") && m_pTxt_Money1 )
	{
		const TALISMAN_MAINPART_ESSENCE *pDBEssence = (TALISMAN_MAINPART_ESSENCE *)(((CECIvtrTalismanMain*)pIvtrSrc)->GetDBEssence());
		if( stricmp(m_szName, "Win_TalismanClean") == 0 )
			m_pTxt_Money1->SetData(pDBEssence->fee_reset);
		else if( stricmp(m_szName, "Win_TalismanCombine") == 0 )
			m_pTxt_Money1->SetData(GetGameRun()->GetSpecialIDConfig().fee_talisman_merge);
		else if( stricmp(m_szName, "Win_TalismanEnchanting") == 0 )
			m_pTxt_Money1->SetData(GetGameRun()->GetSpecialIDConfig().fee_talisman_enchant);
		else if( stricmp(m_szName, "Win_TalismanRefine") == 0 )
			m_pTxt_Money1->SetData(pDBEssence->fee_expfood);
		else if( stricmp(m_szName, "Win_TalismanUpgrade") == 0 )
			m_pTxt_Money1->SetData(pDBEssence->fee_levelup);
	}
}