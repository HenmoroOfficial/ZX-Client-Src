// Filename	: DlgDragDrop.cpp
// Creator	: Tom Zhou
// Date		: October 20, 2005

#include "AFI.h"
#include "A2DSprite.h"
#include "A2DSpriteItem.h"
#include "AUI\\AUIDef.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUIProgress.h"
#include "elementdataman.h"
#include "DlgDragDrop.h"
#include "DlgEquipBind.h"
#include "DlgEquipDestroy.h"
#include "DlgEquipUnDestroy.h"
#include "DlgBooth.h"
#include "DlgAuctionSellList.h"
#include "DlgInputNO.h"
#include "DlgInstall.h"
#include "DlgShop.h"
#include "DlgShopReputation.h"
#include "DlgSkill.h"
#include "DlgSkillEdit.h"
#include "DlgSkillEdit1.h"
#include "DlgInventory.h"
#include "DlgTrade.h"
#include "DlgTalismanService.h"
#include "DlgTalisman.h"
#include "DlgMailWrite.h"
#include "DlgPetService.h"
#include "DlgResetProp.h"
#include "DlgStorage.h"
#include "DlgPetAction.h"
#include "DlgHorseTame.h"
#include "DlgQuickbar.h"
#include "DlgItemLock.h"
#include "DlgMend.h"
#include "DlgSoul.h"
#include "DlgSoulCombine.h"
#include "DlgGardenMan.h"
#include "DlgGardenInventory.h"
#include "DlgPetPackage.h"
#include "DlgInventoryCostume.h"
#include "DlgMarkAstro.h"
#include "DlgItemSvrBase.h"
#include "DlgRuneOperate.h"
#include "DlgPetDetail.h"
#include "DlgReName.h"
#include "DlgAgentCombined.h"	//2012-10-15 by add zy 药剂组合界面
#include "DlgRemedyMetempsychosis.h"	//2012-10-15 by add zy 药剂组合界面
#include "DlgInventoryRide.h"
#include "DlgInventoryExpand.h"
#include "DlgIndividualityColor.h"

#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameSession.h"
#include "EC_GameUIMan.h"
#include "EC_Inventory.h"
#include "EC_DealInventory.h"
#include "EC_HostPlayer.h"
#include "EC_Shortcut.h"
#include "EC_ShortcutSet.h"
#include "EC_Skill.h"
#include "EC_IvtrConsume.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrEquip.h"
#include "EC_IvtrTalismanMain.h"
#include "EC_IvtrStone.h"
#include "EC_IvtrScroll.h"
#include "EC_IvtrHTItem.h"
#include "EC_GameRun.h"
#include "EC_UIManager.h"
#include "ECScriptMan.h"
#include "ECScriptContext.h"
#include "ECScriptCheckStateInGame.h"
#include "LuaEvent.h"
#include "EC_World.h"
#include "EC_ManPlayer.h"
#include "EC_ManNPC.h"
#include "EC_NPC.h"
#include "EC_NewHelpGuide.h"

#define new A_DEBUG_NEW
#define CLEAN_RETURN { Show(false); m_pImgGoods->ClearCover(); return; }
#define MSGBOX(sid) GetGameUIMan()->MessageBox("", GetStringFromTable(sid), MB_OK, A3DCOLORRGBA(255, 255, 255, 160))

AUI_BEGIN_EVENT_MAP(CDlgDragDrop, CDlgBase)

AUI_ON_EVENT("Goods",	WM_LBUTTONUP,	OnEventLButtonUp)
AUI_ON_EVENT("*",		WM_MOUSEMOVE,	OnEventMouseMove)

AUI_END_EVENT_MAP()

CDlgDragDrop::CDlgDragDrop()
{
}

CDlgDragDrop::~CDlgDragDrop()
{
}

bool CDlgDragDrop::OnInitDialog()
{
	m_pImgGoods = (PAUIIMAGEPICTURE)GetDlgItem("Goods");
	m_mapFuncs.clear();
	m_mapFuncs["execute"]				= &CDlgDragDrop::DragExecute;
	m_mapFuncs["Win_Trade"]				= &CDlgDragDrop::DragTrade;
	m_mapFuncs["Win_Garden_Inventory"]	= &CDlgDragDrop::DragGardenInventory;
	m_mapFuncs["Win_Inventory"]			= &CDlgDragDrop::DragInventory;
	m_mapFuncs["Win_InventoryPet"]		= &CDlgDragDrop::DragInventoryPet;
	m_mapFuncs["Win_Inventory_Costume"]	= &CDlgDragDrop::DragInventoryCostume;
	m_mapFuncs["Win_Inventory_Vechile"]	= &CDlgDragDrop::DragInventoryWing;
	m_mapFuncs["Win_Character"]			= &CDlgDragDrop::DragCharacter;
	m_mapFuncs["Win_Action"]			= &CDlgDragDrop::DragAction;
	m_mapFuncs["Win_SkillComboEdit"]	= &CDlgDragDrop::DragSkillComboEdit;
	m_mapFuncs["Win_FamilyManage"]		= &CDlgDragDrop::DragSkill;
	m_mapFuncs["Win_TeacherManage"]		= &CDlgDragDrop::DragSkill;
	m_mapFuncs["Win_Buddhism"]			= &CDlgDragDrop::DragSkill;
	m_mapFuncs["Win_Evil"]				= &CDlgDragDrop::DragSkill;
	m_mapFuncs["Win_Taoism"]			= &CDlgDragDrop::DragSkill;
	m_mapFuncs["Win_BodyChangeSkill"]	= &CDlgDragDrop::DragSkill;
	m_mapFuncs["Win_BodyChangeSkill"]	= &CDlgDragDrop::DragSkill;
	m_mapFuncs["Win_SkillContinual"]	= &CDlgDragDrop::DragSkill;
	m_mapFuncs["Win_TalismanSmall"]		= &CDlgDragDrop::DragTalismanSmall;
	m_mapFuncs["Win_Talisman"]			= &CDlgDragDrop::DragTalisman;
	m_mapFuncs["Win_Pet_Skill"]			= &CDlgDragDrop::DragPet;
	m_mapFuncs["Win_PetAutoFood"]		= &CDlgDragDrop::DragPet;
	m_mapFuncs["Win_PetPackage"]		= &CDlgDragDrop::DragPet;
	m_mapFuncs["Win_Shop"]				= &CDlgDragDrop::DragShop;
	m_mapFuncs["Win_Storage"]			= &CDlgDragDrop::DragStorage;
	m_mapFuncs["Win_Split"]				= &CDlgDragDrop::DragSplit;
	m_mapFuncs["Win_Identify"]			= &CDlgDragDrop::DragIdentify;
	m_mapFuncs["Win_PShop2"]			= &CDlgDragDrop::DragPShop2;
	m_mapFuncs["Win_PetEquip"]			= &CDlgDragDrop::DragPetEquip;
	m_mapFuncs["Win_TargetPlayer"]		= &CDlgDragDrop::DragTarget;
	m_mapFuncs["Win_TargetMonster"]		= &CDlgDragDrop::DragTarget;
	m_mapFuncs["Win_Shop_Credit"]		= &CDlgDragDrop::DragShopReputation;
	m_mapFuncs["Win_Equip_Rune_Enchanting"] = &CDlgDragDrop::DragRune2013;

	m_nOriSize = m_pImgGoods->GetSize();

	return true;
}

void CDlgDragDrop::OnEventMouseMove(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	return;
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;
//	if( x != GetGameUIMan()->m_ptLButtonDown.x ||
//		y != GetGameUIMan()->m_ptLButtonDown.y )
}

void CDlgDragDrop::OnEventLButtonUp(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	m_pImgGoods->SetSize(m_nOriSize.cx, m_nOriSize.cy);

	m_pObjSrc = (PAUIOBJECT)m_pImgGoods->GetDataPtr("ptr_AUIObject");
	if( !m_pObjSrc )
		CLEAN_RETURN;
	void *ptr;
	AString szType;
	m_pObjSrc->ForceGetDataPtr(ptr,szType);
//	if( !ptr ) CLEAN_RETURN;

	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;
	GetGameUIMan()->HitTest(x, y, &m_pDlgDst, &m_pObjDst, this);

	m_nDstInv = 0;
	m_nSrcInv = GetData();
	if( m_pDlgDst )
	{
		if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Inventory") )
			m_nDstInv = GetGameUIMan()->m_pDlgInventory->GetFirstItem();
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Storage") )
			m_nDstInv = GetGameUIMan()->m_pDlgStorage->GetFirstItem();
		else if ( 0 == stricmp(m_pDlgDst->GetName(), "Win_InventoryPet") )
		{
			m_nDstInv = GetGameUIMan()->m_pDlgInventoryPocket->GetFirstItem();
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Inventory_Costume") )
		{
			CDlgInventoryCostume *pDlg = (CDlgInventoryCostume*)GetGameUIMan()->GetDialog("Win_Inventory_Costume");
			m_nDstInv = pDlg->GetFirstItem();
		}
	}

	m_pDlgSrc = m_pObjSrc->GetParent();
	if( szType == "ptr_CECIvtrItem")
		m_pIvtrSrc = (CECIvtrItem *)ptr;
	else
		m_pIvtrSrc = NULL;
	
	if( abs(x - GetGameUIMan()->m_ptLButtonDown.x) < 3 &&
		abs(y - GetGameUIMan()->m_ptLButtonDown.y) < 3 )
	{
		DragExecute();
		CLEAN_RETURN;
	}
	else if (m_mapFuncs.find(m_pDlgSrc->GetName())!= m_mapFuncs.end())
	{
		(this->*m_mapFuncs[m_pDlgSrc->GetName()])();
	}
	else if (strstr(m_pDlgSrc->GetName(), "Win_Skill") != NULL ||
		strstr(m_pDlgSrc->GetName(), "Win_Talent") != NULL)
	{
		DragSkill();
	}
	else if (strstr(m_pDlgSrc->GetName(), "Win_QuickBar") != NULL)
	{
		DragQuickBar();
	}
	
	CLEAN_RETURN;
}

void CDlgDragDrop::DragExecute()
{
	int iSrc;
	if( strstr(m_pDlgSrc->GetName(), "Win_QuickBar") || 0 == stricmp(m_pDlgSrc->GetName(), "Win_Action") || 0 == stricmp(m_pDlgSrc->GetName(), "Win_PetAction"))
	{
		CECShortcut *pSC = (CECShortcut *)GetDataPtr("ptr_CECShortcut");
		if( pSC && m_pObjSrc->GetDataPtr("ptr_CECShortcut") == pSC )
		{
			pSC->Execute();

			{
			   if(0 == stricmp(m_pDlgSrc->GetName(),"Win_QuickBar8H_1"))
			   {
				   int id = atoi(m_pObjSrc->GetName()+strlen("Item_"));
                   if(1 == id)
					 NewHelpGuide::FinishGuide(GUIDE_HELP_FIGHT_2);
				   else if(2 == id)
                     NewHelpGuide::FinishGuide(GUIDE_HELP_USESKILL_1);
				   else if(3 == id)
				     NewHelpGuide::FinishGuide(GUIDE_HELP_USEMEDICINE_1);
			   }
			}
		}
	}
	else if( 0 == stricmp(m_pDlgSrc->GetName(), "Win_SkillContinual"))
	{
		if( strstr(m_pObjSrc->GetName(), "Edt_Item_") )
		{
			CDlgSkillEdit *pDlgSkillCombo = (CDlgSkillEdit*)GetGameUIMan()->GetDialog("Win_SkillContinual");
			pDlgSkillCombo->DragDropItem(NULL, m_pObjSrc, m_pObjDst);
		}
		else if( strstr(m_pObjSrc->GetName(), "Item_") )
		{
			iSrc = atoi(m_pObjSrc->GetName() + strlen("Item_"));
			CDlgSkillEdit *pDlgSkillCombo = (CDlgSkillEdit*)GetGameUIMan()->GetDialog("Win_SkillContinual");
			pDlgSkillCombo->SelectComboSkill(iSrc);
		}
	}
	
	else if( strstr(m_pDlgSrc->GetName(), "Win_Skill") != NULL ||
		0 == stricmp(m_pDlgSrc->GetName(), "Win_FamilyManage") || 0 == stricmp(m_pDlgSrc->GetName(), "Win_TeacherManage"))
	{
		CECSkill *pSkill = (CECSkill *)GetDataPtr("ptr_CECSkill");
		if( pSkill ) GetHostPlayer()->ApplySkillShortcut(pSkill->GetSkillID());
	}
	else if( strstr(m_pDlgSrc->GetName(), "Win_BodyChangeSkill") )
	{
		if( strstr(m_pObjSrc->GetName(), "ChangeSkill_") )
		{
			CECSkill *pSkill = (CECSkill *)GetDataPtr("ptr_CECSkill");
			if( pSkill ) GetHostPlayer()->ApplySkillShortcut(pSkill->GetSkillID());
		}
	}
	else if( strstr(m_pDlgSrc->GetName(), "Win_TalismanSmall"))
	{
		CDlgTalisman *pDlgTalismanSmall = (CDlgTalisman*)m_pDlgSrc;
		if( strstr(m_pObjSrc->GetName(), "Img_Skill") )
		{
			int iSlot = atoi(m_pObjSrc->GetName() + strlen("Img_Skill"));
			pDlgTalismanSmall->UseTalismanSkill(iSlot);
		}
		else if( strstr(m_pObjSrc->GetName(), "Img_Item") )
		{
			int iSlot = atoi(m_pObjSrc->GetName() + strlen("Img_Item"));
			pDlgTalismanSmall->UseTalismanItem(iSlot+9);
		}
	}
	else if( strstr(m_pDlgSrc->GetName(), "Win_Talisman"))
	{
		if( strstr(m_pObjSrc->GetName(), "Item_") )
		{
			int iSlot = atoi(m_pObjSrc->GetName() + strlen("Item_"));
			GetGameUIMan()->m_pDlgTalisman->UseTalismanItem(iSlot);
		}
		else
		{
			int iSlot = atoi(m_pObjSrc->GetName() + strlen("Img_Skill"));
			GetGameUIMan()->m_pDlgTalisman->UseTalismanSkill(iSlot);
		}
	}
	else if( strstr(m_pDlgSrc->GetName(), "Win_PetAction") )
	{
		CECShortcut *pSC = (CECShortcut *)GetDataPtr("ptr_CECShortcut");
		if( pSC && m_pObjSrc->GetDataPtr("ptr_CECShortcut") == pSC )
			pSC->Execute();
	}
	else if( strstr(m_pDlgSrc->GetName(), "Win_Character") )
	{
		if (m_pObjSrc==m_pDlgSrc->GetDlgItem("Equip_23") && m_pObjSrc->GetDataPtr("ptr_CECIvtrItem"))	// 元魂界面
		{
			CECIvtrItem *pItem = (CECIvtrItem *)m_pObjSrc->GetDataPtr("ptr_CECIvtrItem");
			PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Equip_Yuanying");
			pDlg->SetData(EQUIPIVTR_RUNE, "rune_index");
			pDlg->SetDataPtr(pItem, "CECIvtrRune_Ptr");
			pDlg->Show(true);
		}
	}
	else if( strstr(m_pDlgSrc->GetName(), "Win_Inventory_Vechile") )
	{
		GetGameUIMan()->CommonMessage("Win_Inventory_Vechile", "wear", (int)m_pObjSrc, 0);
	}

	GetGameUIMan()->m_ptLButtonDown.x = -1;
	GetGameUIMan()->m_ptLButtonDown.y = -1;
}


void CDlgDragDrop::DragTrade()
{
	int idItem = m_pIvtrSrc->GetTemplateID();
	int nCount = m_pIvtrSrc->GetCount();
	if( strstr(m_pObjSrc->GetName(), "I_") &&
		0 == stricmp(m_pDlgSrc->GetName(), "Win_Trade") )
	{
		if( !m_pDlgDst || 0 != stricmp(m_pDlgDst->GetName(), "Win_Trade") )
		{
			int iSrc = atoi(m_pObjSrc->GetName() + strlen("I_")) - 1;
			const CECDealInventory::ITEMINFO &ii = GetHostPlayer()->GetDealPack()->GetItemInfo(iSrc);
			
			GetGameSession()->trade_RemoveGoods(GetGameUIMan()->m_idTrade,
				idItem, ii.iOrigin, ii.iAmount, 0);
			m_pIvtrSrc->Freeze(false);
			GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
		}
	}
}

void CDlgDragDrop::DragGardenInventory()
{
	int iSrc;
	if( strstr(m_pObjSrc->GetName(), "Item_") && m_pDlgDst && strstr(m_pDlgDst->GetName(), "Win_Garden") )
	{
		CECIvtrHTTool *htTool = (CECIvtrHTTool*)m_pObjSrc->GetDataPtr("ptr_CECIvtrItem");
		if (htTool)
		{
			int iTargetType = htTool->GetTargetType();
			if (iTargetType == 2)
			{
				CDlgGardenInventory *pDlgGardenInv = (CDlgGardenInventory*)GetGameUIMan()->GetDialog("Win_Garden_Inventory");
				pDlgGardenInv->OnEventLButtonDblclk(0, 0, m_pObjSrc);
			}
			
			if ( iTargetType == 3 && m_pObjDst && strstr(m_pObjDst->GetName(), "Img_Plant"))
			{
				iSrc = atoi(m_pObjSrc->GetName() + strlen("Item_")) + m_nSrcInv;
				if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Garden") &&
					m_pObjDst && strstr(m_pObjDst->GetName(), "Img_Plant_") )
				{
					CDlgGardenMan *pDlgGarden = (CDlgGardenMan*)GetGameUIMan()->GetDialog("Win_Garden");
					int fieldId = pDlgGarden->GetFieldId(m_pObjDst->GetData());
					if (fieldId>=0)
					{
						CDlgGardenInventory *pDlgGardenInventory = (CDlgGardenInventory*)m_pDlgSrc;
						GetGameUIMan()->MessageBox("Game_GardenUseItemField", GetGameUIMan()->GetStringFromTable(8598), 
							MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160));
						pDlgGardenInventory->SetFieldId(fieldId);
					}
				}
			}
		}
	}
	
}
	
void CDlgDragDrop::DragInventory()
{
	int iSrc, iDst;	
	AString strFile;
	if( strstr(m_pObjSrc->GetName(), "Item_") )
	{
		if(!m_pIvtrSrc)
			return;

		iSrc = atoi(m_pObjSrc->GetName() + strlen("Item_")) + m_nSrcInv;
		if( !m_pDlgDst )		// Drop item.
		{
			if( !GetGameUIMan()->GetDialog("Win_PShop1")->IsShow() )
			{
				if(!m_pIvtrSrc->IsDropable())
					MSGBOX(8462);
				else if (GetGame()->IsValuableItem(m_pIvtrSrc->GetTemplateID()))
				{
					PAUIDIALOG pMsgBox;
					GetGameUIMan()->MessageBox("Game_WorthyItemDrop",GetStringFromTable(8456), MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
					pMsgBox->SetData(m_pIvtrSrc->GetCount());
					if( AUI_PRESS(VK_MENU) && m_pIvtrSrc->GetPileLimit() > 1 && m_pIvtrSrc->GetCount() > 1 )
						pMsgBox->SetDataPtr(m_pObjSrc,"input");
					else
						pMsgBox->SetDataPtr((void*)iSrc);
				}
				else
				{
					if( AUI_PRESS(VK_MENU) && m_pIvtrSrc->GetPileLimit() > 1 && m_pIvtrSrc->GetCount() > 1 )
					{
						GetGameUIMan()->InvokeNumberDialog(m_pObjSrc, NULL,
							CDlgInputNO::INPUTNO_DROP_ITEM, m_pIvtrSrc->GetCount());
					}
					else
					{
						GetGameSession()->c2s_CmdDropIvtrItem(iSrc, m_pIvtrSrc->GetCount());
					}
					GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
				}
				LuaEvent::FirstDiscardItem();
			}
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Inventory") && 
			m_pObjDst && strstr(m_pObjDst->GetName(), "Item_") )
		{
			iDst = atoi(m_pObjDst->GetName() + strlen("Item_")) + m_nDstInv;
			if( iSrc != iDst )
			{
				CECScriptMan * pScriptMan = g_pGame->GetGameRun()->GetUIManager()->GetScriptMan();
				if( pScriptMan )
				{
					pScriptMan->GetContext()->GetCheckState()->SetInventoryItemMoved(true);
				}
				m_pIvtrDst = (CECIvtrItem *)m_pObjDst->GetDataPtr("ptr_CECIvtrItem");
				if( !m_pIvtrDst || !m_pIvtrDst->IsFrozen() )
				{
					if( m_pIvtrSrc->GetPileLimit() > 1 && (!m_pIvtrDst ||
						( m_pIvtrSrc->GetTemplateID() == m_pIvtrDst->GetTemplateID() && m_pIvtrSrc->GetStateFlag() == m_pIvtrDst->GetStateFlag() ) ) )
					{
						int nMax = m_pIvtrSrc->GetCount();
						if( m_pIvtrDst && m_pIvtrSrc->GetPileLimit() - m_pIvtrDst->GetCount() < nMax )
							nMax = m_pIvtrSrc->GetPileLimit() - m_pIvtrDst->GetCount();
						
						if( GetGameUIMan()->GetDialog("Win_Trade")->IsShow())
						{
							if( AUI_PRESS(VK_MENU) && m_pIvtrSrc->GetPileLimit() > 1 && m_pIvtrSrc->GetCount() > 1 )
								GetGameUIMan()->InvokeNumberDialog(m_pObjSrc, m_pObjDst,
								CDlgInputNO::INPUTNO_TRADE_MOVE, max(nMax, 0));
							else
								GetGameSession()->trade_MoveItem(GetGameUIMan()->m_idTrade, m_pIvtrSrc->GetTemplateID(), iSrc, iDst, nMax);
						}
						else
						{
							if( AUI_PRESS(VK_MENU) && nMax > 1 )
							{
								GetGameUIMan()->InvokeNumberDialog(m_pObjSrc, m_pObjDst,
									CDlgInputNO::INPUTNO_MOVE_ITEM, max(nMax, 0));
							}
							else if( !m_pIvtrDst ) 
								GetGameSession()->c2s_CmdExgIvtrItem(iSrc, iDst);
							else if( m_pIvtrDst && m_pIvtrSrc->GetTemplateID() == m_pIvtrDst->GetTemplateID() && nMax >= 1 )
								GetGameSession()->c2s_CmdMoveIvtrItem(iSrc, iDst, nMax);
						}
					}
					else
					{
						if( GetGameUIMan()->GetDialog("Win_Trade")->IsShow())
							GetGameSession()->trade_MoveItem(GetGameUIMan()->m_idTrade, m_pIvtrSrc->GetTemplateID(), iSrc, iDst, m_pIvtrSrc->GetCount());
						else
							GetGameSession()->c2s_CmdExgIvtrItem(iSrc, iDst);
					}
					GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
				}
			}
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Inventory") && 
			m_pObjDst && strstr(m_pObjDst->GetName(), "Rdo_Page") )
		{
			int iPage = atoi(m_pObjDst->GetName() + strlen("Rdo_Page")) - 1;
			CECInventory* pPack = GetHostPlayer()->GetPack();
			int nSize = pPack->GetSize();
			if((nSize - 1) / 48 >= iPage /*&& iPage != 0*/)
			{
				int iDst = pPack->SearchEmpty(iPage * 48, (iPage + 1) * 48 - 1);
				if(iDst != -1)
				{
					CECScriptMan * pScriptMan = g_pGame->GetGameRun()->GetUIManager()->GetScriptMan();
					if( pScriptMan )
					{
						pScriptMan->GetContext()->GetCheckState()->SetInventoryItemMoved(true);
					}
					GetGameSession()->c2s_CmdExgIvtrItem(iSrc, iDst);
					GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
				}
			}

		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Trade")
			&& m_pDlgDst->GetDlgItem("Btn_Lock")->IsShow() && m_pDlgDst->GetDlgItem("Btn_Lock")->IsEnabled() )
		{
			int idItem = m_pIvtrSrc->GetTemplateID();
			int nCount = m_pIvtrSrc->GetCount();
			iSrc = atoi(m_pObjSrc->GetName() + strlen("Item_")) + m_nSrcInv;
			
			if( m_pIvtrSrc->IsLocked() )
			{
				GetGameUIMan()->AddChatMessage(GetStringFromTable(7035), GP_CHAT_MISC);
			}
			else if( AUI_PRESS(VK_MENU) && m_pIvtrSrc->GetPileLimit() > 1 && m_pIvtrSrc->GetCount() > 1 )
			{
				GetGameUIMan()->InvokeNumberDialog(m_pObjSrc, m_pObjDst,
					CDlgInputNO::INPUTNO_TRADE_ADD, m_pIvtrSrc->GetCount());
			}
			else
			{
				GetGameSession()->trade_AddGoods(GetGameUIMan()->m_idTrade, idItem, iSrc, nCount, 0);
				m_pIvtrSrc->Freeze(true);
			}
			GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Character") &&
			m_pObjDst && strstr(m_pObjDst->GetName(), "Equip_") )
		{
			m_pIvtrDst = (CECIvtrItem *)m_pObjDst->GetDataPtr("ptr_CECIvtrItem");
			
			iDst = atoi(m_pObjDst->GetName() + strlen("Equip_"));
			
			if( m_pIvtrDst && m_pIvtrSrc->GetPileLimit() > 1 &&
				m_pIvtrSrc->GetTemplateID() == m_pIvtrDst->GetTemplateID() && m_pIvtrSrc->GetStateFlag() == m_pIvtrDst->GetStateFlag() )
			{
				GetGameSession()->c2s_CmdMoveItemToEquip(iSrc, iDst);
				GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
			}
			else if( m_pIvtrSrc->CanEquippedTo(iDst) && 
				m_pIvtrSrc->IsEquipment() && GetHostPlayer()->CanUseEquipment((CECIvtrEquip*)m_pIvtrSrc))
			{
				if( ((CECIvtrEquip*)m_pIvtrSrc)->IsTalismanMain() && 
					((CECIvtrTalismanMain*)m_pIvtrSrc)->GetEssence().ess.master_id == 0 )
				{
					PAUIDIALOG pMsgBox;
					GetGameUIMan()->MessageBox("Game_EquipBind1", GetStringFromTable(873), 
						MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
					pMsgBox->SetData(iSrc);
					pMsgBox->SetDataPtr((void*)iDst);
				}
				else if( m_pIvtrSrc->GetStateFlag() & ITEM_STATE_BINDAFTEREQUIP )
				{
					PAUIDIALOG pMsgBox;
					GetGameUIMan()->MessageBox("Game_EquipBind1", GetStringFromTable(811), 
						MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
					pMsgBox->SetData(iSrc);
					pMsgBox->SetDataPtr((void*)iDst);
				}
				else
				{
					GetGameSession()->c2s_CmdEquipItem(iSrc, iDst);
					GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
				}
			}
		}
		else if( strstr(m_pDlgDst->GetName(), "Win_QuickBar")
			&& m_pObjDst && strstr(m_pObjDst->GetName(), "Item_") )
		{
			int iSlot = atoi(m_pObjDst->GetName() + strlen("Item_"));
			CECShortcutSet *pSCS = CECGameUIMan::GetSCSByDlg(m_pDlgDst->GetName());
			
			pSCS->CreateItemShortcut(iSlot - 1, IVTRTYPE_PACK, iSrc, m_pIvtrSrc);
			GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Pet_Detail") )
		{
			CDlgPetDetailOperate *pDlg = (CDlgPetDetailOperate*)GetGameUIMan()->GetDialog("Win_Pet_Detail");
			pDlg->SetItem(m_pIvtrSrc, iSrc, (PAUIIMAGEPICTURE)m_pObjDst);
		}
		else if( (0 == stricmp(m_pDlgDst->GetName(), "Win_Pet_ChangeHoroscope")
			|| 0 == stricmp(m_pDlgDst->GetName(), "Win_Pet_ChangeImage")
			|| 0 == stricmp(m_pDlgDst->GetName(), "Win_Pet_ChangeLife")
			|| 0 == stricmp(m_pDlgDst->GetName(), "Win_Pet_ChangeBone")
			|| 0 == stricmp(m_pDlgDst->GetName(), "Win_Pet_ChangeFight")
			|| 0 == stricmp(m_pDlgDst->GetName(), "Win_Pet_ChangeNature")
			|| 0 == stricmp(m_pDlgDst->GetName(), "Win_Pet_ChangePrelife")
			|| 0 == stricmp(m_pDlgDst->GetName(), "Win_Pet_ChangeSkill")
			|| 0 == stricmp(m_pDlgDst->GetName(), "Win_Pet_ExpandSkill")
			|| 0 == stricmp(m_pDlgDst->GetName(), "Win_Pet_ChangeBasicType")
			|| 0 == stricmp(m_pDlgDst->GetName(), "Win_Pet_Tame")
			|| 0 == stricmp(m_pDlgDst->GetName(), "Win_Pet_Release")
			|| 0 == stricmp(m_pDlgDst->GetName(), "Win_Pet_EquipEnhanse"))
			&& m_pObjDst && m_pObjDst->GetType()==AUIOBJECT_TYPE_IMAGEPICTURE )
		{
			if( m_pIvtrSrc->IsLocked() )
			{
				GetGameUIMan()->AddChatMessage(GetStringFromTable(7035), GP_CHAT_MISC);
			}
			else
			{
				CDlgItemSvrBase *pDlg = (CDlgItemSvrBase*)m_pDlgDst;
				pDlg->SetItem(m_pIvtrSrc, iSrc, (PAUIIMAGEPICTURE)m_pObjDst);
			}
		}
		else if( (0 == stricmp(m_pDlgDst->GetName(), "Win_PetCombine")
			|| 0 == stricmp(m_pDlgDst->GetName(), "Win_PetChangeName"))
			&& m_pObjDst && strstr(m_pObjDst->GetName(), "Item_") )
		{
			CDlgPetService *pDlg = (CDlgPetService*)m_pDlgDst;
			pDlg->DragDropItem(iSrc, m_pObjDst);
		}
		else if( stricmp(m_pDlgDst->GetName(), "Win_HorseTame") == 0
			&& m_pObjDst && strstr(m_pObjDst->GetName(), "Item_") )
		{
			if( m_pIvtrSrc->IsLocked() )
			{
				GetGameUIMan()->AddChatMessage(GetStringFromTable(7035), GP_CHAT_MISC);
			}
			else
			{
				GetGameUIMan()->m_pDlgHorseTame->DragDropItem(iSrc, m_pObjDst);
			}
		}
		else if( (0 == stricmp(m_pDlgDst->GetName(), "Win_TalismanClean")
			|| 0 == stricmp(m_pDlgDst->GetName(), "Win_TalismanCombine")
			|| 0 == stricmp(m_pDlgDst->GetName(), "Win_TalismanEnchanting")
			|| 0 == stricmp(m_pDlgDst->GetName(), "Win_TalismanLevelUp")
			|| 0 == stricmp(m_pDlgDst->GetName(), "Win_TalismanEnchanting")
			|| 0 == stricmp(m_pDlgDst->GetName(), "Win_TalismanEnchaseSkill")
			|| 0 == stricmp(m_pDlgDst->GetName(), "Win_TalismanLevelUp")
			|| 0 == stricmp(m_pDlgDst->GetName(), "Win_TalismanCombineSkill"))
			&& m_pObjDst && strstr(m_pObjDst->GetName(), "Item_") )
		{
			if( m_pIvtrSrc->IsLocked() )
			{
				GetGameUIMan()->AddChatMessage(GetStringFromTable(7035), GP_CHAT_MISC);
			}
			else
			{
				CDlgTalismanService *pDlg = (CDlgTalismanService*)m_pDlgDst;
				pDlg->DragDropItem(iSrc, m_pObjDst);
			}
		}
		else if(  0 == stricmp(m_pDlgDst->GetName(), "Win_TalismanRefine")
			|| 0 == stricmp(m_pDlgDst->GetName(), "Win_TalismanUpgrade")
			&& m_pObjDst && strstr(m_pObjDst->GetName(), "Item_") )
		{
			CDlgTalismanService *pDlg = (CDlgTalismanService*)m_pDlgDst;
			pDlg->DragDropItem(iSrc, m_pObjDst);
		}
		else if( stricmp(m_pDlgDst->GetName(), "Win_Talisman") == 0 && m_pObjDst && strstr(m_pObjDst->GetName(), "Item_") )
		{		
			m_pObjDst = GetGameUIMan()->m_pDlgTalisman->GetDlgItem(m_pObjDst->GetName());
			int iSlot = atoi(m_pObjDst->GetName() + strlen("Item_"));
			if( m_pIvtrSrc->GetClassID() == CECIvtrItem::ICID_MEDICINE )
			{
				CECIvtrMedicine *pMedicine = (CECIvtrMedicine*)m_pIvtrSrc;
				int iType = pMedicine->GetDBEssence()->type;
				if( iSlot == 1 && (iType == 0 || iType == 3 || iType == 2 || iType == 5) ||
					iSlot == 2 && (iType == 1 || iType == 4 || iType == 2 || iType == 5) ||
					iSlot == 4 && (iType == 6 || iType == 7) )
				{
					CECSCItem* pItemSC = new CECSCItem;
					if (pItemSC)
					{
						if (pItemSC->Init(IVTRTYPE_PACK, iSrc, m_pIvtrSrc))
							GetGameUIMan()->m_pDlgTalisman->SetTalismanItem(m_pObjDst, pItemSC);
						else
							delete pItemSC;
					}
				}
			}
			else if( m_pIvtrSrc->GetClassID() == CECIvtrItem::ICID_TALISMAN_ENERGYFOOD && iSlot == 3 )
			{
				CECSCItem* pItemSC = new CECSCItem;
				if (pItemSC)
				{
					if (pItemSC->Init(IVTRTYPE_PACK, iSrc, m_pIvtrSrc))
						GetGameUIMan()->m_pDlgTalisman->SetTalismanItem(m_pObjDst, pItemSC);
					else
						delete pItemSC;

					if(NewHelpGuide::CheckInventoryItemId(m_pIvtrSrc->GetTemplateID(),ESTEP_ITEM_EQUIPTALISMAN_4_3))
						NewHelpGuide::FinishGuide(GUIDE_HELP_EQUIPTALISMAN_4);
				}
			}
			else if(m_pIvtrSrc->GetClassID() == CECIvtrItem::ICID_AGENTCOMBINED)	// 2012-10-21 add 添加将合计拖拽到天人合一挂机栏 by zy
			{
				CECIvtrAgentCombinedItem *pAgentCombined = (CECIvtrAgentCombinedItem*)m_pIvtrSrc;
				int iType = pAgentCombined->GetDBEssence()->type;
				if( iSlot == 1 && (iType == 0 || iType == 3 ) 
					|| iSlot == 2 && (iType == 1 || iType == 4) 
					|| iSlot == 4 && (iType == 2 || iType == 5) 
					|| iSlot == 3)
				{
					CECSCItem* pItemSC = new CECSCItem;
					if (pItemSC)
					{
						if (pItemSC->Init(IVTRTYPE_PACK, iSrc, m_pIvtrSrc))
							GetGameUIMan()->m_pDlgTalisman->SetTalismanItem(m_pObjDst, pItemSC);
						else
							delete pItemSC;
					}
				}
			}
		}
		else if( stricmp(m_pDlgDst->GetName(), "Win_TalismanSmall") == 0 && m_pObjDst && strstr(m_pObjDst->GetName(), "Img_Item") )
		{
			int iSlot = atoi(m_pObjDst->GetName() + strlen("Img_Item"));
			
			CECSCItem* pItemSC = new CECSCItem;
			if (pItemSC)
			{
				if (pItemSC->Init(IVTRTYPE_PACK, iSrc, m_pIvtrSrc))
				{
					CDlgTalisman *pDlgTalismanSmall = (CDlgTalisman*)m_pDlgSrc;
					pDlgTalismanSmall->SetTalismanItem(m_pObjDst, pItemSC);
				}
				else
					delete pItemSC;
			}
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Shop")
			&& m_pObjDst && strstr(m_pObjDst->GetName(), "Sell_") )
		{
			CDlgInventory *pInventory = (CDlgInventory*)GetGameUIMan()->GetDialog("Win_Inventory");
			pInventory->OnEventLButtonDblclk(0,0,m_pObjSrc);
		}
		else if( (0 == stricmp(m_pDlgDst->GetName(), "Win_Enchase")
			|| 0 == stricmp(m_pDlgDst->GetName(), "Win_Disenchase1")
			|| 0 == stricmp(m_pDlgDst->GetName(), "Win_Disenchase2"))
			&& m_pObjDst && strstr(m_pObjDst->GetName(), "Item_") )
		{
			if( m_pIvtrSrc->IsLocked() )
			{
				GetGameUIMan()->AddChatMessage(GetStringFromTable(7035), GP_CHAT_MISC);
			}
			else
			{
				CDlgInstall *pDlg = (CDlgInstall*)m_pDlgDst;
				pDlg->DragDropItem(iSrc, m_pObjDst);
			}
		}
		else if( (0 == stricmp(m_pDlgDst->GetName(), "Win_BodyChange_Enchanting"))
			|| 0 == stricmp(m_pDlgDst->GetName(), "Win_BodyChange_Recover")
			&& m_pObjDst && strstr(m_pObjDst->GetName(), "Item_") )
		{
			CDlgInventory *pInventory = (CDlgInventory*)GetGameUIMan()->GetDialog("Win_Inventory");
			pInventory->OnEventLButtonDblclk(0,0,m_pObjSrc);
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Split")	&&
			m_pObjDst && 0 == stricmp(m_pObjDst->GetName(), "Item_a") &&
			((PAUIPROGRESS)m_pDlgDst->GetDlgItem("Prgs_1"))->GetProgress() <= 0 )
		{
			m_pObjDst->SetData(iSrc);
			void *ptr;
			AString szType;
			m_pObjSrc->ForceGetDataPtr(ptr,szType);
			m_pObjDst->SetDataPtr(ptr,szType);
			GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
			
			af_GetFileTitle(m_pIvtrSrc->GetIconFile(), strFile);
			strFile.MakeLower();
			PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)m_pObjDst;
			pImage->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
				GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
			
			GetGameUIMan()->UnfreezeAllItems();
			m_pIvtrSrc->Freeze(true);
			
			bool bCan = true;
			DATA_TYPE DataType;
			int idSkillNeed = -1, nLevelNeed = 0;
			CECSkill *pSkill, *pSkillThis = NULL;
			elementdataman *pDataMan = g_pGame->GetElementDataMan();
			int i, uiID, nItem = -1, nTime = 0, idItem = 0, nNumItem = 0;
			ACHAR szText[40], szPrice[40] = _AL("0"), szNumber[40] = _AL("0");
			
			m_pDlgDst->GetDlgItem("Txt_SkillName")->SetText(_AL(""));
			m_pDlgDst->GetDlgItem("Txt_SkillLevel")->SetText(_AL(""));
			for( i = 0; i < (int)pDataMan->get_data_num(ID_SPACE_RECIPE); i++ )
			{
				uiID = pDataMan->get_data_id(ID_SPACE_RECIPE, i, DataType);
				if( DataType != DT_RECIPE_ESSENCE ) continue;
				
				const RECIPE_ESSENCE *pRecipe = (const RECIPE_ESSENCE *)
					pDataMan->get_data_ptr(uiID, ID_SPACE_RECIPE, DataType);
				if( (int)pRecipe->products[0].id_to_make != m_pIvtrSrc->GetTemplateID() )
					continue;
				
				m_pDlgDst->GetDlgItem("Txt_SkillName")->SetText(_AL(""));
				a_sprintf(szText, _AL("%d"), pRecipe->level);
				m_pDlgDst->GetDlgItem("Txt_SkillLevel")->SetText(szText);
				idSkillNeed = 0;
				nLevelNeed = pRecipe->level;
				
				break;
			}
			
			for( i = 0; i < GetHostPlayer()->GetPassiveSkillNum(); i++ )
			{
				pSkill = GetHostPlayer()->GetPassiveSkillByIndex(i);
				if( pSkill->GetType() == CECSkill::TYPE_LIVE &&
					(!pSkillThis || idSkillNeed == pSkill->GetSkillID()) )
				{
					pSkillThis = pSkill;
				}
			}
			if( idSkillNeed > 0 )
			{
				if( pSkillThis )
				{
					if( pSkillThis->GetSkillID() == idSkillNeed )
						m_pDlgDst->GetDlgItem("Txt_SkillName")->SetColor(A3DCOLORRGB(255, 255, 255));
					else
					{
						bCan = false;
						m_pDlgDst->GetDlgItem("Txt_SkillName")->SetColor(A3DCOLORRGB(128, 128, 128));
					}
					
					if( pSkillThis->GetSkillLevel() >= nLevelNeed )
						m_pDlgDst->GetDlgItem("Txt_SkillLevel")->SetColor(A3DCOLORRGB(255, 255, 255));
					else
					{
						bCan = false;
						m_pDlgDst->GetDlgItem("Txt_SkillLevel")->SetColor(A3DCOLORRGB(128, 128, 128));
					}
					
					m_pDlgDst->GetDlgItem("Txt_CurrentSkillName")->SetText(
						g_pGame->GetSkillDesc()->GetWideString(pSkillThis->GetSkillID() * 10));
					a_sprintf(szText, _AL("%d"), pSkillThis->GetSkillLevel());
					m_pDlgDst->GetDlgItem("Txt_CurrentSkillLevel")->SetText(szText);
				}
				else
				{
					bCan = false;
					m_pDlgDst->GetDlgItem("Txt_CurrentSkillName")->SetText(_AL(""));
					m_pDlgDst->GetDlgItem("Txt_CurrentSkillLevel")->SetText(_AL(""));
					m_pDlgDst->GetDlgItem("Txt_SkillName")->SetColor(A3DCOLORRGB(128, 128, 128));
					m_pDlgDst->GetDlgItem("Txt_SkillLevel")->SetColor(A3DCOLORRGB(128, 128, 128));
				}
			}
			else
			{
				m_pDlgDst->GetDlgItem("Txt_CurrentSkillName")->SetText(_AL(""));
				m_pDlgDst->GetDlgItem("Txt_CurrentSkillLevel")->SetText(_AL(""));
				m_pDlgDst->GetDlgItem("Txt_SkillName")->SetColor(A3DCOLORRGB(128, 128, 128));
				m_pDlgDst->GetDlgItem("Txt_SkillLevel")->SetColor(A3DCOLORRGB(128, 128, 128));
			}
			
			CECIvtrEquip *pEquip = (CECIvtrEquip *)m_pIvtrSrc;
			const void *pData = pDataMan->get_data_ptr(
				m_pIvtrSrc->GetTemplateID(), ID_SPACE_ESSENCE, DataType);
				/*	if( DataType == DT_WEAPON_ESSENCE )
				{
				__int64 nCur = pEquip->GetCurEndurance();
				__int64 nMax = pEquip->GetMaxEndurance();
				WEAPON_ESSENCE *pEssence = (WEAPON_ESSENCE *)pData;
				
				  a_sprintf(szPrice, _AL("%d"), pEssence->decompose_price);
				  nNumItem = int(pEssence->element_num * nCur / nMax);
				  a_sprintf(szNumber, _AL("%d"), nNumItem);
				  idItem = pEssence->element_id;
				  }
				  else if( DataType == DT_ARMOR_ESSENCE )
				  {
				  __int64 nCur = pEquip->GetCurEndurance();
				  __int64 nMax = pEquip->GetMaxEndurance();
				  ARMOR_ESSENCE *pEssence = (ARMOR_ESSENCE *)pData;
				  
					a_sprintf(szPrice, _AL("%d"), pEssence->decompose_price);
					nNumItem = int(pEssence->element_num * nCur / nMax);
					a_sprintf(szNumber, _AL("%d"), nNumItem);
					idItem = pEssence->element_id;
					}
					else if( DataType == DT_DECORATION_ESSENCE )
					{
					__int64 nCur = pEquip->GetCurEndurance();
					__int64 nMax = pEquip->GetMaxEndurance();
					DECORATION_ESSENCE *pEssence = (DECORATION_ESSENCE *)pData;
					
					  a_sprintf(szPrice, _AL("%d"), pEssence->decompose_price);
					  nNumItem = int(pEssence->element_num * nCur / nMax);
					  a_sprintf(szNumber, _AL("%d"), nNumItem);
					  idItem = pEssence->element_id;
				}	*/
			if( a_atoi(szPrice) > GetHostPlayer()->GetMoneyAmount() )
				bCan = false;
			
			A2DSprite *pCover;
			if( idItem > 0 && nNumItem > 0 )
			{
			/*	const void *pData = pDataMan->get_data_ptr(
			idItem, ID_SPACE_ESSENCE, DataType);
			ELEMENT_ESSENCE *pElement = (ELEMENT_ESSENCE *)pData;
			
			  af_GetFileTitle(pElement->file_icon, strFile);
			  nItem = GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile];
			  pCover = GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY];
				m_pDlgDst->GetDlgItem("Btn_Start")->Enable(bCan);	*/
			}
			else
			{
				pCover = NULL;
				m_pDlgDst->GetDlgItem("Btn_Start")->Enable(false);
			}
			
			pImage = (PAUIIMAGEPICTURE)m_pDlgDst->GetDlgItem("Item_b");
			pImage->SetCover(pCover, nItem);
			
			m_pDlgDst->GetDlgItem("Txt_Gold")->SetText(szPrice);
			m_pDlgDst->GetDlgItem("Txt_no2")->SetText(szNumber);
		}
		else if(0 == stricmp(m_pDlgDst->GetName(), "Win_Dye_Combine")
					&& m_pObjDst && strstr(m_pObjDst->GetName(), "Item_0"))
		{

			DlgIndividualityColor* pDlgIndividuality = (DlgIndividualityColor*)GetGameUIMan()->GetDialog("Win_Dye_Combine");
			if(pDlgIndividuality)
			{
				pDlgIndividuality->SetNameToPosItem(m_pObjDst->GetName(),m_pIvtrSrc,iSrc);
			}
		}
		else if(0 == stricmp(m_pDlgDst->GetName(), "Win_Dye_FashDress")	
			&& m_pObjDst && strstr(m_pObjDst->GetName(), "Item_0"))
		{
			DlgIndividualityReveal* pDlgIndividuality = (DlgIndividualityReveal*)GetGameUIMan()->GetDialog("Win_Dye_FashDress");
			if(pDlgIndividuality)
			{
				if(m_pIvtrSrc->GetClassID() == CECIvtrItem::ICID_EQUIP && ((CECIvtrEquip*)m_pIvtrSrc)->IsFashion() && ((CECIvtrEquip*)m_pIvtrSrc)->GetDBEssence()->color == 1)
				{
					pDlgIndividuality->SetNameToPosItem(m_pObjDst->GetName(),m_pIvtrSrc,0,IVTRTYPE_PACK,iSrc);
				}
				else if(m_pIvtrSrc->GetClassID() == CECIvtrItem::ICID_INDIVIDUALITY_ITEM)
				{
					pDlgIndividuality->SetNameToPosItem(m_pObjDst->GetName(),m_pIvtrSrc,iSrc,IVTRTYPE_PACK,0);
				}
			}
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Storage")
			&& m_pObjDst && strstr(m_pObjDst->GetName(), "Item_") )
		{
			if(!m_pIvtrSrc->CanMoveToTrash())
			{
				GetGameUIMan()->AddChatMessage(GetStringFromTable(8461), GP_CHAT_MISC);
				return;
			}
			m_pIvtrDst = (CECIvtrItem *)m_pObjDst->GetDataPtr("ptr_CECIvtrItem");
			
			iDst = atoi(m_pObjDst->GetName() + strlen("Item_")) + m_nDstInv;
			if( m_pIvtrDst )
			{
				if( m_pIvtrSrc->GetPileLimit() > 1 &&
					m_pIvtrSrc->GetTemplateID() == m_pIvtrDst->GetTemplateID() && m_pIvtrSrc->GetStateFlag() == m_pIvtrDst->GetStateFlag() )
				{
					if( AUI_PRESS(VK_MENU) && m_pIvtrSrc->GetCount() > 1 )
					{
						GetGameUIMan()->InvokeNumberDialog(m_pObjSrc, m_pObjDst,
							CDlgInputNO::INPUTNO_STORAGE_PUT_ITEMS, m_pIvtrSrc->GetCount());
					}
					else if( m_pIvtrDst->GetPileLimit() != m_pIvtrDst->GetCount() )
					{
						GetGameSession()->c2s_CmdMoveIvtrToTrashBox(GetGameUIMan()->m_pDlgStorage->IsFactionStorage(), 
							iDst, iSrc, min(m_pIvtrSrc->GetCount(), m_pIvtrDst->GetPileLimit() - m_pIvtrDst->GetCount()));
					}
				}
				else
					GetGameSession()->c2s_CmdExgTrashBoxIvtrItem(GetGameUIMan()->m_pDlgStorage->IsFactionStorage(), iDst, iSrc);
			}
			else
			{
				if( AUI_PRESS(VK_MENU) && m_pIvtrSrc->GetCount() > 1 && m_pIvtrSrc->GetPileLimit() > 1 )
				{
					GetGameUIMan()->InvokeNumberDialog(m_pObjSrc, m_pObjDst,
						CDlgInputNO::INPUTNO_STORAGE_PUT_ITEMS, m_pIvtrSrc->GetCount());
				}
				else
					GetGameSession()->c2s_CmdExgTrashBoxIvtrItem(GetGameUIMan()->m_pDlgStorage->IsFactionStorage(), iDst, iSrc);
			}
			GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_InventoryPet")
			&& m_pObjDst && strstr(m_pObjDst->GetName(), "Item_") )
		{
			m_pIvtrDst = (CECIvtrItem *)m_pObjDst->GetDataPtr("ptr_CECIvtrItem");
			
			if (g_pGame->IsPocketableItem(m_pIvtrSrc->GetTemplateID()))
			{
				iDst = atoi(m_pObjDst->GetName() + strlen("Item_")) + m_nDstInv;
				if( m_pIvtrDst )
				{
					if( m_pIvtrSrc->GetPileLimit() > 1 &&
						m_pIvtrSrc->GetTemplateID() == m_pIvtrDst->GetTemplateID() && m_pIvtrSrc->GetStateFlag() == m_pIvtrDst->GetStateFlag() )
					{
						if( AUI_PRESS(VK_MENU) && m_pIvtrSrc->GetCount() > 1 )
						{
							GetGameUIMan()->InvokeNumberDialog(m_pObjSrc, m_pObjDst,
								CDlgInputNO::INPUTNO_PETINVO_PUT_ITEMS, m_pIvtrSrc->GetCount());
						}
						else if( m_pIvtrDst->GetPileLimit() != m_pIvtrDst->GetCount() )
						{
							GetGameSession()->c2s_CmdMoveIvtrToPocket( iDst, iSrc, min(m_pIvtrSrc->GetCount(), m_pIvtrDst->GetPileLimit() - m_pIvtrDst->GetCount()));
						}
					}
					else
						GetGameSession()->c2s_CmdExgPocketIvtrItem(iDst, iSrc);
				}
				else
				{
					if( AUI_PRESS(VK_MENU) && m_pIvtrSrc->GetCount() > 1 && m_pIvtrSrc->GetPileLimit() > 1 )
					{
						GetGameUIMan()->InvokeNumberDialog(m_pObjSrc, m_pObjDst,
							CDlgInputNO::INPUTNO_PETINVO_PUT_ITEMS, m_pIvtrSrc->GetCount());
					}
					else
						GetGameSession()->c2s_CmdExgPocketIvtrItem(iDst, iSrc);
				}
				GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
			}
			else
				GetGameUIMan()->AddChatMessage(GetStringFromTable(7325), GP_CHAT_MISC);
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Inventory_Costume") && 
			m_pObjDst && strstr(m_pObjDst->GetName(), "Img_Item"))
		{
			m_pIvtrDst = (CECIvtrItem *)m_pObjDst->GetDataPtr("ptr_CECIvtrItem");
			
			if( m_pIvtrSrc->GetClassID() == CECIvtrItem::ICID_EQUIP )
			{
				CECIvtrEquip *pEquip = (CECIvtrEquip *) m_pIvtrSrc;
				if (pEquip->GetDBEssence()->equip_mask & (EQUIP_MASK_FASHION_EYE  |
					EQUIP_MASK_FASHION_HEAD|
					EQUIP_MASK_FASHION_BODY|
					EQUIP_MASK_FASHION_FOOT))
				{
					int iDst = atoi(m_pObjDst->GetName() + strlen("Img_Item")) + m_nDstInv-1;
					if( iDst != -1 )
					{
						GetGameSession()->c2s_CmdExgIvtrFashionItem( iSrc, iDst);
						GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
					}
				}
				else
					GetGameUIMan()->AddChatMessage(GetStringFromTable(7330), GP_CHAT_MISC);
			}
			else
				GetGameUIMan()->AddChatMessage(GetStringFromTable(7330), GP_CHAT_MISC);
			
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Inventory_Vechile") && 
			m_pObjDst && strstr(m_pObjDst->GetName(), "Img_Item"))
		{
			m_pIvtrDst = (CECIvtrItem *)m_pObjDst->GetDataPtr("ptr_CECIvtrItem");
			int iDst = atoi(m_pObjDst->GetName() + strlen("Img_Item")) + m_nDstInv-1;
			if (iDst==-1)
			{
				return;
			}
			if(m_pIvtrSrc->GetClassID() == CECIvtrItem::ICID_TALISMAN_MAINPART)
			{
				CECIvtrTalismanMain *pTalisman = (CECIvtrTalismanMain *) m_pIvtrSrc;
				if (pTalisman->GetDBEssence()->is_aircraft)
				{
					GetGameSession()->c2s_CmdExgIvtrMountWingItem(iSrc, iDst);
					GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
				}
				else
					GetGameUIMan()->AddChatMessage(GetStringFromTable(7335), GP_CHAT_MISC);
			}
			else if (m_pIvtrSrc->GetClassID() == CECIvtrItem::ICID_VEHICLE)
			{
				GetGameSession()->c2s_CmdExgIvtrMountWingItem(iSrc, iDst);
				GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
			}
			else
				GetGameUIMan()->AddChatMessage(GetStringFromTable(7335), GP_CHAT_MISC);
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Inventory_Vechile") && 
			m_pObjDst && strstr(m_pObjDst->GetName(), "Img_Vechile01"))
		{
			if (m_pIvtrSrc->GetClassID() == CECIvtrItem::ICID_VEHICLE)
			{
				GetHostPlayer()->CheckRecordMount(IVTRTYPE_PACK,iSrc,m_pIvtrSrc->GetTemplateID());
			}
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Inventory_Vechile") && 
			m_pObjDst && strstr(m_pObjDst->GetName(), "Img_Vechile02"))
		{
			m_pIvtrDst = (CECIvtrItem *)m_pObjDst->GetDataPtr("ptr_CECIvtrItem");
			
			iDst = EQUIPIVTR_WING;
			if(m_pIvtrSrc->GetClassID() == CECIvtrItem::ICID_TALISMAN_MAINPART)
			{
				if( m_pIvtrDst && m_pIvtrSrc->GetPileLimit() > 1 &&
					m_pIvtrSrc->GetTemplateID() == m_pIvtrDst->GetTemplateID() && m_pIvtrSrc->GetStateFlag() == m_pIvtrDst->GetStateFlag() )
				{
					GetGameSession()->c2s_CmdMoveItemToEquip(iSrc, iDst);
					GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
				}
				else if( m_pIvtrSrc->CanEquippedTo(iDst) && 
					m_pIvtrSrc->IsEquipment() && GetHostPlayer()->CanUseEquipment((CECIvtrEquip*)m_pIvtrSrc))
				{
					if( ((CECIvtrEquip*)m_pIvtrSrc)->IsTalismanMain() && 
						((CECIvtrTalismanMain*)m_pIvtrSrc)->GetEssence().ess.master_id == 0 )
					{
						PAUIDIALOG pMsgBox;
						GetGameUIMan()->MessageBox("Game_EquipBind1", GetStringFromTable(873), 
							MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
						pMsgBox->SetData(iSrc);
						pMsgBox->SetDataPtr((void*)iDst);
					}
					else if( m_pIvtrSrc->GetStateFlag() & ITEM_STATE_BINDAFTEREQUIP )
					{
						PAUIDIALOG pMsgBox;
						GetGameUIMan()->MessageBox("Game_EquipBind1", GetStringFromTable(811), 
							MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
						pMsgBox->SetData(iSrc);
						pMsgBox->SetDataPtr((void*)iDst);
					}
					else
					{
						GetGameSession()->c2s_CmdEquipItem(iSrc, iDst);
						GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
					}
				}
			}
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_PShop1") &&
			m_pDlgDst->GetDlgItem("DEFAULT_Txt_Name")->IsEnabled() )
		{
			if( !m_pIvtrSrc->IsTradeable() )
				MSGBOX(652);
			else if( m_pIvtrSrc->IsLocked() )
			{
				GetGameUIMan()->AddChatMessage(GetStringFromTable(7035), GP_CHAT_MISC);
			}
			else
			{
				PAUIEDITBOX pMoney1, pMoney2, pMoney3, pAmount;
				PAUIDIALOG pShow = GetGameUIMan()->GetDialog("Win_InputNO2");
				
				pMoney1 = (PAUIEDITBOX)pShow->GetDlgItem("Txt_Money1");
				pMoney2 = (PAUIEDITBOX)pShow->GetDlgItem("Txt_Money2");
				pMoney3 = (PAUIEDITBOX)pShow->GetDlgItem("Txt_Money3");
				pMoney1->SetIsNumberOnly(true);
				pMoney2->SetIsNumberOnly(true);
				pMoney3->SetIsNumberOnly(true);
				int nMoney = m_pIvtrSrc->GetUnitPrice();
				ACHAR szMoney[20];
				a_sprintf(szMoney, _AL("%d"), nMoney % 100);
				pMoney3->SetText(szMoney);
				a_sprintf(szMoney, _AL("%d"), (nMoney % 10000) / 100);
				pMoney2->SetText(szMoney);
				a_sprintf(szMoney, _AL("%d"), nMoney / 10000);
				pMoney1->SetText(szMoney);
				pMoney1->SetData(iSrc);
				pShow->ChangeFocus(pMoney1);
				
				pAmount = (PAUIEDITBOX)pShow->GetDlgItem("Txt_Amount");
				pAmount->SetIsNumberOnly(true);
				pAmount->SetText(_AL("1"));
				pAmount->Enable(m_pIvtrSrc->GetPileLimit() > 0 ? true : false);
				
				if( m_pObjDst && strstr(m_pObjDst->GetName(), "BuyItem_") )
					pShow->SetData(CDlgInputNO::INPUTNO_BUY_ADD);
				else
					pShow->SetData(CDlgInputNO::INPUTNO_SELL_ADD);
				PAUILABEL pLabel = (PAUILABEL)pShow->GetDlgItem("Txt_name");
				pLabel->SetText(m_pIvtrSrc->GetName());
				PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)pShow->GetDlgItem("Item");
				af_GetFileTitle(m_pIvtrSrc->GetIconFile(), strFile);
				strFile.MakeLower();
				pImage->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
					GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
				pShow->SetDataPtr(m_pIvtrSrc,"ptr_CECIvtrItem");
				pShow->Show(true, true);
			}
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_PShop2") && m_pObjDst &&
			m_pObjDst->GetDataPtr("ptr_CECIvtrItem") && strstr(m_pObjDst->GetName(), "BuyItem_") )
		{
			m_pIvtrDst = (CECIvtrItem *)m_pObjDst->GetDataPtr("ptr_CECIvtrItem");
			if( m_pIvtrSrc->GetTemplateID() != m_pIvtrDst->GetTemplateID() ) return;
			
			if( m_pIvtrSrc->GetPileLimit() > 1 && m_pIvtrDst->GetCount() > 1 )
			{
				PAUIOBJECT pEdit = GetGameUIMan()->m_pDlgInputNO->GetDlgItem("DEFAULT_Txt_No.");
				
				pEdit->SetData(min(m_pIvtrSrc->GetCount(), m_pIvtrDst->GetCount()));
				pEdit->SetText(_AL("1"));
				
				iDst = atoi(m_pObjDst->GetName() + strlen("BuyItem_")) - 1;
				GetGameUIMan()->m_pDlgInputNO->SetData((DWORD)m_pObjDst);
				GetGameUIMan()->m_pDlgInputNO->SetDataPtr(m_pObjSrc,"ptr_AUIObject");
				GetGameUIMan()->m_pDlgInputNO->Show(true, true);
				GetGameUIMan()->m_pDlgInputNO->SetType(CDlgInputNO::INPUTNO_SELL_ADD);
			}
			else
			{
				CECDealInventory *pSell = GetHostPlayer()->GetSellPack();
				CECDealInventory *pBuy = GetHostPlayer()->GetEPBoothBuyPack();
				iDst = atoi(m_pObjDst->GetName() + strlen("BuyItem_")) - 1;
				int iOrigin = pBuy->GetItemInfo(iDst).iOrigin;
				int nIndex = pSell->AddBoothItem(m_pIvtrSrc,
					iOrigin, 1, m_pIvtrDst->GetUnitPrice());
				
				m_pIvtrSrc->Freeze(true);
				pBuy->GetItem(iDst)->Freeze(true);
				pSell->SetItemInfoFlag(nIndex, iSrc);
			}
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_MailWrite")
			&& m_pObjDst && 0 == stricmp(m_pObjDst->GetName(), "Img_AttachItem") )
		{
			if( !m_pIvtrSrc->IsTradeable() )
				MSGBOX(652);
			else if( m_pIvtrSrc->IsLocked() )
			{
				GetGameUIMan()->AddChatMessage(GetStringFromTable(7035), GP_CHAT_MISC);
			}
			else if( m_pIvtrSrc->GetCount() == 1 )
			{
				((CDlgMailWrite *)m_pDlgDst)->SetAttachItem(
					m_pIvtrSrc, 1, iSrc );
			}
			else
			{
				GetGameUIMan()->InvokeNumberDialog(m_pObjSrc, m_pObjDst,
					CDlgInputNO::INPUTNO_MOVE_MAILATTACH, m_pIvtrSrc->GetCount());
			}
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_ResetSkillPart")
			&& m_pObjDst && 0 == stricmp(m_pObjDst->GetName(), "Item_01") )
		{
			GetGameUIMan()->m_pDlgResetSkillPart->SetScroll(m_pIvtrSrc);
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_ResetSkillAll")
			&& m_pObjDst && 0 == stricmp(m_pObjDst->GetName(), "Item_01") )
		{
			GetGameUIMan()->m_pDlgResetSkillAll->SetScroll(m_pIvtrSrc);
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_ResetTalent")
			&& m_pObjDst && 0 == stricmp(m_pObjDst->GetName(), "Item_01") )
		{
			GetGameUIMan()->m_pDlgResetTalent->SetScroll(m_pIvtrSrc);
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Char_ChangeName")
			&& m_pObjDst && 0 == stricmp(m_pObjDst->GetName(), "Img_Item") )
		{
			CDlgReName *pDlg = (CDlgReName*)GetGameUIMan()->GetDialog("Win_Char_ChangeName");
			pDlg->SetItem(iSrc, m_pIvtrSrc);
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Guild_ChangeName")
			&& m_pObjDst && 0 == stricmp(m_pObjDst->GetName(), "Img_Item") )
		{
			CDlgReName *pDlg = (CDlgReName*)GetGameUIMan()->GetDialog("Win_Guild_ChangeName");
			pDlg->SetItem(iSrc, m_pIvtrSrc);
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_EquipBind") && m_pObjDst )
		{
			if( 0 == stricmp(m_pObjDst->GetName(), "Img_Item") )
			{
				if( m_pIvtrSrc->IsEquipment() && 
					(m_pIvtrSrc->GetStateFlag() == 0) )
					GetGameUIMan()->m_pDlgEquipBind->SetEquip(m_pIvtrSrc, iSrc);
			}
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_EquipDestroy") && m_pObjDst )
		{
			if( m_pIvtrSrc->IsLocked() )
			{
				GetGameUIMan()->AddChatMessage(GetStringFromTable(7035), GP_CHAT_MISC);
			}
			else if( 0 == stricmp(m_pObjDst->GetName(), "Img_Item") )
			{
				if( m_pIvtrSrc->GetStateFlag() & ITEM_STATE_BINDED )
					GetGameUIMan()->m_pDlgEquipDestroy->SetEquip(m_pIvtrSrc, iSrc);
			}
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_EquipUndestroy") && m_pObjDst )
		{
			if( 0 == stricmp(m_pObjDst->GetName(), "Img_Item") )
			{
				if( m_pIvtrSrc->GetClassID() == CECIvtrItem::ICID_DESTROYINGESSENCE )
					GetGameUIMan()->m_pDlgEquipUndestroy->SetEquip(m_pIvtrSrc, iSrc);
			}
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_PetEquip") && m_pObjDst )
		{
			if( strstr(m_pObjDst->GetName(), "Img_Equip") )
			{
				int iDst = atoi(m_pObjDst->GetName() + strlen("Img_Equip")) - 1;
				if( m_pIvtrSrc->GetClassID() == CECIvtrItem::ICID_PET_ARMOR && ((CECPetArmor*)m_pIvtrSrc)->GetDBEssence()->equip_location == iDst )
				{
					GetGameSession()->c2s_CmdPetEquip(iSrc, iDst);
				}
			}
		}
		else if( strstr(m_pDlgDst->GetName(), "Win_Pet_Skill") && m_pObjDst && stricmp(m_pObjDst->GetName(), "Img_AutoFood") == 0 ||
			strstr(m_pDlgDst->GetName(), "Win_PetAutoFood") && m_pObjDst && stricmp(m_pObjDst->GetName(), "Img_AutoFood") == 0 ||
			stricmp(m_pDlgDst->GetName(), "Win_PetPackage") == 0 && m_pObjDst && stricmp(m_pObjDst->GetName(), "Img_AutoFood") == 0 )
		{
			if( m_pIvtrSrc->GetClassID() == CECIvtrItem::ICID_PET_AUTO_FOOD )
			{
				CECPetAutoFood *pAutoFood = (CECPetAutoFood*)m_pIvtrSrc;
				CECSCItem* pItemSC = new CECSCItem;
				if (pItemSC)
				{
					if (pItemSC->Init(IVTRTYPE_PACK, iSrc, m_pIvtrSrc))
					{
						GetGameUIMan()->m_pDlgPetPackage->SetAutoFood(pItemSC);
					}
					else
						delete pItemSC;
				}
			}
		}
		else if( m_pDlgDst && (0 == stricmp(m_pDlgDst->GetName(), "Win_ItemLock")) && m_pIvtrSrc )
		{
			if (m_pIvtrSrc->IsLocked() || m_pIvtrSrc->GetExpireDate()>0)
			{
				GetGameUIMan()->AddChatMessage(GetStringFromTable(7036), GP_CHAT_MISC);
			}
			else if ( m_pIvtrSrc->GetPileLimit()==1 )
			{
				const SPECIAL_ID_CONFIG& cfg = g_pGame->GetGameRun()->GetSpecialIDConfig();
				if (m_pIvtrSrc->GetTemplateID() != cfg.id_destroying_matter)
				{
					CDlgItemLock *pDlgLock = (CDlgItemLock*)m_pDlgDst;
					pDlgLock->SetItemLock(m_pIvtrSrc, iSrc);
				}
			}
		}
		else if( m_pDlgDst && (0 == stricmp(m_pDlgDst->GetName(), "Win_ItemUnlock")) && m_pIvtrSrc )
		{
			if (!(m_pIvtrSrc->IsLocked()))
			{
				GetGameUIMan()->AddChatMessage(GetStringFromTable(7037), GP_CHAT_MISC);
			}
			else if (m_pIvtrSrc->GetPileLimit()==1 )
			{
				CDlgItemLock *pDlgLock = (CDlgItemLock*)m_pDlgDst;
				pDlgLock->SetItemUnLock(m_pIvtrSrc, iSrc);
			}
		}
		else if( m_pDlgDst && (0 == stricmp(m_pDlgDst->GetName(), "Win_Mend")) && m_pIvtrSrc && m_pObjDst )
		{
			const SPECIAL_ID_CONFIG& cfg = g_pGame->GetGameRun()->GetSpecialIDConfig();
			iDst = atoi(m_pObjDst->GetName() + strlen("Item_"));
			if (m_pIvtrSrc->GetTemplateID() == cfg.id_damaged_item && 1==iDst)
			{
				CDlgMend *pDlgMend = GetGameUIMan()->m_pDlgMend;
				pDlgMend->SetBrokeItem(m_pIvtrSrc, iSrc);
			}
			else if (m_pIvtrSrc->GetTemplateID() == cfg.id_repair_damaged_item && 2==iDst)
			{
				CDlgMend *pDlgMend = GetGameUIMan()->m_pDlgMend;
				pDlgMend->SetRepairItem(m_pIvtrSrc, iSrc);
			}
		}
		else if( m_pDlgDst && (0 == stricmp(m_pDlgDst->GetName(), "Win_MendMoney")) && m_pIvtrSrc )
		{
			const SPECIAL_ID_CONFIG& cfg = g_pGame->GetGameRun()->GetSpecialIDConfig();
			if (m_pIvtrSrc->GetTemplateID() == cfg.id_damaged_item && m_pObjDst && strstr(m_pObjDst->GetName(), "Item_1"))
			{
				CDlgMend *pDlgMend = (CDlgMend*)GetGameUIMan()->GetDialog("Win_MendMoney");
				pDlgMend->SetBrokeItem(m_pIvtrSrc, iSrc);
			}
		}
		else if (m_pDlgDst && (0 == stricmp(m_pDlgDst->GetName(), "Win_AttriUp_Enchase")) && m_pObjDst && m_pObjDst->GetType()==AUIOBJECT_TYPE_IMAGEPICTURE)
		{
			GetGameUIMan()->CommonMessage("Win_AttriUp_Enchase", "set_forge", iSrc, 0);
		}
		else if (m_pDlgDst && (0 == stricmp(m_pDlgDst->GetName(), "Win_AttriUp")) && m_pIvtrSrc && m_pObjDst && m_pObjDst->GetType()==AUIOBJECT_TYPE_IMAGEPICTURE)
		{
			if (0 == stricmp(m_pObjDst->GetName(), "Item_00"))
			{
				if (m_pIvtrSrc->GetClassID() == CECIvtrItem::ICID_PROP_ADD_MATERIAL_ITEM)
					GetGameUIMan()->CommonMessage("Win_AttriUp", "set_material", iSrc, 0);
				else if (m_pIvtrSrc->GetClassID() == CECIvtrItem::ICID_PROP_ADD_ITEM)
					GetGameUIMan()->CommonMessage("Win_AttriUp", "set_propadd_item", iSrc, 0);
			}
		}
		else if( GetGameUIMan()->GetDialog("Win_EquipOwn")->IsShow()  )
		{
			if( m_pIvtrSrc->GetClassID() == CECIvtrItem::ICID_EQUIP )
			{
				CECIvtrEquip *pEquip = (CECIvtrEquip *) m_pIvtrSrc;
				if (pEquip->GetDBEssence()->blood_sacrifice && 0==pEquip->GetNewEssence().master_id)
				{
					CDlgSoul *pDlgSoul = (CDlgSoul*)GetGameUIMan()->GetDialog("Win_EquipOwn");
					pDlgSoul->SetItemToOwn(m_pIvtrSrc, iSrc);
				}
			}
		}
		else if( GetGameUIMan()->GetDialog("Win_EquipEnchase")->IsShow()  )
		{
			if( m_pIvtrSrc->GetClassID() == CECIvtrItem::ICID_EQUIP )
			{
				CECIvtrEquip *pEquip = (CECIvtrEquip *) m_pIvtrSrc;
				if (pEquip->GetNewEssence().master_id)
				{
					CDlgSoul *pDlgSoul = (CDlgSoul*)GetGameUIMan()->GetDialog("Win_EquipEnchase");
					pDlgSoul->SetItemEnchaseEquip(m_pIvtrSrc, iSrc);
				}
			}
			else if ( m_pIvtrSrc->GetClassID() == CECIvtrItem::ICID_EQUIP_SOUL )
			{
				CDlgSoul *pDlgSoul = (CDlgSoul*)GetGameUIMan()->GetDialog("Win_EquipEnchase");
				pDlgSoul->SetItemEnchaseSoul(m_pIvtrSrc, iSrc);
			}
		}
		else if( GetGameUIMan()->GetDialog("Win_Spilt")->IsShow()  )
		{
			if( m_pIvtrSrc->GetClassID() == CECIvtrItem::ICID_EQUIP )
			{
				CECIvtrEquip *pEquip = (CECIvtrEquip *) m_pIvtrSrc;
				if (pEquip->GetNewEssence().master_id)
				{
					CDlgSoul *pDlgSoul = (CDlgSoul*)GetGameUIMan()->GetDialog("Win_Spilt");
					pDlgSoul->SetItemSplit(m_pIvtrSrc, iSrc);
				}
			}
		}
		else if( GetGameUIMan()->GetDialog("Win_EquipResume")->IsShow()  )
		{
			if( m_pIvtrSrc->GetClassID() == CECIvtrItem::ICID_EQUIP )
			{
				CECIvtrEquip *pEquip = (CECIvtrEquip *) m_pIvtrSrc;
				if (pEquip->GetNewEssence().master_id)
				{
					CDlgSoul *pDlgSoul = (CDlgSoul*)GetGameUIMan()->GetDialog("Win_EquipResume");
					pDlgSoul->SetItemResume(m_pIvtrSrc, iSrc);
				}
			}
		}
		else if( GetGameUIMan()->GetDialog("Win_EquipDepart")->IsShow()  )
		{
			if( m_pIvtrSrc->GetClassID() == CECIvtrItem::ICID_EQUIP )
			{
				CECIvtrEquip *pEquip = (CECIvtrEquip *) m_pIvtrSrc;
				if (pEquip->GetNewEssence().master_id && !(m_pIvtrSrc->IsLocked()))
				{
					CDlgSoul *pDlgSoul = (CDlgSoul*)GetGameUIMan()->GetDialog("Win_EquipDepart");
					pDlgSoul->SetItemDepart(m_pIvtrSrc, iSrc);
				}
			}
		}
		else if( GetGameUIMan()->GetDialog("Win_MarkAstro")->IsShow()  )
		{
			// 拖动放到合理的位置而非对应的坑
			CDlgMarkAstro* pDlg = (CDlgMarkAstro*)GetGameUIMan()->GetDialog("Win_MarkAstro");
			if( m_pIvtrSrc->GetClassID() == CECIvtrItem::ICID_TELEPORTATION )
			{
				CECIvtrTelePortation* pTelePort = (CECIvtrTelePortation*)m_pIvtrSrc;
				if (pTelePort->GetEssence().ess.time==0)
				{
					GetGameUIMan()->MessageBox(9467);
				}
				else
					pDlg->DragDropItem(iSrc,pDlg->GetDlgItem("Item_a"));
			}
			else if( m_pIvtrSrc->GetClassID() == CECIvtrItem::ICID_TELEPORTATION_STONE )
			{
				pDlg->DragDropItem(iSrc,pDlg->GetDlgItem("Item_d"));			
			}
		}
		else if( GetGameUIMan()->GetDialog("Win_EquipLvup")->IsShow() && m_pObjDst  )
		{
			if (0 == stricmp(m_pObjDst->GetName(), "Item_1"))
			{
				GetGameUIMan()->CommonMessage("Win_EquipLvup", "set_equip", iSrc, 0);
			}
			else if (0 == stricmp(m_pObjDst->GetName(), "Item_2"))
			{
				GetGameUIMan()->CommonMessage("Win_EquipLvup", "set_props", iSrc, 0);
			}
			else if (0 == stricmp(m_pObjDst->GetName(), "Item_3"))
			{
				GetGameUIMan()->CommonMessage("Win_EquipLvup", "set_helper", iSrc, 0);
			}
		}
		else if( GetGameUIMan()->GetDialog("Win_Equip_JewelUp")->IsShow() && m_pObjDst )
		{
			if (0 == stricmp(m_pObjDst->GetName(), "Item_01"))
			{
				GetGameUIMan()->CommonMessage("Win_Equip_JewelUp", "set_equip", iSrc, 0);
			}
			else if (0 == stricmp(m_pObjDst->GetName(), "Item_02"))
			{
				GetGameUIMan()->CommonMessage("Win_Equip_JewelUp", "set_props", iSrc, 0);
			}
		}
		else if(GetGameUIMan()->GetDialog("Win_Equip_ChangeName")->IsShow() && m_pObjDst)
		{
			if (0 == stricmp(m_pObjDst->GetName(), "Img_Equip"))
			{
				GetGameUIMan()->CommonMessage("Win_Equip_ChangeName", "set_equip", iSrc, 0);
			}
			else if (0 == stricmp(m_pObjDst->GetName(), "Img_Item"))
			{
				GetGameUIMan()->CommonMessage("Win_Equip_ChangeName", "set_props", iSrc, 0);
			}
		    
		}
		else if( GetGameUIMan()->GetDialog("Win_ShopConsign")->IsShow() && m_pObjDst  )
		{
			if (0 == stricmp(m_pObjDst->GetName(), "Img_Goods"))
			{
				if (m_pIvtrSrc->GetCount()==1)
					GetGameUIMan()->CommonMessage("Win_ShopConsign", "set_item", iSrc, 1);
				else
					GetGameUIMan()->InvokeNumberDialog(m_pObjSrc, m_pObjDst,
					CDlgInputNO::INPUTNO_COSIGN, m_pIvtrSrc->GetCount());
			}
		}
		else if( GetGameUIMan()->GetDialog("Win_Accessory_Upgrade")->IsShow() && m_pObjDst && m_pObjDst->GetType()==AUIOBJECT_TYPE_IMAGEPICTURE)
		{
			((CDlgItemSvrBase*)GetGameUIMan()->GetDialog("Win_Accessory_Upgrade"))->SetItem(m_pIvtrSrc, iSrc, (PAUIIMAGEPICTURE)m_pObjDst);
		}
		else if( GetGameUIMan()->GetDialog("Win_Accessory_Refine")->IsShow() && m_pObjDst && m_pObjDst->GetType()==AUIOBJECT_TYPE_IMAGEPICTURE)
		{
			((CDlgItemSvrBase*)GetGameUIMan()->GetDialog("Win_Accessory_Refine"))->SetItem(m_pIvtrSrc, iSrc, (PAUIIMAGEPICTURE)m_pObjDst);
		}
		else if( GetGameUIMan()->GetDialog("Win_Accessory_Spilt")->IsShow() && m_pObjDst && m_pObjDst->GetType()==AUIOBJECT_TYPE_IMAGEPICTURE)
		{
			((CDlgItemSvrBase*)GetGameUIMan()->GetDialog("Win_Accessory_Spilt"))->SetItem(m_pIvtrSrc, iSrc, (PAUIIMAGEPICTURE)m_pObjDst);
		}
		else if( GetGameUIMan()->GetDialog("Win_Accessory_Move")->IsShow() && m_pObjDst && m_pObjDst->GetType()==AUIOBJECT_TYPE_IMAGEPICTURE)
		{
			((CDlgItemSvrBase*)GetGameUIMan()->GetDialog("Win_Accessory_Move"))->SetItem(m_pIvtrSrc, iSrc, (PAUIIMAGEPICTURE)m_pObjDst);
		}
		else if( GetGameUIMan()->GetDialog("Win_Accessory_Drill")->IsShow() && m_pObjDst && m_pObjDst->GetType()==AUIOBJECT_TYPE_IMAGEPICTURE)
		{
			((CDlgItemSvrBase*)GetGameUIMan()->GetDialog("Win_Accessory_Drill"))->SetItem(m_pIvtrSrc, iSrc, (PAUIIMAGEPICTURE)m_pObjDst);
		}
		else if( GetGameUIMan()->GetDialog("Win_Accessory_ReDrill")->IsShow() && m_pObjDst && m_pObjDst->GetType()==AUIOBJECT_TYPE_IMAGEPICTURE)
		{
			((CDlgItemSvrBase*)GetGameUIMan()->GetDialog("Win_Accessory_ReDrill"))->SetItem(m_pIvtrSrc, iSrc, (PAUIIMAGEPICTURE)m_pObjDst);
		}
		else if( GetGameUIMan()->GetDialog("Win_Accessory_Custom")->IsShow() && m_pObjDst && m_pObjDst->GetType()==AUIOBJECT_TYPE_IMAGEPICTURE)
		{
			((CDlgItemSvrBase*)GetGameUIMan()->GetDialog("Win_Accessory_Custom"))->SetItem(m_pIvtrSrc, iSrc, (PAUIIMAGEPICTURE)m_pObjDst);
		}
		else if( GetGameUIMan()->GetDialog("Win_Accessory_Attach")->IsShow() && m_pObjDst && m_pObjDst->GetType()==AUIOBJECT_TYPE_IMAGEPICTURE)
		{
			((CDlgItemSvrBase*)GetGameUIMan()->GetDialog("Win_Accessory_Attach"))->SetItem(m_pIvtrSrc, iSrc, (PAUIIMAGEPICTURE)m_pObjDst);
		}
		else if( GetGameUIMan()->GetDialog("Win_Accessory_Detach")->IsShow() && m_pObjDst && m_pObjDst->GetType()==AUIOBJECT_TYPE_IMAGEPICTURE)
		{
			((CDlgItemSvrBase*)GetGameUIMan()->GetDialog("Win_Accessory_Detach"))->SetItem(m_pIvtrSrc, iSrc, (PAUIIMAGEPICTURE)m_pObjDst);
		}
		else if( GetGameUIMan()->GetDialog("Win_Equip_Yuanying_Upgrade")->IsShow() && m_pObjDst && m_pObjDst->GetType()==AUIOBJECT_TYPE_IMAGEPICTURE)
		{
			((CDlgItemSvrBase*)GetGameUIMan()->GetDialog("Win_Equip_Yuanying_Upgrade"))->SetItem(m_pIvtrSrc, iSrc, (PAUIIMAGEPICTURE)m_pObjDst);
		}
		else if( GetGameUIMan()->GetDialog("Win_Equip_Yuanying_Refresh")->IsShow() && m_pObjDst && m_pObjDst->GetType()==AUIOBJECT_TYPE_IMAGEPICTURE)
		{
			((CDlgItemSvrBase*)GetGameUIMan()->GetDialog("Win_Equip_Yuanying_Refresh"))->SetItem(m_pIvtrSrc, iSrc, (PAUIIMAGEPICTURE)m_pObjDst);
		}
		else if( GetGameUIMan()->GetDialog("Win_Equip_Yuanying_RuneOn")->IsShow() && m_pObjDst && m_pObjDst->GetType()==AUIOBJECT_TYPE_IMAGEPICTURE)
		{
			((CDlgRuneFillSlot*)GetGameUIMan()->GetDialog("Win_Equip_Yuanying_RuneOn"))->SetItem(m_pIvtrSrc, iSrc, (PAUIIMAGEPICTURE)m_pObjDst);
		}
		else if( GetGameUIMan()->GetDialog("Win_Equip_Yuanying_HoleErase")->IsShow() && m_pObjDst && m_pObjDst->GetType()==AUIOBJECT_TYPE_IMAGEPICTURE)
		{
			((CDlgItemSvrBase*)GetGameUIMan()->GetDialog("Win_Equip_Yuanying_HoleErase"))->SetItem(m_pIvtrSrc, iSrc, (PAUIIMAGEPICTURE)m_pObjDst);
		}
		else if( GetGameUIMan()->GetDialog("Win_Equip_Yuanying_Identify")->IsShow() && m_pObjDst && m_pObjDst->GetType()==AUIOBJECT_TYPE_IMAGEPICTURE)
		{
			((CDlgItemSvrBase*)GetGameUIMan()->GetDialog("Win_Equip_Yuanying_Identify"))->SetItem(m_pIvtrSrc, iSrc, (PAUIIMAGEPICTURE)m_pObjDst);
		}
		else if( GetGameUIMan()->GetDialog("Win_Equip_Yuanying_Default")->IsShow() && m_pObjDst && m_pObjDst->GetType()==AUIOBJECT_TYPE_IMAGEPICTURE)
		{
			((CDlgItemSvrBase*)GetGameUIMan()->GetDialog("Win_Equip_Yuanying_Default"))->SetItem(m_pIvtrSrc, iSrc, (PAUIIMAGEPICTURE)m_pObjDst);
		}
		else if( GetGameUIMan()->GetDialog("Win_Equip_Yuanying_HoleErase")->IsShow() && m_pObjDst && m_pObjDst->GetType()==AUIOBJECT_TYPE_IMAGEPICTURE)
		{
			((CDlgItemSvrBase*)GetGameUIMan()->GetDialog("Win_Equip_Yuanying_Default"))->SetItem(m_pIvtrSrc, iSrc, (PAUIIMAGEPICTURE)m_pObjDst);
		}
		else if( GetGameUIMan()->GetDialog("Win_Equip_Yuanying_RuneOn")->IsShow() && m_pObjDst && m_pObjDst->GetType()==AUIOBJECT_TYPE_IMAGEPICTURE)
		{
			((CDlgRuneFillSlot*)GetGameUIMan()->GetDialog("Win_Equip_Yuanying_RuneOn"))->SetItem(m_pIvtrSrc, iSrc, (PAUIIMAGEPICTURE)m_pObjDst);
		}
		else if( GetGameUIMan()->GetDialog("Win_PetLevelUp")->IsShow() && m_pObjDst && m_pObjDst->GetType()==AUIOBJECT_TYPE_IMAGEPICTURE)
		{
			((CDlgItemSvrBase*)GetGameUIMan()->GetDialog("Win_PetLevelUp"))->SetItem(m_pIvtrSrc, iSrc, (PAUIIMAGEPICTURE)m_pObjDst);
		}
		else if ( GetGameUIMan()->GetDialog("Win_Equip_Fengshen_Drill")->IsShow() && m_pObjDst && m_pObjDst->GetType()==AUIOBJECT_TYPE_IMAGEPICTURE)
		{
			if (0 == stricmp(m_pObjDst->GetName(), "Img_Item"))
			{
				GetGameUIMan()->CommonMessage("Win_Equip_Fengshen_Drill", "set_equip", iSrc, 0);
			}
			else if (0 == stricmp(m_pObjDst->GetName(), "Item_d"))
			{
				GetGameUIMan()->CommonMessage("Win_Equip_Fengshen_Drill", "set_props", iSrc, 0);
			}
		}
		else if ( GetGameUIMan()->GetDialog("Win_Equip_Fengshen_Attach")->IsShow() && m_pObjDst && m_pObjDst->GetType()==AUIOBJECT_TYPE_IMAGEPICTURE)
		{
			if (0 == stricmp(m_pObjDst->GetName(), "Img_Item"))
			{
				GetGameUIMan()->CommonMessage("Win_Equip_Fengshen_Attach", "set_equip", iSrc, 0);
			}
			else if (0 == stricmp(m_pObjDst->GetName(), "Item_d"))
			{
				GetGameUIMan()->CommonMessage("Win_Equip_Fengshen_Attach", "set_props", iSrc, 0);
			}
		}
		else if ( GetGameUIMan()->GetDialog("Win_Equip_Fengshen_Erase")->IsShow() && m_pObjDst && m_pObjDst->GetType()==AUIOBJECT_TYPE_IMAGEPICTURE)
		{
			GetGameUIMan()->CommonMessage("Win_Equip_Fengshen_Erase", "set_equip", iSrc, 0);
		}
		else if ( GetGameUIMan()->GetDialog("Win_Equip_Fengshen_Identify")->IsShow() && m_pObjDst && m_pObjDst->GetType()==AUIOBJECT_TYPE_IMAGEPICTURE)
		{
			GetGameUIMan()->CommonMessage("Win_Equip_Fengshen_Identify", "set_equip", iSrc, 0);
		}
		else if ( GetGameUIMan()->GetDialog("Win_Equip_Fengshen_Convert")->IsShow() && m_pObjDst && m_pObjDst->GetType()==AUIOBJECT_TYPE_IMAGEPICTURE)
		{
			GetGameUIMan()->CommonMessage("Win_Equip_Fengshen_Convert", "set_equip", iSrc, 0);
		}
		else if( strstr(m_pDlgDst->GetName(), "Win_DrugEnchanting") ) 	//2012-10-15 by add zy 药剂组合界面
		{
			if(m_pObjDst && strstr(m_pObjDst->GetName(), "Item_2"))
			{
				CDlgAgentCombined* pDlg = (CDlgAgentCombined*)GetGameUIMan()->GetDialog(m_pDlgDst->GetName());
				if(pDlg)
				{
					if(pDlg->IsItemIDEffective(m_pIvtrSrc->GetTemplateID()))
					{
						m_pIvtrSrc->Freeze(true);
						pDlg->IsAgentEffective(m_pIvtrSrc,iSrc);
						GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
					}
					else
					{
						MSGBOX(17000);
					}
				}
			}
		}
 		else if( strstr(m_pDlgDst->GetName(), "Win_Character_Attrimakeup") ) 	//2012-10-17 by add zy 弥补飞升界面
 		{
 			if(m_pObjDst && strstr(m_pObjDst->GetName(), "Img_Item"))
 			{
 				DlgRemedyMetempsychosis* pDlg = (DlgRemedyMetempsychosis*)GetGameUIMan()->GetDialog(m_pDlgDst->GetName());
 				if(pDlg)
				{	
					int iError = pDlg->CheckRemedyMetempsychosisItem(m_pIvtrSrc->GetTemplateID(),m_pIvtrSrc->GetCount());
					if(iError == 2)
					{
						MSGBOX(17005);
					}
					else if(iError == 3)
					{
						MSGBOX(17003);
					}
					else if(iError == 1)
					{
						m_pIvtrSrc->Freeze(true);
						pDlg->SetRemedyMetempsychosisItem(m_pIvtrSrc,iSrc);
						GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
					}
				}
 			}
 		}
		else if( strstr(m_pDlgDst->GetName(), "Win_Equip_Rune") )
		{
			if (m_pObjDst)
			{
				GetGameUIMan()->CommonMessage("Win_Equip_Rune", "set_install", iSrc, (long)m_pObjDst);
			}
		}
	}
}
void CDlgDragDrop::DragInventoryPet()
{
	int iSrc, iDst;
	if (strstr(m_pObjSrc->GetName(), "Item_") && m_pDlgDst && m_pObjDst && strstr(m_pObjDst->GetName(), "Item_"))
	{
		iSrc = atoi(m_pObjSrc->GetName() + strlen("Item_")) + m_nSrcInv;
		if( 0 == stricmp(m_pDlgDst->GetName(), "Win_InventoryPet") )
		{
			m_pIvtrDst = (CECIvtrItem *)m_pObjDst->GetDataPtr("ptr_CECIvtrItem");

			iDst = atoi(m_pObjDst->GetName() + strlen("Item_")) + m_nDstInv;
			if( iSrc != iDst )
			{
				if( m_pIvtrDst )
				{
					if( m_pIvtrSrc->GetPileLimit() > 1 &&
						m_pIvtrSrc->GetTemplateID() == m_pIvtrDst->GetTemplateID() && m_pIvtrSrc->GetStateFlag() == m_pIvtrDst->GetStateFlag() )
					{
						if( AUI_PRESS(VK_MENU) && m_pIvtrSrc->GetCount() > 1 )
						{
							GetGameUIMan()->InvokeNumberDialog(m_pObjSrc, m_pObjDst,
								CDlgInputNO::INPUTNO_PETINVO_MOVE_ITEMS, m_pIvtrSrc->GetCount());
						}
						else if( m_pIvtrSrc->GetPileLimit() - m_pIvtrDst->GetCount() > 0 )
						{
							GetGameSession()->c2s_CmdMovePocketItem(iSrc, iDst, min(m_pIvtrSrc->GetCount(), m_pIvtrSrc->GetPileLimit() - m_pIvtrDst->GetCount()));
						}
					}
					else
						GetGameSession()->c2s_CmdExgPocketItem(iSrc, iDst);
				}
				else
				{
					if( AUI_PRESS(VK_MENU) && m_pIvtrSrc->GetCount() > 1 && m_pIvtrSrc->GetPileLimit() > 1 )
					{
						GetGameUIMan()->InvokeNumberDialog(m_pObjSrc, m_pObjDst,
							CDlgInputNO::INPUTNO_PETINVO_MOVE_ITEMS, m_pIvtrSrc->GetCount());
					}
					else
						GetGameSession()->c2s_CmdExgPocketItem(iSrc, iDst);
				}
				GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
			}
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Inventory") &&
			GetGameUIMan()->m_pDlgInventory->GetShowItem() == CDlgInventory::INVENTORY_ITEM_NORMAL )
		{
			m_pIvtrDst = (CECIvtrItem *)m_pObjDst->GetDataPtr("ptr_CECIvtrItem");

			iDst = atoi(m_pObjDst->GetName() + strlen("Item_")) + m_nDstInv;
			if( m_pIvtrDst )
			{
				if( m_pIvtrSrc->GetPileLimit() > 1 &&
					m_pIvtrSrc->GetTemplateID() == m_pIvtrDst->GetTemplateID() && m_pIvtrSrc->GetStateFlag() == m_pIvtrDst->GetStateFlag() )
				{
					if( AUI_PRESS(VK_MENU) && m_pIvtrSrc->GetCount() > 1 )
					{
						GetGameUIMan()->InvokeNumberDialog(m_pObjSrc, m_pObjDst,
							CDlgInputNO::INPUTNO_PETINVO_GET_ITEMS, m_pIvtrSrc->GetCount());
					}
					else if( m_pIvtrDst->GetPileLimit() != m_pIvtrDst->GetCount() )
					{
						GetGameSession()->c2s_CmdMovePocketToIvtr(iSrc, iDst,
							min(m_pIvtrSrc->GetCount(), m_pIvtrDst->GetPileLimit() - m_pIvtrDst->GetCount()));
					}
				}
				else
					GetGameSession()->c2s_CmdExgPocketIvtrItem(iSrc, iDst);
			}
			else
			{
				if( AUI_PRESS(VK_MENU) && m_pIvtrSrc->GetCount() > 1 &&	m_pIvtrSrc->GetPileLimit() > 1 )
				{
					GetGameUIMan()->InvokeNumberDialog(m_pObjSrc, m_pObjDst,
						CDlgInputNO::INPUTNO_PETINVO_GET_ITEMS, m_pIvtrSrc->GetCount());
				}
				else
					GetGameSession()->c2s_CmdExgPocketIvtrItem(iSrc, iDst);
			}
			GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
		}
	}
}

void CDlgDragDrop::DragInventoryCostume()
{
	int iSrc, iDst;
	if(	strstr(m_pObjSrc->GetName(), "Img_Item") && m_pDlgDst && m_pObjDst )
	{
		iSrc = atoi(m_pObjSrc->GetName() + strlen("Img_Item")) + m_nSrcInv-1;
		m_pIvtrSrc = (CECIvtrItem *)m_pObjSrc->GetDataPtr("ptr_CECIvtrItem");
		m_pIvtrDst = (CECIvtrItem *)m_pObjDst->GetDataPtr("ptr_CECIvtrItem");
		if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Inventory") && strstr(m_pObjDst->GetName(), "Item_"))
		{
			if( NULL==m_pIvtrDst ||
				m_pIvtrDst->GetClassID() == CECIvtrItem::ICID_EQUIP && 
				((CECIvtrEquip *)m_pIvtrDst)->GetDBEssence()->equip_mask & (EQUIP_MASK_FASHION_EYE  |
																EQUIP_MASK_FASHION_HEAD|
																EQUIP_MASK_FASHION_BODY|
																EQUIP_MASK_FASHION_FOOT))
			{
				iDst = atoi(m_pObjDst->GetName() + strlen("Item_")) + m_nDstInv;
				GetGameSession()->c2s_CmdExgIvtrFashionItem(iDst, iSrc);
			}
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Inventory_Costume") && strstr(m_pObjDst->GetName(), "Img_Item") )
		{
//			if( NULL==m_pIvtrDst )
			{
				iDst = atoi(m_pObjDst->GetName() + strlen("Img_Item")) + m_nDstInv-1;
				GetGameSession()->c2s_CmdExgFashionItem(iDst, iSrc);
			}
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Inventory_CostumeQuick") )
		{
			AString strName = m_pObjDst->GetName();
			iDst = atoi(m_pObjDst->GetName() + (strName.GetLength()-1)) + m_nDstInv-1;
			CECHostPlayer::FASHION_HOTKEY fHotkey = GetHostPlayer()->GetFashionHotkey(iDst);
			CECIvtrEquip *pEquip = (CECIvtrEquip *) m_pIvtrSrc;
			if (strstr(m_pObjDst->GetName(),"Img_Hat") &&
				(EQUIP_MASK_FASHION_HEAD & pEquip->GetDBEssence()->equip_mask) )
			{
				GetHostPlayer()->SetFashionHotkey(iDst,m_pIvtrSrc->GetTemplateID(),fHotkey.id_body,fHotkey.id_shoe);
			}
			else if (strstr(m_pObjDst->GetName(),"Img_cloth") && 
				(EQUIP_MASK_FASHION_BODY & pEquip->GetDBEssence()->equip_mask) )
			{
				GetHostPlayer()->SetFashionHotkey(iDst,fHotkey.id_head,m_pIvtrSrc->GetTemplateID(),fHotkey.id_shoe);
			}
			else if ( strstr(m_pObjDst->GetName(),"Img_Shoe") &&
				(EQUIP_MASK_FASHION_FOOT & pEquip->GetDBEssence()->equip_mask)) 
			{
				GetHostPlayer()->SetFashionHotkey(iDst,fHotkey.id_head,fHotkey.id_body,m_pIvtrSrc->GetTemplateID());
			}
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Character") &&
			m_pObjDst && strstr(m_pObjDst->GetName(), "Equip_") )
		{
			iDst = atoi(m_pObjDst->GetName() + strlen("Equip_"));
			
			if( m_pIvtrSrc->CanEquippedTo(iDst) && 
				m_pIvtrSrc->IsEquipment() && GetHostPlayer()->CanUseEquipment((CECIvtrEquip*)m_pIvtrSrc))
			{
				if( ((CECIvtrEquip*)m_pIvtrSrc)->GetStateFlag() & ITEM_STATE_BINDAFTEREQUIP )
				{
					PAUIDIALOG pMsgBox;
					GetGameUIMan()->MessageBox("Game_EquipBind4", GetStringFromTable(811), 
						MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
					pMsgBox->SetData(iSrc);
					return;
				}
				else
				{
					GetHostPlayer()->UseItemInPack(IVTRTYPE_FASHION, iSrc);
				}
			}
		}
		else if(0 == stricmp(m_pDlgDst->GetName(), "Win_Dye_FashDress")	
			&& m_pObjDst && strstr(m_pObjDst->GetName(), "Item_0"))
		{
			DlgIndividualityReveal* pDlgIndividuality = (DlgIndividualityReveal*)GetGameUIMan()->GetDialog("Win_Dye_FashDress");
			if(pDlgIndividuality)
			{
				if(m_pIvtrSrc->GetClassID() == CECIvtrItem::ICID_EQUIP && ((CECIvtrEquip*)m_pIvtrSrc)->IsFashion()&& ((CECIvtrEquip*)m_pIvtrSrc)->GetDBEssence()->color == 1)
				{
					pDlgIndividuality->SetNameToPosItem(m_pObjDst->GetName(),m_pIvtrSrc,0,IVTRTYPE_FASHION,iSrc);
				}
				else if(m_pIvtrSrc->GetClassID() == CECIvtrItem::ICID_INDIVIDUALITY_ITEM)
				{
					pDlgIndividuality->SetNameToPosItem(m_pObjDst->GetName(),m_pIvtrSrc,iSrc,IVTRTYPE_PACK,0);
				}
			}
		}
	}
}

void CDlgDragDrop::DragInventoryWing()
{
	int iSrc, iDst;
	if(	strstr(m_pObjSrc->GetName(), "Img_Item") && m_pDlgDst && m_pObjDst )
	{
		iSrc = atoi(m_pObjSrc->GetName() + strlen("Img_Item")) + m_nSrcInv-1;
		m_pIvtrSrc = (CECIvtrItem *)m_pObjSrc->GetDataPtr("ptr_CECIvtrItem");
		m_pIvtrDst = (CECIvtrItem *)m_pObjDst->GetDataPtr("ptr_CECIvtrItem");
		if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Inventory") && 
			strstr(m_pObjDst->GetName(), "Item_") && 
			m_pDlgDst->GetCheckedRadioButton(2)==1)
		{
			if( NULL==m_pIvtrDst || m_pIvtrDst->GetClassID() == CECIvtrItem::ICID_VEHICLE ||
				m_pIvtrDst->GetClassID() == CECIvtrItem::ICID_TALISMAN_MAINPART && 
				((CECIvtrTalismanMain *)m_pIvtrDst)->GetDBEssence()->is_aircraft)
			{
				iDst = atoi(m_pObjDst->GetName() + strlen("Item_")) + m_nDstInv;
				GetGameSession()->c2s_CmdExgIvtrMountWingItem(iDst, iSrc);
			}
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Inventory_Vechile") && strstr(m_pObjDst->GetName(), "Img_Item") )
		{
			iDst = atoi(m_pObjDst->GetName() + strlen("Img_Item")) + m_nDstInv-1;
			GetGameSession()->c2s_CmdExgMountWingItem(iDst, iSrc);
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Inventory_Vechile") && 
			m_pObjDst && strstr(m_pObjDst->GetName(), "Img_Vechile01"))
		{
			if (m_pIvtrSrc->GetClassID() == CECIvtrItem::ICID_VEHICLE)
			{
				GetHostPlayer()->CheckRecordMount(IVTRTYPE_MOUNTWING,iSrc,m_pIvtrSrc->GetTemplateID());
			}
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Inventory_Vechile") && 
			m_pObjDst && strstr(m_pObjDst->GetName(), "Img_Vechile02"))
		{
			if(m_pIvtrSrc->GetClassID() == CECIvtrItem::ICID_TALISMAN_MAINPART)
			{
				if( m_pIvtrSrc->CanEquippedTo(EQUIPIVTR_WING) && 
					m_pIvtrSrc->GetClassID()==CECIvtrItem::ICID_TALISMAN_MAINPART && 
					((CECIvtrTalismanMain *)m_pIvtrSrc)->GetDBEssence()->is_aircraft)
				{
					if( ((CECIvtrTalismanMain*)m_pIvtrSrc)->GetEssence().ess.master_id==0)
					{
						PAUIDIALOG pMsgBox;
						GetGameUIMan()->MessageBox("Game_EquipBind_Wing", GetStringFromTable(811), 
							MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
						pMsgBox->SetData(iSrc);
						return;
					}
					else
					{
						GetHostPlayer()->UseItemInPack(IVTRTYPE_MOUNTWING, iSrc);
					}
				}
			}			
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Character") &&
			m_pObjDst && strstr(m_pObjDst->GetName(), "Equip_") )
		{
			iDst = atoi(m_pObjDst->GetName() + strlen("Equip_"));
			
			if( m_pIvtrSrc->CanEquippedTo(iDst) && 
				m_pIvtrSrc->GetClassID()==CECIvtrItem::ICID_TALISMAN_MAINPART && 
				((CECIvtrTalismanMain *)m_pIvtrSrc)->GetDBEssence()->is_aircraft)
			{
				if( ((CECIvtrTalismanMain*)m_pIvtrSrc)->GetEssence().ess.master_id==0)
				{
					PAUIDIALOG pMsgBox;
					GetGameUIMan()->MessageBox("Game_EquipBind_Wing", GetStringFromTable(811), 
						MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
					pMsgBox->SetData(iSrc);
					return;
				}
				else
				{
					GetHostPlayer()->UseItemInPack(IVTRTYPE_MOUNTWING, iSrc);
				}
			}
		}
	}
	else if(!m_pDlgDst)
	{
		if(0 == stricmp(m_pDlgSrc->GetName(), "Win_Inventory_Vechile") && 
			m_pDlgSrc && strstr(m_pObjSrc->GetName(), "Img_Vechile01"))
		{
			EC_LOCAL_SETTING setting = g_pGame->GetConfigs()->GetLocalSettings();
			setting.nRecordMountType = IVTRTYPE_MOUNTWING;
			setting.nRecordMountID = -1;
			setting.nRecordMountItemID = 0;
			g_pGame->GetConfigs()->SetLocalSettings(setting);

			CDlgInventoryRide *pDlg = (CDlgInventoryRide*)GetGameUIMan()->GetDialog("Win_Inventory_Vechile");
			if(pDlg)
			{
				pDlg->UpdataMountsOrFly();
			}
		}
		else if(0 == stricmp(m_pDlgSrc->GetName(), "Win_Inventory_Vechile") && 
			m_pDlgSrc && strstr(m_pObjSrc->GetName(), "Img_Vechile02"))
		{
			CDlgInventoryRide *pDlg = (CDlgInventoryRide*)GetGameUIMan()->GetDialog("Win_Inventory_Vechile");
			if(pDlg)
			{
				pDlg->SendMsgMoveItem(0);
			}
		}
	}
}

void CDlgDragDrop::DragCharacter()
{
	int iSrc, iDst;
	if( strstr(m_pObjSrc->GetName(), "Equip_") )
	{
		iSrc = atoi(m_pObjSrc->GetName() + strlen("Equip_"));
		if( !m_pDlgDst )		// Drop item.
		{
			if(!m_pIvtrSrc->IsDropable())
				MSGBOX(8462);
			else if (GetGame()->IsValuableItem(m_pIvtrSrc->GetTemplateID()))
			{
				PAUIDIALOG pMsgBox;
				GetGameUIMan()->MessageBox("Game_WorthyItemDrop_Equip",GetStringFromTable(8456), MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
				pMsgBox->SetData(iSrc);
			}
			else
				GetGameSession()->c2s_CmdDropEquipItem(iSrc);
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Inventory") && m_pObjDst )
		{
			m_pIvtrDst = (CECIvtrItem *)m_pObjDst->GetDataPtr("ptr_CECIvtrItem");
			if( !m_pIvtrDst || !m_pIvtrDst->IsFrozen() )
			{
				if( strstr(m_pObjDst->GetName(), "Item_") )
				{
					iDst = atoi(m_pObjDst->GetName() + strlen("Item_")) + m_nDstInv;
					if( m_pIvtrDst )
					{
						if( m_pIvtrDst->CanEquippedTo(iSrc) && m_pIvtrDst->IsEquipment() && 
							GetHostPlayer()->CanUseEquipment((CECIvtrEquip*)m_pIvtrDst) )
						{
							if( ((CECIvtrEquip*)m_pIvtrSrc)->IsTalismanMain() && 
								((CECIvtrTalismanMain*)m_pIvtrSrc)->GetEssence().ess.master_id == 0 )
							{
								PAUIDIALOG pMsgBox;
								GetGameUIMan()->MessageBox("Game_EquipBind1", GetStringFromTable(873), 
									MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
								pMsgBox->SetData(iDst);
								pMsgBox->SetDataPtr((void*)iSrc);
							}
							else if( m_pIvtrDst->GetStateFlag() & ITEM_STATE_BINDAFTEREQUIP )
							{
								PAUIDIALOG pMsgBox;
								GetGameUIMan()->MessageBox("Game_EquipBind1", GetStringFromTable(811), 
									MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
								pMsgBox->SetData(iDst);
								pMsgBox->SetDataPtr((void*)iSrc);
							}
							else
							{
								GetGameSession()->c2s_CmdEquipItem(iDst, iSrc);
								GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
							}
						}
					}
					else
					{
						GetGameSession()->c2s_CmdEquipItem(iDst, iSrc);
						GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
					}

				}
			}
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Inventory_Costume") && m_pObjDst && strstr(m_pObjDst->GetName(), "Img_Item") )
		{
			m_pIvtrDst = (CECIvtrItem *)m_pObjDst->GetDataPtr("ptr_CECIvtrItem");
			if( !m_pIvtrDst || !m_pIvtrDst->IsFrozen() )
			{
				iDst = atoi(m_pObjDst->GetName() + strlen("Img_Item"))-1 + m_nDstInv;
				if( m_pIvtrSrc->GetClassID() == CECIvtrItem::ICID_EQUIP )
				{
					CECIvtrEquip *pEquip = (CECIvtrEquip *) m_pIvtrSrc;
					if (pEquip->GetDBEssence()->equip_mask & (EQUIP_MASK_FASHION_EYE  |
						EQUIP_MASK_FASHION_HEAD|
						EQUIP_MASK_FASHION_BODY|
						EQUIP_MASK_FASHION_FOOT))
					{
						if( m_pIvtrDst )
						{
							if( m_pIvtrDst->CanEquippedTo(iSrc) && GetHostPlayer()->CanUseEquipment((CECIvtrEquip*)m_pIvtrDst) )
							{
								if( m_pIvtrDst->GetStateFlag() & ITEM_STATE_BINDAFTEREQUIP )
								{
									PAUIDIALOG pMsgBox;
									GetGameUIMan()->MessageBox("Game_EquipBind4", GetStringFromTable(811), 
										MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
									pMsgBox->SetData(iDst);
									pMsgBox->SetDataPtr((void*)iSrc);
								}
								else
								{
									GetGameSession()->c2s_CmdExgEquMountWingItem(iSrc, iDst);
									GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
								}
							}
						}
						else
						{
							GetGameSession()->c2s_CmdExgEquMountWingItem(iSrc, iDst);
							GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
						}
						
					}
					else
						GetGameUIMan()->AddChatMessage(GetStringFromTable(7330), GP_CHAT_MISC);
				}
				else
					GetGameUIMan()->AddChatMessage(GetStringFromTable(7330), GP_CHAT_MISC);
			}
		}
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Inventory_Vechile") && m_pObjDst && strstr(m_pObjDst->GetName(), "Img_Item") )
		{
			m_pIvtrDst = (CECIvtrItem *)m_pObjDst->GetDataPtr("ptr_CECIvtrItem");
			if( !m_pIvtrDst || !m_pIvtrDst->IsFrozen() )
			{
				iDst = atoi(m_pObjDst->GetName() + strlen("Img_Item"))-1 + m_nDstInv;
				bool bCanEquip = false;
				
				if(m_pIvtrSrc->GetClassID() == CECIvtrItem::ICID_TALISMAN_MAINPART)
				{
					CECIvtrTalismanMain *pTalisman = (CECIvtrTalismanMain *) m_pIvtrSrc;
					if (pTalisman->GetDBEssence()->is_aircraft)
					{
						if (pTalisman->GetEssence().ess.master_id==0)
						{
							PAUIDIALOG pMsgBox;
							GetGameUIMan()->MessageBox("Game_EquipBind_Wing", GetStringFromTable(873), 
								MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
							pMsgBox->SetData(iDst);
						}
						else
						{
							GetGameSession()->c2s_CmdExgEquMountWingItem(iSrc, iDst);
							GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
						}
					}
					else
						GetGameUIMan()->AddChatMessage(GetStringFromTable(7335), GP_CHAT_MISC);
				}
				else
					GetGameUIMan()->AddChatMessage(GetStringFromTable(7335), GP_CHAT_MISC);
			}
		}
		else if(  0 == stricmp(m_pDlgDst->GetName(), "Win_Character") && 
			m_pObjDst && strstr(m_pObjDst->GetName(), "Equip_") )
		{
			iDst = atoi(m_pObjDst->GetName() + strlen("Equip_"));
			if( iSrc != iDst ) GetGameSession()->c2s_CmdExgEquipItem(iSrc, iDst);
			GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
		}
		else if( strstr(m_pDlgDst->GetName(), "Win_QuickBar")
			&& m_pObjDst && strstr(m_pObjDst->GetName(), "Item_") )
		{
			int iSlot = atoi(m_pObjDst->GetName() + strlen("Item_"));
			CECShortcutSet *pSCS = CECGameUIMan::GetSCSByDlg(m_pDlgDst->GetName());
			
			pSCS->CreateItemShortcut(iSlot - 1, IVTRTYPE_EQUIPPACK, iSrc, m_pIvtrSrc);
			GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
		}
		else if( (0 == stricmp(m_pDlgDst->GetName(), "Win_Shop")
			&& m_pObjDst && strstr(m_pObjDst->GetName(), "Sell_"))
			|| (0 == stricmp(m_pDlgDst->GetName(), "Win_PShop2")
			&& m_pObjDst && strstr(m_pObjDst->GetName(), "BuyItem_")) )
		{
			MSGBOX(572);
		}
	}
}


void CDlgDragDrop::DragTarget()
{
	if( m_pDlgDst )		// Drop item.
	{
		if( strstr(m_pDlgDst->GetName(), "Win_QuickBar")
			&& m_pObjDst && strstr(m_pObjDst->GetName(), "Item_") )
		{
			int iSlot = atoi(m_pObjDst->GetName() + strlen("Item_"));
		
			CECShortcutSet *pSCS = CECGameUIMan::GetSCSByDlg(m_pDlgDst->GetName());
			CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
			if(!pHost)
				return;

			int idTarget = pHost->GetSelectedTarget();
			int nType = 0;
			if(ISPLAYERID(idTarget))
			{
				nType = 1;

				// select else player
				CECPlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(idTarget);
				if (!pPlayer)
					return;
				
				if(ISCLONEPLAYERID(idTarget))	// 怀光的分身，要记录真身的id
					idTarget = pPlayer->GetSummonerID();

				int level = pPlayer->GetBasicProps().iLevel;
				int prof = pPlayer->GetProfession();
				int feishengcnt = pPlayer->GetRebornCount();
				int cultivation = pPlayer->GetCultivation();

				pSCS->CreateTargetShortcut(iSlot-1, nType, idTarget, level, prof, feishengcnt, cultivation);
	
			}
			else if(ISNPCID(idTarget))
			{
				nType = 2;

				// select npc
				CECNPC* pMonster = g_pGame->GetGameRun()->GetWorld()->GetNPCMan()->GetNPC(idTarget);
				if(!pMonster)
					return;

				// 查询npc的模板id,获取等级信息
				int idTemplate = pMonster->GetTemplateID();

				//	Get data type from database
				DATA_TYPE DataType = DT_INVALID;
				MONSTER_ESSENCE* pDBEssence = (MONSTER_ESSENCE*)g_pGame->GetElementDataMan()->get_data_ptr(idTemplate, ID_SPACE_ESSENCE, DataType);
				if(!pDBEssence)
					return;

				int level = pDBEssence->level;
				pSCS->CreateTargetShortcut(iSlot-1, nType, idTemplate, level, 0, 0, 0);
			}

//			GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
		}
	}

}


void CDlgDragDrop::DragAction()
{
	if( m_pDlgDst && strstr(m_pDlgDst->GetName(), "Win_QuickBar")
		&& m_pObjDst && strstr(m_pObjDst->GetName(), "Item_") )
	{
		CECShortcut *pSC = (CECShortcut *)m_pObjSrc->GetDataPtr("ptr_CECShortcut");
		int iSlot = atoi(m_pObjDst->GetName() + strlen("Item_"));
		CECShortcutSet *pSCS = CECGameUIMan::GetSCSByDlg(m_pDlgDst->GetName());
		
		pSCS->CreateClonedShortcut(iSlot - 1, pSC);
	}
	else if( m_pDlgDst && stricmp(m_pDlgDst->GetName(), "Win_Talisman") == 0 
		&& m_pObjDst && strstr(m_pObjDst->GetName(), "Img_Skill") && atoi(m_pObjDst->GetName() + strlen("Img_Skill")) > 2)
	{
		CECSCCommand *pSC = (CECSCCommand *)m_pObjSrc->GetDataPtr("ptr_CECShortcut");
		if( pSC->GetCommandID() == CECSCCommand::CMD_NORMALATTACK )
		{
			CECShortcut *pNewSC = pSC->Clone();
			if (pNewSC)
				GetGameUIMan()->m_pDlgTalisman->SetTalismanItem(m_pObjDst, pNewSC);
		}
	}
}

void CDlgDragDrop::DragSkillComboEdit()
{
	if( m_pObjSrc && strstr(m_pObjSrc->GetName(), "Item_") &&
		m_pDlgDst && 0 == stricmp(m_pDlgDst->GetName(), "Win_SkillComboEdit") && m_pObjDst && strstr(m_pObjDst->GetName(), "Img_Icon") )
	{
		CDlgSkillComboEdit* pDlgSkillComboEdit = (CDlgSkillComboEdit*)GetGameUIMan()->GetDialog("Win_SkillComboEdit");
		pDlgSkillComboEdit->DragDropItem(pDlgSkillComboEdit, m_pObjSrc, m_pObjDst);
	}
}

void CDlgDragDrop::DragSkill()
{
	if( m_pDlgDst && strstr(m_pDlgDst->GetName(), "Win_QuickBar")
		&& m_pObjDst && strstr(m_pObjDst->GetName(), "Item_") )
	{
		CDlgQuickBar *pDlgQuickbar = (CDlgQuickBar*)m_pDlgDst;
		//			if (m_pObjDst->GetDataPtr("ptr_CECShortcut") == NULL || !pDlgQuickbar->IsQuickbarLocked())
		{
			if( strstr(m_pObjSrc->GetName(), "Item") )
			{
				int nCombo = m_pObjSrc->GetData();
				int iSlot = atoi(m_pObjDst->GetName() + strlen("Item_"));
				CECShortcutSet *pSCS = CECGameUIMan::GetSCSByDlg(m_pDlgDst->GetName());
				pSCS->CreateSkillGroupShortcut(iSlot - 1, nCombo - 1);
			}
			else if ( strstr(m_pObjSrc->GetName(), "Special") )
			{
				if (strstr(m_pObjSrc->GetName(), "Special_1"))
				{
					CECShortcut *pSC = GetGameRun()->GetGenCmdShortcuts()->GetShortcut(0);
					if (pSC)
					{
						int iSlot = atoi(m_pObjDst->GetName() + strlen("Item_"));
						CECShortcutSet *pSCS = CECGameUIMan::GetSCSByDlg(m_pDlgDst->GetName());
						
						pSCS->CreateClonedShortcut(iSlot - 1, pSC);
					}
				}
			}
			else
			{
				CECSkill *pSkill = (CECSkill *)m_pObjSrc->GetDataPtr("ptr_CECSkill");
				int iSlot = atoi(m_pObjDst->GetName() + strlen("Item_"));
				CECShortcutSet *pSCS = CECGameUIMan::GetSCSByDlg(m_pDlgDst->GetName());
				pSCS->CreateSkillShortcut(iSlot - 1, pSkill);
			}
		}
	}
	else if( m_pDlgDst && stricmp(m_pDlgDst->GetName(), "Win_TalismanSmall") == 0
		&& m_pObjDst && strstr(m_pObjDst->GetName(), "Img_Skill") && atoi(m_pObjDst->GetName() + strlen("Img_Skill")) > 2)
	{
		if( strstr(m_pObjSrc->GetName(), "Item") )
		{
			//				int nCombo = m_pObjSrc->GetData();
			//				CECSCSkillGrp *pGroupSC = new CECSCSkillGrp();
			//				if( pGroupSC )
			//				{
			//					if( pGroupSC->Init(nCombo - 1) )
			//						GetGameUIMan()->m_pDlgAuto->SetShortcut(m_pObjDst, pGroupSC);
			//					else
			//						delete pGroupSC;
			//				}
		}
		else
		{
			CECSkill *pSkill = (CECSkill *)m_pObjSrc->GetDataPtr("ptr_CECSkill");
			CECSCSkill *pSkillSC = new CECSCSkill;
			if( pSkillSC )
			{
				if( pSkillSC->Init(pSkill) )
					GetGameUIMan()->m_pDlgTalisman->SetTalismanItem(m_pObjDst, pSkillSC);
				else
					delete pSkillSC;
			}
		}
	}
	else if( m_pDlgDst && stricmp(m_pDlgDst->GetName(), "Win_SkillContinual") == 0
		&& m_pObjDst && strstr(m_pObjDst->GetName(), "Edt_Item_") )
		GetGameUIMan()->m_pDlgSkillEdit->DragDropItem(m_pDlgSrc, m_pObjSrc, m_pObjDst);
	
}

void CDlgDragDrop::DragTalent()
{
	DragSkill();
}

void CDlgDragDrop::DragTalismanSmall()
{
	//	m_pObjSrc = GetGameUIMan()->GetDialog("Win_TalismanSmall")->GetDlgItem(m_pObjSrc->GetName());
	if( m_pDlgDst && strstr(m_pDlgDst->GetName(), "Win_QuickBar")
		&& m_pObjDst && strstr(m_pObjDst->GetName(), "Item_") )
	{
		CECShortcut *pSC = (CECShortcut *)m_pObjSrc->GetDataPtr("ptr_CECShortcut");
		CECShortcutSet *pSCS = CECGameUIMan::GetSCSByDlg(m_pDlgDst->GetName());
		int iSlot = atoi(m_pObjDst->GetName() + strlen("Item_"));
		pSCS->CreateClonedShortcut(iSlot - 1, pSC);
	}
	else if( m_pDlgDst && stricmp(m_pDlgDst->GetName(), "Win_TalismanSmall") == 0 && m_pObjDst &&
		((strstr(m_pObjDst->GetName(), "Img_Skill") && strstr(m_pObjSrc->GetName(), "Img_Skill")) ||
		(strstr(m_pObjDst->GetName(), "Img_Item") && strstr(m_pObjSrc->GetName(), "Img_Item"))))
	{
		CECShortcut *pSC1 = (CECShortcut *)m_pObjSrc->GetDataPtr("ptr_CECShortcut");
		CECShortcut *pSC2 = (CECShortcut *)m_pObjDst->GetDataPtr("ptr_CECShortcut");
		
		if (pSC2 && pSC2==pSC1)
			return;
		
		m_pObjSrc->SetDataPtr(NULL, "ptr_CECShortcut");
		m_pObjDst->SetDataPtr(NULL, "ptr_CECShortcut");
		GetGameUIMan()->m_pDlgTalisman->SetTalismanItem(m_pObjSrc, pSC2);
		GetGameUIMan()->m_pDlgTalisman->SetTalismanItem(m_pObjDst, pSC1);
	}
	else if( !m_pDlgDst )
	{
		GetGameUIMan()->m_pDlgTalisman->SetTalismanItem(m_pObjSrc, NULL);
	}
}

void CDlgDragDrop::DragTalisman()
{
	if( m_pDlgDst && strstr(m_pDlgDst->GetName(), "Win_QuickBar")
		&& m_pObjDst && strstr(m_pObjDst->GetName(), "Item_") )
	{
		CECShortcut *pSC = (CECShortcut *)m_pObjSrc->GetDataPtr("ptr_CECShortcut");
		CECShortcutSet *pSCS = CECGameUIMan::GetSCSByDlg(m_pDlgDst->GetName());
		int iSlot = atoi(m_pObjDst->GetName() + strlen("Item_"));
		pSCS->CreateClonedShortcut(iSlot - 1, pSC);
	}
	else if( m_pDlgDst && stricmp(m_pDlgDst->GetName(), "Win_Talisman") == 0 && m_pObjDst &&
		strstr(m_pObjSrc->GetName(), "Img_Skill"))
	{
		int iSlot = atoi(m_pObjDst->GetName() + strlen("Img_Skill"));
		if( iSlot > 2 )
		{
			int iSlotSrc = atoi(m_pObjSrc->GetName() + strlen("Img_Skill"));
			CECShortcut *pSC1 = (CECShortcut *)m_pObjSrc->GetDataPtr("ptr_CECShortcut");
			CECShortcut *pSC2 = (CECShortcut *)m_pObjDst->GetDataPtr("ptr_CECShortcut");
			if (pSC2 && pSC2==pSC1)
				return;
			
			m_pObjSrc->SetDataPtr(NULL, "ptr_CECShortcut");
			m_pObjDst->SetDataPtr(NULL, "ptr_CECShortcut");
			GetGameUIMan()->m_pDlgTalisman->SetTalismanItem(m_pObjSrc, pSC2);
			GetGameUIMan()->m_pDlgTalisman->SetTalismanItem(m_pObjDst, pSC1);
		}
	}
	else if( !m_pDlgDst )
	{
		GetGameUIMan()->m_pDlgTalisman->SetTalismanItem(m_pObjSrc, NULL);
	}
}

void CDlgDragDrop::DragPet()
{
	m_pObjSrc = GetGameUIMan()->m_pDlgPetPackage->GetDlgItem(m_pObjSrc->GetName());
	if( m_pDlgDst && strstr(m_pDlgDst->GetName(), "Win_QuickBar")
		&& m_pObjDst && strstr(m_pObjDst->GetName(), "Item_") )
	{
		CECShortcut *pSC = (CECShortcut *)m_pObjSrc->GetDataPtr("ptr_CECShortcut");
		CECShortcutSet *pSCS = CECGameUIMan::GetSCSByDlg(m_pDlgDst->GetName());
		int iSlot = atoi(m_pObjDst->GetName() + strlen("Item_"));
		pSCS->CreateClonedShortcut(iSlot - 1, pSC);
	}
	else if( !m_pDlgDst )
	{
		GetGameUIMan()->m_pDlgPetPackage->SetAutoFood(NULL);
	}
}

void CDlgDragDrop::DragQuickBar()
{
	CDlgQuickBar *pCurBar = (CDlgQuickBar*)m_pDlgSrc;
	if( !m_pDlgDst )		// Remove it.
	{
		if( !pCurBar->IsQuickbarLocked() )
		{
			int iSlot = atoi(m_pObjSrc->GetName() + strlen("Item_"));
			CECShortcutSet *pSCS = CECGameUIMan::GetSCSByDlg(m_pDlgSrc->GetName());
			
			pSCS->SetShortcut(iSlot - 1, NULL);
		}
	}
	else if( strstr(m_pDlgDst->GetName(), "Win_QuickBar")
		&& m_pObjDst && strstr(m_pObjDst->GetName(), "Item_") )
	{
		if( !pCurBar->IsQuickbarLocked() )
		{
			int iSlotSrc = atoi(m_pObjSrc->GetName() + strlen("Item_"));
			int iSlotDst = atoi(m_pObjDst->GetName() + strlen("Item_"));
			CECShortcutSet *pSCSSrc = CECGameUIMan::GetSCSByDlg(m_pDlgSrc->GetName());
			CECShortcutSet *pSCSDst = CECGameUIMan::GetSCSByDlg(m_pDlgDst->GetName());
			CECShortcut *pSCSrc = pSCSSrc->GetShortcut(iSlotSrc - 1, true);
			CECShortcut *pSCDst = pSCSDst->GetShortcut(iSlotDst - 1, true);
			
			pSCSSrc->SetShortcut(iSlotSrc - 1, pSCDst);
			pSCSDst->SetShortcut(iSlotDst - 1, pSCSrc);
		}
	}
	else if( (0 == stricmp(m_pDlgDst->GetName(), "Win_Talisman") || 0 == stricmp(m_pDlgDst->GetName(), "Win_TalismanSmall")) && m_pObjDst )
	{
		int iSlotSrc = atoi(m_pObjSrc->GetName() + strlen("Item_"));
		CECShortcutSet *pSCSSrc = CECGameUIMan::GetSCSByDlg(m_pDlgSrc->GetName());
		CECShortcut *pSCSrc = pSCSSrc->GetShortcut(iSlotSrc - 1);
		if( strstr(m_pObjDst->GetName(), "Item_") )
		{
			int iSlot = atoi(m_pObjDst->GetName() + strlen("Item_"));
			if( pSCSrc->GetType() == CECShortcut::SCT_ITEM )
			{
				CECSCItem *pSCItem = (CECSCItem*)pSCSrc;
				if( pSCItem->GetInventory() == IVTRTYPE_PACK )
				{
					CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(pSCItem->GetIvtrSlot());
					if( pItem && pItem->GetClassID() == CECIvtrItem::ICID_MEDICINE )
					{
						CECIvtrMedicine *pMedicine = (CECIvtrMedicine*)pItem;
						int iType = pMedicine->GetDBEssence()->type;
						if( iSlot == 1 && (iType == 0 || iType == 3 || iType == 2 || iType == 5) ||
							iSlot == 2 && (iType == 1 || iType == 4 || iType == 2 || iType == 5) ||
							iSlot == 4 && (iType == 6 || iType == 7) )
						{
							CECSCItem* pItemSC = new CECSCItem;
							if (pItemSC)
							{
								if (pItemSC->Init(IVTRTYPE_PACK, pSCItem->GetIvtrSlot(), pItem))
									GetGameUIMan()->m_pDlgTalisman->SetTalismanItem(m_pObjDst, pItemSC);
								else
									delete pItemSC;
							}
						}
					}
					else if( pItem && pItem->GetClassID() == CECIvtrItem::ICID_TALISMAN_ENERGYFOOD && iSlot == 3 )
					{
						CECSCItem* pItemSC = new CECSCItem;
						if (pItemSC)
						{
							if (pItemSC->Init(IVTRTYPE_PACK, pSCItem->GetIvtrSlot(), pItem))
								GetGameUIMan()->m_pDlgTalisman->SetTalismanItem(m_pObjDst, pItemSC);
							else
								delete pItemSC;
						}
					}
				}
			}
		}
		else if( strstr(m_pObjDst->GetName(), "Img_Skill") )
		{
			if( pSCSrc->GetType() == CECShortcut::SCT_SKILL )
			{
				CECSCSkill *pSC = (CECSCSkill*)pSCSrc;
				CECSCSkill *pNewSC = new CECSCSkill;
				if( pNewSC )
				{
					if( pNewSC->Init(pSC->GetSkill()) )
						GetGameUIMan()->m_pDlgTalisman->SetTalismanItem(m_pObjDst, pNewSC);
					else
						delete pNewSC;
				}
			}
			else if( pSCSrc->GetType() == CECShortcut::SCT_COMMAND )
			{
				CECSCCommand *pSC = (CECSCCommand*)pSCSrc;
				if( pSC->GetCommandID() == CECSCCommand::CMD_NORMALATTACK )
				{
					CECSCCommand *pNewSC = new CECSCCommand(pSC->GetCommandID());
					if( pNewSC )
						GetGameUIMan()->m_pDlgTalisman->SetTalismanItem(m_pObjDst, pNewSC);
				}
			}
		}
		else if( strstr(m_pObjDst->GetName(), "Img_Item") )
		{
			if( pSCSrc->GetType() == CECShortcut::SCT_ITEM )
			{
				CECSCItem *pSC = (CECSCItem*)pSCSrc;
				CECSCItem *pNewSC = new CECSCItem(*pSC);
				if( pNewSC )
					GetGameUIMan()->m_pDlgTalisman->SetTalismanItem(m_pObjDst, pNewSC);
			}
		}
	}
}

void CDlgDragDrop::DragShop()
{
	if( strstr(m_pObjSrc->GetName(), "U_") &&
		m_pObjDst && strstr(m_pObjDst->GetName(), "Buy_") )
	{
		GetGameUIMan()->m_pDlgShop->OnEventLButtonDblclk(0, 0, m_pObjSrc);
		GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
	}
	else if( strstr(m_pObjSrc->GetName(), "Buy_") || strstr(m_pObjSrc->GetName(), "Sell_") )
	{
		if( !m_pDlgDst || (m_pObjDst && strstr(m_pObjDst->GetName(), "U_")
			&& 0 == stricmp(m_pDlgDst->GetName(), "Win_Shop")) || (m_pDlgDst
			&& 0 == stricmp(m_pDlgDst->GetName(), "Win_Inventory")) )
		{
			GetGameUIMan()->m_pDlgShop->OnEventLButtonDblclk(0, 0, m_pObjSrc);
			GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
		}
	}
}

void CDlgDragDrop::DragShopReputation()
{
	if( strstr(m_pObjSrc->GetName(), "U_") &&
		m_pObjDst && strstr(m_pObjDst->GetName(), "Buy_") )
	{
		GetGameUIMan()->m_pDlgShopReputation->OnEventLButtonDblclk(0, 0, m_pObjSrc);
		GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
	}
	else if( strstr(m_pObjSrc->GetName(), "Buy_") )
	{
		if( !m_pDlgDst || (m_pObjDst && strstr(m_pObjDst->GetName(), "U_")
			&& 0 == stricmp(m_pDlgDst->GetName(), "Win_Shop_Credit")) || (m_pDlgDst
			&& 0 == stricmp(m_pDlgDst->GetName(), "Win_Inventory")) )
		{
			GetGameUIMan()->m_pDlgShopReputation->OnEventLButtonDblclk(0, 0, m_pObjSrc);
			GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
		}
	}
}

void CDlgDragDrop::DragStorage()
{
	int iSrc, iDst;
	if( m_pDlgDst && m_pObjDst && strstr(m_pObjDst->GetName(), "Item_") )
	{
		iSrc = atoi(m_pObjSrc->GetName() + strlen("Item_")) + m_nSrcInv;
		if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Storage") )
		{
			m_pIvtrDst = (CECIvtrItem *)m_pObjDst->GetDataPtr("ptr_CECIvtrItem");

			iDst = atoi(m_pObjDst->GetName() + strlen("Item_")) + m_nDstInv;
			if( iSrc != iDst )
			{
				if( m_pIvtrDst )
				{
					if( m_pIvtrSrc->GetPileLimit() > 1 &&
						m_pIvtrSrc->GetTemplateID() == m_pIvtrDst->GetTemplateID() && m_pIvtrSrc->GetStateFlag() == m_pIvtrDst->GetStateFlag() )
					{
						if( AUI_PRESS(VK_MENU) && m_pIvtrSrc->GetCount() > 1 )
						{
							GetGameUIMan()->InvokeNumberDialog(m_pObjSrc, m_pObjDst,
								CDlgInputNO::INPUTNO_STORAGE_MOVE_ITEMS, m_pIvtrSrc->GetCount());
						}
						else if( m_pIvtrSrc->GetPileLimit() - m_pIvtrDst->GetCount() > 0 )
						{
							GetGameSession()->c2s_CmdMoveTrashBoxItem(GetGameUIMan()->m_pDlgStorage->IsFactionStorage(), 
								iSrc, iDst, min(m_pIvtrSrc->GetCount(), m_pIvtrSrc->GetPileLimit() - m_pIvtrDst->GetCount()));
						}
					}
					else
						GetGameSession()->c2s_CmdExgTrashBoxItem(GetGameUIMan()->m_pDlgStorage->IsFactionStorage(), iSrc, iDst);
				}
				else
				{
					if( AUI_PRESS(VK_MENU) && m_pIvtrSrc->GetCount() > 1 && m_pIvtrSrc->GetPileLimit() > 1 )
					{
						GetGameUIMan()->InvokeNumberDialog(m_pObjSrc, m_pObjDst,
							CDlgInputNO::INPUTNO_STORAGE_MOVE_ITEMS, m_pIvtrSrc->GetCount());
					}
					else
						GetGameSession()->c2s_CmdExgTrashBoxItem(GetGameUIMan()->m_pDlgStorage->IsFactionStorage(), iSrc, iDst);
				}
				GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
			}
		}
#ifdef LUA_UI
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Inventory")) 
#else
		else if( 0 == stricmp(m_pDlgDst->GetName(), "Win_Inventory") &&
			GetGameUIMan()->m_pDlgInventory->GetShowItem() == CDlgInventory::INVENTORY_ITEM_NORMAL )
#endif		
		{
			m_pIvtrDst = (CECIvtrItem *)m_pObjDst->GetDataPtr("ptr_CECIvtrItem");

			iDst = atoi(m_pObjDst->GetName() + strlen("Item_")) + m_nDstInv;
			if( m_pIvtrDst )
			{
				if( m_pIvtrSrc->GetPileLimit() > 1 &&
					m_pIvtrSrc->GetTemplateID() == m_pIvtrDst->GetTemplateID() && m_pIvtrSrc->GetStateFlag() == m_pIvtrDst->GetStateFlag() )
				{
					if( AUI_PRESS(VK_MENU) && m_pIvtrSrc->GetCount() > 1 )
					{
						GetGameUIMan()->InvokeNumberDialog(m_pObjSrc, m_pObjDst,
							CDlgInputNO::INPUTNO_STORAGE_GET_ITEMS, m_pIvtrSrc->GetCount());
					}
					else if( m_pIvtrDst->GetPileLimit() != m_pIvtrDst->GetCount() )
					{
						GetGameSession()->c2s_CmdMoveTrashBoxToIvtr(GetGameUIMan()->m_pDlgStorage->IsFactionStorage(), iSrc, iDst,
							min(m_pIvtrSrc->GetCount(), m_pIvtrDst->GetPileLimit() - m_pIvtrDst->GetCount()));
					}
				}
				else
					GetGameSession()->c2s_CmdExgTrashBoxIvtrItem(GetGameUIMan()->m_pDlgStorage->IsFactionStorage(), iSrc, iDst);
			}
			else
			{
				if( AUI_PRESS(VK_MENU) && m_pIvtrSrc->GetCount() > 1 &&	m_pIvtrSrc->GetPileLimit() > 1 )
				{
					GetGameUIMan()->InvokeNumberDialog(m_pObjSrc, m_pObjDst,
						CDlgInputNO::INPUTNO_STORAGE_GET_ITEMS, m_pIvtrSrc->GetCount());
				}
				else
					GetGameSession()->c2s_CmdExgTrashBoxIvtrItem(GetGameUIMan()->m_pDlgStorage->IsFactionStorage(), iSrc, iDst);
			}
			GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
		}
	}
	else if(m_pDlgDst && 0 == stricmp(m_pDlgDst->GetName(), "Win_Storage") && 
		m_pObjDst && strstr(m_pObjDst->GetName(), "Rdo_Page"))
	{
		iSrc = atoi(m_pObjSrc->GetName() + strlen("Item_")) + m_nSrcInv;
		int iPage = atoi(m_pObjDst->GetName() + strlen("Rdo_Page")) - 1;
		CECInventory* pPack = GetHostPlayer()->GetTrashBox();
		int nSize = pPack->GetSize();
		if((nSize - 1) / 48 >= iPage /*&& iPage != 0*/)
		{
			int iDst = pPack->SearchEmpty(iPage * 48, (iPage + 1) * 48 - 1);
			if(iDst != -1)
			{
				CECScriptMan * pScriptMan = g_pGame->GetGameRun()->GetUIManager()->GetScriptMan();
				if( pScriptMan )
				{
					pScriptMan->GetContext()->GetCheckState()->SetInventoryItemMoved(true);
				}
				GetGameSession()->c2s_CmdExgTrashBoxItem(GetGameUIMan()->m_pDlgStorage->IsFactionStorage(), iSrc, iDst);
				GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
			}
		}

	}
}

void CDlgDragDrop::DragSplit()
{
	if( !m_pDlgDst || 0 != stricmp(m_pDlgDst->GetName(), "Win_Split") ||
		!m_pObjDst || 0 != stricmp(m_pObjDst->GetName(), "Item_a")  )
	{
		PAUIIMAGEPICTURE pImage;
		
		pImage = (PAUIIMAGEPICTURE)m_pDlgSrc->GetDlgItem("Item_a");
		pImage->ClearCover();
		pImage->SetData(0);
		pImage->SetDataPtr(NULL);
		
		pImage = (PAUIIMAGEPICTURE)m_pDlgSrc->GetDlgItem("Item_b");
		pImage->ClearCover();
		pImage->SetData(0);
		pImage->SetDataPtr(NULL);
		
		m_pDlgSrc->GetDlgItem("Txt_no1")->SetText(_AL("0"));
		m_pDlgSrc->GetDlgItem("Txt_no2")->SetText(_AL("0"));
		m_pDlgSrc->GetDlgItem("Txt_Gold")->SetText(_AL("0"));
		m_pDlgSrc->GetDlgItem("Txt_SkillName")->SetText(_AL(""));
		m_pDlgSrc->GetDlgItem("Txt_SkillLevel")->SetText(_AL(""));
		
		GetGameUIMan()->UnfreezeAllItems();
		GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
	}
}

void CDlgDragDrop::DragIdentify()
{
	if( !m_pDlgDst || 0 != stricmp(m_pDlgDst->GetName(), "Win_Split") ||
		!m_pObjDst || 0 != stricmp(m_pObjDst->GetName(), "Item")  )
	{
		PAUIIMAGEPICTURE pImage;
		
		pImage = (PAUIIMAGEPICTURE)m_pDlgSrc->GetDlgItem("Item");
		pImage->ClearCover();
		pImage->SetData(0);
		pImage->SetDataPtr(NULL);
		
		m_pDlgSrc->GetDlgItem("Txt_Gold")->SetText(_AL("0"));
		m_pDlgSrc->GetDlgItem("Txt")->SetText(_AL(""));
		
		GetGameUIMan()->UnfreezeAllItems();
		GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
		m_pDlgSrc->GetDlgItem("Btn_Confirm")->Enable(false);
	}
}

void CDlgDragDrop::DragPShop2()
{
	if( strstr(m_pObjSrc->GetName(), "SellItem_") &&
		m_pObjDst && strstr(m_pObjDst->GetName(), "Buy_") )
	{
		GetGameUIMan()->m_pDlgBooth2->OnEventLButtonDblclk_SellItem(0, 0, m_pObjSrc);
		GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
	}
	else if( strstr(m_pObjSrc->GetName(), "Buy_") )
	{
		if( !m_pDlgDst || (0 == stricmp(m_pDlgDst->GetName(), "Win_PShop2")
			&& m_pObjDst && strstr(m_pObjDst->GetName(), "SellItem_")) )
		{
			GetGameUIMan()->m_pDlgBooth2->OnEventLButtonDblclk_Buy(0, 0, m_pObjSrc);
			GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
		}
	}
	else if( strstr(m_pObjSrc->GetName(), "Sell_") )
	{
		if( !m_pDlgDst || (0 == stricmp(m_pDlgDst->GetName(), "Win_PShop2")
			&& m_pObjDst && strstr(m_pObjDst->GetName(), "BuyItem_")) ||
			0 == stricmp(m_pDlgDst->GetName(), "Win_Inventory") )
		{
			GetGameUIMan()->m_pDlgBooth2->OnEventLButtonDblclk_Sell(0, 0, m_pObjSrc);
			GetGameUIMan()->PlayItemSound(m_pIvtrSrc, true);
		}
	}
}

void CDlgDragDrop::DragPetEquip()
{
	if( m_pDlgDst && 0 == stricmp(m_pDlgDst->GetName(), "Win_Inventory") && 
		m_pObjDst && strstr(m_pObjDst->GetName(), "Item_") )
	{
		int iDst = atoi(m_pObjDst->GetName() + strlen("Item_")) + m_nDstInv;
		int iSrc = atoi(m_pObjSrc->GetName() + strlen("Img_Equip")) - 1;
		GetGameSession()->c2s_CmdPetEquip(iDst, iSrc);
	}
}

void CDlgDragDrop::DragRune2013()
{
	if ( m_pDlgSrc && strstr(m_pDlgSrc->GetName(), "Win_Equip_Rune_Enchanting") &&
		 m_pDlgDst && strstr(m_pDlgDst->GetName(), "Win_Equip_Rune_Enchanting") && 
	     m_pObjSrc && m_pObjDst && m_pObjDst == m_pDlgSrc->GetDlgItem("Img_Item"))
	{
		GetGameUIMan()->CommonMessage("Win_Equip_Rune_Enchanting", "set_rune", m_pObjSrc->GetData(), 0);
	}
}

void CDlgDragDrop::SetCapture(bool bCapture, int x, int y)
{
	CDlgBase::SetCapture(bCapture, x, y);
	if( bCapture )
	{
		m_ptDragPos.x = m_ptLButtonDown.x - m_x;
		m_ptDragPos.y = m_ptLButtonDown.y - m_y;
	}
}
