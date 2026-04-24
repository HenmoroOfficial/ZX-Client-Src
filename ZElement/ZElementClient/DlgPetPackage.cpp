// File		: DlgPetPackage.cpp
// Creator	: Xiao Zhou
// Date		: 2008/4/3

#include "AFI.h"
#include "DlgPetPackage.h"
#include "DlgChat.h"
#include "DlgPetAction.h"
#include "DlgQuickBar.h"
#include "DlgPetDetail.h"
#include "EC_IvtrConsume.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_FixedMsg.h"
#include "EC_Shortcut.h"
#include "LuaEvent.h"
#include "EC_NewHelpGuide.h"
#include "AUI\\AUICTranslate.h"
#include "AUI\\AUIDef.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgPetPackage, CDlgBase)

AUI_ON_COMMAND("detail",	OnCommandDetail)
AUI_ON_COMMAND("pro",		OnCommandPro)
AUI_ON_COMMAND("status",	OnCommandStatus)
AUI_ON_COMMAND("withdraw",	OnCommandWithdraw)
AUI_ON_COMMAND("moveup",	OnCommandMoveUp)
AUI_ON_COMMAND("movedown",	OnCommandMoveDown)
AUI_ON_COMMAND("equip",		OnCommandEquip)
AUI_ON_COMMAND("summon",	OnCommandSummon)
AUI_ON_COMMAND("factory",	OnCommandFactory)
AUI_ON_COMMAND("raise",		OnCommandRaise)
AUI_ON_COMMAND("raiseall",	OnCommandRaiseAll)
AUI_ON_COMMAND("Btn_SetImg",	OnCommandSetApperance)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgPetPackage, CDlgBase)

AUI_ON_EVENT("Lst_Pet",			WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Img_AutoFood",	WM_LBUTTONDOWN,		OnEventLButtonDown_AutoFood)
AUI_ON_EVENT("Img_AutoFood",	WM_LBUTTONDBLCLK,	OnEventLButtonDblclk_AutoFood)
AUI_ON_EVENT("Img_AutoFood",	WM_RBUTTONUP,		OnEventLButtonDblclk_AutoFood)

AUI_END_EVENT_MAP()


CDlgPetPackage::CDlgPetPackage()
{
}

CDlgPetPackage::~CDlgPetPackage()
{
}

bool CDlgPetPackage::OnInitDialog()
{
	m_pImg_HeadPic = (PAUIIMAGEPICTURE)GetDlgItem("Img_HeadPic");
	m_pTxt_Name = (PAUILABEL)GetDlgItem("Txt_Name");
	m_pTxt_Lv = (PAUILABEL)GetDlgItem("Txt_Lv");
	m_pTxt_Hp = (PAUILABEL)GetDlgItem("Txt_Hp");
	m_pTxt_Mp = (PAUILABEL)GetDlgItem("Txt_Mp");
	m_pTxt_Horoscope = (PAUILABEL)GetDlgItem("Txt_Horoscope");
	m_pTxt_Element = (PAUILABEL)GetDlgItem("Txt_Element");
	m_pTxt_Race = (PAUILABEL)GetDlgItem("Txt_Race");
	m_pBtn_Status = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Status");
	m_pBtn_Pro = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Pro");
	m_pBtn_Detail = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Detail");
	m_pLst_Pet = (PAUILISTBOX)GetDlgItem("Lst_Pet");

	return true;
}

void CDlgPetPackage::OnShowDialog()
{
	if (stricmp(m_szName, "Win_PetPackage")==0)
	{
		CECSCItem *pSC = (CECSCItem*)GetDlgItem("Img_AutoFood")->GetDataPtr("ptr_CECShortcut");
		if( pSC )
			return;
		int i;
		int indexFoodUsed	= -1;
		int indexFoodUnUsed	= -1;
		CECInventory *pInventory = GetHostPlayer()->GetPack();
		CECIvtrItem *pItem;
		for (i=0; i<pInventory->GetSize(); ++i)
		{
			pItem = pInventory->GetItem(i);
			if( pItem && pItem->GetClassID() == CECIvtrItem::ICID_PET_AUTO_FOOD )
			{
				CECPetAutoFood *pFood = (CECPetAutoFood*)pItem;
				if( pFood->GetCurValue() < pFood->GetDBEssence()->food_value )
				{
					indexFoodUsed = i;
					break;
				}
				else if(indexFoodUnUsed==-1)
					indexFoodUnUsed = i;
			}		
		}
		
		if(indexFoodUsed!=-1)
		{
			SetAutoFoodFromIndex(indexFoodUsed);
		}
		else if(indexFoodUnUsed!=-1)
		{
			SetAutoFoodFromIndex(indexFoodUnUsed);
		}
	}
}

bool CDlgPetPackage::Release()
{
	if (m_szName == "Win_PetPackage")
	{
		CECShortcut *pSC = (CECShortcut*)GetDlgItem("Img_AutoFood")->GetDataPtr("ptr_CECShortcut");
		if (pSC)
		{
			delete pSC;
		}
	}
	return CDlgBase::Release();
}

void CDlgPetPackage::OnTick()
{
	CECPetBedge *pPet;
	if( m_pLst_Pet->GetCount() != GetHostPlayer()->GetPetPack()->GetSize() )
	{
		m_pLst_Pet->ResetContent();
		for(int i = 0; i < GetHostPlayer()->GetPetPack()->GetSize(); i++)
		{
			pPet = (CECPetBedge *)GetHostPlayer()->GetPetPack()->GetItem(i);
			if( pPet )
				m_pLst_Pet->AddString(pPet->GetName());
			else
				m_pLst_Pet->AddString(GetStringFromTable(780));
		}
		m_pLst_Pet->SetCurSel(0);
		m_pLst_Pet->SetData(0);
	}
	else
	{
		for(int i = 0; i < GetHostPlayer()->GetPetPack()->GetSize(); i++)
		{
			pPet = (CECPetBedge *)GetHostPlayer()->GetPetPack()->GetItem(i);
			if( pPet )
			{
				ACString strName = pPet->GetName();
				if( pPet->GetEssence().life - pPet->GetEssence().age <= 300 )
				{
					strName += GetStringFromTable(1001);
					LuaEvent::FirstPetWeak();
				}
				if( pPet->GetEssence().cur_hunger_point <= pPet->GetEssence().max_hunger_point / 4 )
				{
					strName += GetStringFromTable(1002);
					LuaEvent::FirstPetHungry();
				}
				if( pPet->GetEssence().level >= 60 )
					strName = _AL("(*)") + strName;
				m_pLst_Pet->SetText(i, strName);
				if( pPet->GetEssence().main_status == 0 )
					m_pLst_Pet->SetItemTextColor(i, A3DCOLORRGB(255, 0, 0));
				else if( pPet->GetEssence().main_status == 1 )
					m_pLst_Pet->SetItemTextColor(i, A3DCOLORRGB(255, 255, 255));
				else if( pPet->GetEssence().main_status == 2 )
					m_pLst_Pet->SetItemTextColor(i, A3DCOLORRGB(0, 0, 255));
				else if( pPet->GetEssence().main_status == 3 )
					m_pLst_Pet->SetItemTextColor(i, A3DCOLORRGB(255, 255, 0));
			}
			else
			{
				m_pLst_Pet->SetText(i, GetStringFromTable(780));
				m_pLst_Pet->SetItemTextColor(i, A3DCOLORRGB(128, 128, 128));
			}
		}
	}
	ACHAR szText[100];
	CECStringTab* pDescTab = GetGame()->GetItemDesc();
	pPet = NULL;
	int nSlot = m_pLst_Pet->GetCurSel();
	if( nSlot < GetHostPlayer()->GetPetPack()->GetSize() )
		pPet = (CECPetBedge *)GetHostPlayer()->GetPetPack()->GetItem(nSlot);
	if( pPet )
	{
		AString strFile;
		af_GetFileTitle(GetGame()->GetDataPath(pPet->GetDBEssence()->file_head_icon[pPet->GetEssence().face]), strFile);
		strFile.MakeLower();
		m_pImg_HeadPic->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_PET],
			GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_PET][strFile]);
		const ACHAR * szDesc = pPet->GetDesc();
		AUICTranslate trans;
		if( szDesc )
			m_pImg_HeadPic->SetHint(trans.Translate(szDesc));
		else
			m_pImg_HeadPic->SetHint(_AL(""));
		m_pTxt_Name->SetText(pPet->GetName());
		a_sprintf(szText, _AL("%d"), pPet->GetEssence().level);
		m_pTxt_Lv->SetText(szText);
		if( nSlot == GetHostPlayer()->GetCurPetIndex() )
			a_sprintf(szText, _AL("%d/%d"), pPet->GetEssence().cur_hp, GetHostPlayer()->GetCurPetMaxHp());
		else
			a_sprintf(szText, _AL("%d/%d"), pPet->GetEssence().cur_hp, pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_HP].cur_value);
		m_pTxt_Hp->SetText(szText);
		if( nSlot == GetHostPlayer()->GetCurPetIndex() )
			a_sprintf(szText, _AL("%d/%d"), pPet->GetEssence().cur_vigor, GetHostPlayer()->GetCurPetMaxVp());
		else
			a_sprintf(szText, _AL("%d/%d"), pPet->GetEssence().cur_vigor, pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_VIGOR].cur_value);
		m_pTxt_Mp->SetText(szText);
		m_pTxt_Horoscope->SetText(pDescTab->GetWideString(ITEMDESC_PET_HOROSCOPE0 + pPet->GetEssence().horoscope));
		m_pTxt_Element->SetText(pDescTab->GetWideString(ITEMDESC_PET_ELEMENT0 + pPet->GetEssence().element));
		m_pTxt_Race->SetText(pDescTab->GetWideString(ITEMDESC_PET_RACE0 + pPet->GetEssence().race));
		m_pBtn_Status->SetText(GetStringFromTable(4200 + pPet->GetEssence().main_status * 10 + pPet->GetEssence().sub_status));
		m_pBtn_Pro->SetText(GetStringFromTable(4300 + pPet->GetEssence().rank));
		m_pBtn_Status->Show(true);
		m_pBtn_Pro->Show(true);
	}
	else
	{
		m_pImg_HeadPic->ClearCover();
		m_pImg_HeadPic->SetHint(_AL(""));
		m_pTxt_Name->SetText(_AL(""));
		m_pTxt_Lv->SetText(_AL(""));
		m_pTxt_Hp->SetText(_AL(""));
		m_pTxt_Mp->SetText(_AL(""));
		m_pTxt_Horoscope->SetText(_AL(""));
		m_pTxt_Element->SetText(_AL(""));
		m_pTxt_Race->SetText(_AL(""));
		m_pBtn_Status->Show(false);
		m_pBtn_Pro->Show(false);
	}
	GetDlgItem("Btn_SetImg")->Show(pPet && pPet->GetEssence().reborn_cnt>0);
	if( pPet && pPet->GetEssence().identify )
		m_pBtn_Detail->Show(true);
	else
		m_pBtn_Detail->Show(false);

	a_sprintf(szText, _AL("%d/%d"), GetHostPlayer()->GetPetPack()->GetSize(), 32);
	GetDlgItem("Txt_Num")->SetText(szText);

	CECShortcut *pSC = (CECShortcut*)GetGameUIMan()->m_pDlgPetPackage->GetDlgItem("Img_AutoFood")->GetDataPtr("ptr_CECShortcut");
	CDlgQuickBar::UpdateShortcut(pSC, (PAUIIMAGEPICTURE)GetDlgItem("Img_AutoFood"));
	if( pSC )
	{
		AUICTranslate trans;
		const ACHAR *pszHint = pSC->GetDesc();
		if( pszHint )
			GetDlgItem("Img_AutoFood")->SetHint(trans.Translate(pszHint));
		else
			GetDlgItem("Img_AutoFood")->SetHint(_AL(""));
	}
	else
		GetDlgItem("Img_AutoFood")->SetHint(GetStringFromTable(984));
}

void CDlgPetPackage::OnCommandRaise(const char * szCommand)
{
	CECSCItem *pSC = (CECSCItem*)GetDlgItem("Img_AutoFood")->GetDataPtr("ptr_CECShortcut");
	int nSlot = m_pLst_Pet->GetCurSel();
	if( pSC && nSlot < GetHostPlayer()->GetPetPack()->GetSize() && GetHostPlayer()->GetPetPack()->GetItem(nSlot) )
	{
		struct auto_food_data
		{
			int index;
			int mask;
		};
		auto_food_data autofood;
		autofood.index = nSlot;
		autofood.mask = 0x4;
		GetGameSession()->c2s_CmdUseItemWithArg(IVTRTYPE_PACK, 1, (unsigned short)pSC->GetIvtrSlot(),
				pSC->GetItemTID(), &autofood, sizeof(auto_food_data));
		LuaEvent::FirstFeedPet();
	}
	else if( !pSC )
		GetGameUIMan()->MessageBox(359);
	else
		GetGameUIMan()->MessageBox(358);
}

void CDlgPetPackage::OnCommandRaiseAll(const char * szCommand)
{
	CECSCItem *pSC = (CECSCItem*)GetDlgItem("Img_AutoFood")->GetDataPtr("ptr_CECShortcut");
	bool bEmpty = true;
	for (int i=0; i<GetHostPlayer()->GetPetPack()->GetSize(); ++i)
	{
		if (GetHostPlayer()->GetPetPack()->GetItem(i))
		{
			bEmpty = false;
			break;
		}
	}
	if( pSC && !bEmpty )
	{
		struct auto_food_data
		{
			int index;
			int mask;
		};
		auto_food_data autofood;
		autofood.index = -1;
		autofood.mask = 0x4;
		GetGameSession()->c2s_CmdUseItemWithArg(IVTRTYPE_PACK, 1, (unsigned short)pSC->GetIvtrSlot(),
				pSC->GetItemTID(), &autofood, sizeof(auto_food_data));
	}
	else if( !pSC )
		GetGameUIMan()->MessageBox(359);
	else
		GetGameUIMan()->MessageBox(358);
}

void CDlgPetPackage::OnCommandSetApperance(const char * szCommand)
{
	GetGameUIMan()->CommonMessage("Win_PetLvUp_SetImg", "set_pet", m_pLst_Pet->GetCurSel(), 0);
}

void CDlgPetPackage::OnCommandDetail(const char * szCommand)
{
	CDlgPetDetail* pDlg = (CDlgPetDetail*)GetGameUIMan()->GetDialog("Win_PetDetail");
	pDlg->SetInfo(m_pLst_Pet->GetCurSel());
	pDlg->Show(!pDlg->IsShow());
	if(pDlg->IsShow() && GetHostPlayer()->GetCurPetIndex()==m_pLst_Pet->GetCurSel())
	{
		CECPetBedge *pPet = (CECPetBedge *)GetHostPlayer()->GetPetPack()->GetItem(m_pLst_Pet->GetCurSel());
		if(pPet->GetEssence().reborn_cnt>0)
			GetGameSession()->c2s_CmdGetPetProps();
	}
}

void CDlgPetPackage::OnCommandPro(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_PetProSetting");
	pDlg->SetData(m_pLst_Pet->GetCurSel());
	pDlg->Show(!pDlg->IsShow());
}

void CDlgPetPackage::OnCommandStatus(const char * szCommand)
{
	int nSlot = atoi(szCommand + strlen("status")) - 1;
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_PetStsSetting");
	pDlg->SetData(m_pLst_Pet->GetCurSel());
	pDlg->Show(!pDlg->IsShow());
}

void CDlgPetPackage::OnCommandWithdraw(const char * szCommand)
{
	int nSlot = m_pLst_Pet->GetCurSel();
	if( nSlot < GetHostPlayer()->GetPetPack()->GetSize() )
		GetHostPlayer()->UseItemInPack(IVTRTYPE_PET, nSlot);
}

void CDlgPetPackage::OnCommandMoveUp(const char * szCommand)
{
	int nSlot = m_pLst_Pet->GetCurSel();
	if( nSlot > 0 )
	{
		m_pLst_Pet->SetCurSel(nSlot - 1);
		GetGameSession()->c2s_CmdPetMoveBedgeItem(nSlot, nSlot - 1);
	}
}

void CDlgPetPackage::OnCommandMoveDown(const char * szCommand)
{
	int nSlot = m_pLst_Pet->GetCurSel();
	if( nSlot < GetHostPlayer()->GetPetPack()->GetSize() - 1 )
	{
		m_pLst_Pet->SetCurSel(nSlot + 1);
		GetGameSession()->c2s_CmdPetMoveBedgeItem(nSlot, nSlot + 1);
	}
}

void CDlgPetPackage::OnCommandSummon(const char * szCommand)
{
	int nCurPetIndex = GetHostPlayer()->GetBattlePetIndex();
	
	if ( GetHostPlayer()->GetCurPetId() )
		GetGameSession()->c2s_CmdPetRecall(nCurPetIndex);
	else
	{
		if( m_pLst_Pet->GetCurSel() >= m_pLst_Pet->GetCount() || 
			m_pLst_Pet->GetCurSel() < 0 ||
			!GetHostPlayer()->GetPetPack()->GetItem(m_pLst_Pet->GetCurSel()) )
		{
			bool bFind = false;
			for(int i = 0; i < GetHostPlayer()->GetPetPack()->GetSize(); i++)
				if( GetHostPlayer()->GetPetPack()->GetItem(i) )
				{
					m_pLst_Pet->SetCurSel(i);
					bFind = true;
					break;
				}
			if( !bFind )
				return;
		}
		
		if( nCurPetIndex != m_pLst_Pet->GetCurSel() )
		{
			CECIvtrItem *pItem;
			if( nCurPetIndex != -1 )
			{
				pItem = GetHostPlayer()->GetPetPack()->GetItem(nCurPetIndex);
				if( pItem )
					GetGameSession()->c2s_CmdPetSetStatus(nCurPetIndex, pItem->GetTemplateID(), 3, 0);
			}
			nCurPetIndex = m_pLst_Pet->GetCurSel();
			pItem = GetHostPlayer()->GetPetPack()->GetItem(nCurPetIndex);
			GetGameSession()->c2s_CmdPetSetStatus(nCurPetIndex, pItem->GetTemplateID(), 0, 0);
		}
		GetGameSession()->c2s_CmdPetSummon(nCurPetIndex);
	}


	{
		NewHelpGuide::FinishGuide(GUIDE_HELP_PET_11);
	}
}

void CDlgPetPackage::OnCommandEquip(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_PetEquip");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgPetPackage::OnCommandFactory(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_PetFactory");
	if( pDlg->IsShow() )
		pDlg->OnCommand("IDCANCEL");
	else if ( GetHostPlayer()->CanDoProduce() && !GetHostPlayer()->IsPlayerMoving() )
		pDlg->Show(true);
}

void CDlgPetPackage::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( m_pLst_Pet->GetCurSel() >= GetHostPlayer()->GetPetPack()->GetSize() )
		return;

	if( m_pLst_Pet->GetCurSel() != (int)m_pLst_Pet->GetData() )
	{
		if( GetGameUIMan()->GetDialog("Win_PetDetail")->IsShow() )
		{
			GetGameUIMan()->GetDialog("Win_PetDetail")->Show(false);
			if( m_pBtn_Detail->IsShow() )
				OnCommandDetail("");
		}

		if( GetGameUIMan()->GetDialog("Win_PetProSetting")->IsShow() )
		{
			GetGameUIMan()->GetDialog("Win_PetProSetting")->Show(false);
			if( m_pBtn_Detail->IsShow() )
				OnCommandPro("");
		}

		if( GetGameUIMan()->GetDialog("Win_PetStsSetting")->IsShow() )
		{
			GetGameUIMan()->GetDialog("Win_PetStsSetting")->Show(false);
			if( m_pBtn_Detail->IsShow() )
				OnCommandStatus("");
		}
		m_pLst_Pet->SetData(m_pLst_Pet->GetCurSel());
	}

	if( AUI_PRESS(VK_CONTROL) || AUI_PRESS(VK_SHIFT) )
	{
		CECIvtrItem *pItem = GetHostPlayer()->GetPetPack()->GetItem(m_pLst_Pet->GetCurSel());
		if( pItem && pItem->VerifyInfoMD5() )
		{
			CDlgChat* pChat = GetGameUIMan()->m_pDlgChat;
			PAUIEDITBOX pEdit = (PAUIEDITBOX)pChat->GetDlgItem("DEFAULT_Txt_Speech");
			ACString szName, strText;
			szName = pItem->GetName();
			strText.Format(GetStringFromTable(939), szName);

			/*
			if( pEdit->GetItemCount(enumEIIvtrlItem) < 1 )
			{
				ACString strColor = pItem->GetColor();
				if( strColor.GetLength() >= 7 && strColor[0] == '^' )
				{
					A3DCOLOR color = 0;
					for( int j = 0; j < 6; j++ )
					{
						ACHAR nValue = strColor[j + 1];
						if( nValue >= '0' && nValue <= '9' )
							nValue -= '0';
						else if( nValue >= 'a' && nValue <= 'z' )
							nValue = nValue - 'a' + 0xA;
						else if( nValue >= 'A' && nValue <= 'Z' )
							nValue = nValue - 'A' + 0xA;
						else
							nValue = 0;
						color = color * 0x10 + nValue;
					}
					color |= 0xFF000000;
					pEdit->AppendItem(enumEIIvtrlItem, color, strText, pItem->Serialize());
				}
				else
					pEdit->AppendItem(enumEIIvtrlItem, A3DCOLORRGB(255, 255, 255), strText, pItem->Serialize());
			}
			*/
			GetGameUIMan()->BringWindowToTop(pChat);
			pChat->ChangeFocus(pEdit);
		}
	}
}

void CDlgPetPackage::OnEventLButtonDown_AutoFood(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( !pObj->GetDataPtr("ptr_CECShortcut") )
		return;
	
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	POINT pt = 
	{
		GET_X_LPARAM(lParam) - p->X,
		GET_Y_LPARAM(lParam) - p->Y,
	};
	
	GetGameUIMan()->m_ptLButtonDown = pt;
	GetGameUIMan()->InvokeDragDrop(this, pObj, pt);
}

void CDlgPetPackage::OnEventLButtonDblclk_AutoFood(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	CECShortcut *pSC = (CECShortcut*)GetGameUIMan()->m_pDlgPetPackage->GetDlgItem("Img_AutoFood")->GetDataPtr("ptr_CECShortcut");
	if( pSC )
	{
		CECSCItem *pSCItem = (CECSCItem*)pSC;
		GetHostPlayer()->UseItemInPack(IVTRTYPE_PACK, pSCItem->GetIvtrSlot());
	}
}

void CDlgPetPackage::SetAutoFoodFromIndex(int index)
{
	CECInventory *pInventory = GetHostPlayer()->GetPack();
	CECPetAutoFood *pAutoFood = (CECPetAutoFood*)pInventory->GetItem(index);
	CECSCItem* pItemSC = new CECSCItem;
	if (pItemSC)
	{
		if (pItemSC->Init(IVTRTYPE_PACK, index, pAutoFood))
		{
			SetAutoFood(pItemSC);
		}
		else
			delete pItemSC;
	}
}

void CDlgPetPackage::SetAutoFood(CECShortcut *pSC)
{
	AUIObject* pObj = GetDlgItem("Img_AutoFood");
	CECShortcut *pOldSC = (CECShortcut*)pObj->GetDataPtr("ptr_CECShortcut");
	if( pOldSC )
		delete pOldSC;
	PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)pObj;
	CDlgQuickBar::UpdateShortcut(pSC, pImage);
}

void CDlgPetPackage::UpdateExchangedItemSC(int tidItem1, int iIvtr1, int iSlot1, int tidItem2, int iIvtr2, int iSlot2)
{
	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem("Img_AutoFood");
	
	CECSCItem *pSC = (CECSCItem*)pImg->GetDataPtr("ptr_CECShortcut");
	if( pSC && pSC->GetInventory() == iIvtr1 && pSC->GetItemTID() == tidItem1 && pSC->GetIvtrSlot() == iSlot1 )
		pSC->MoveItem(iIvtr2, iSlot2);
	else if( pSC && pSC->GetInventory() == iIvtr2 && pSC->GetItemTID() == tidItem2 && pSC->GetIvtrSlot() == iSlot2 )
		pSC->MoveItem(iIvtr1, iSlot1);
}

void CDlgPetPackage::UpdateMovedItemSC(int tidItem, int iSrcIvtr, int iSrcSlot, int iDstIvtr, int iDstSlot)
{
	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem("Img_AutoFood");
	
	CECSCItem *pSC = (CECSCItem*)pImg->GetDataPtr("ptr_CECShortcut");
	if( pSC && pSC->GetInventory() == iSrcIvtr && pSC->GetItemTID() == tidItem && pSC->GetIvtrSlot() == iSrcSlot )
		pSC->MoveItem(iDstIvtr, iDstSlot);
}

void CDlgPetPackage::UpdateRemovedItemSC(int tidItem, int iIvtr, int iSlot, int iSameItem)
{
	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem("Img_AutoFood");
	
	CECSCItem *pSC = (CECSCItem*)pImg->GetDataPtr("ptr_CECShortcut");
	if( pSC && pSC->GetInventory() == iIvtr && pSC->GetItemTID() == tidItem && pSC->GetIvtrSlot() == iSlot )
	{
		if( iSameItem >= 0 )
			pSC->MoveItem(iIvtr, iSameItem);
		else
			SetAutoFood(NULL);
	}
}
