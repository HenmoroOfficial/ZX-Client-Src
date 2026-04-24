// Filename	: DlgInventoryCostume.cpp
// Creator	: Fuchonggang
// Date		: 2009/04/08

#include "AFI.h"
#include "AUI\\AUIDef.h"
#include "DlgInventoryCostume.h"
#include "DlgItemLock.h"
#include "DlgSkill.h"
#include "DlgDragDrop.h"
#include "DlgCharacter.h"

#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_LoginPlayer.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "EC_Model.h"
#include "EC_Inventory.h"
#include "EC_GameSession.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrEquip.h"
#include "roleinfo"

#define new A_DEBUG_NEW

int					CDlgInventoryCostume::m_aEquips[SIZE_EQUIPIVTR];	//	Equipment item ID array
CECLoginPlayer*		CDlgInventoryCostume::m_pPlayer  = NULL;

const int a_ChangeTimes[4] = { 5,10,30,60};

AUI_BEGIN_COMMAND_MAP(CDlgInventoryCostume, CDlgBase)

AUI_ON_COMMAND("change",		OnCommand_ChangeQuickBar)
AUI_ON_COMMAND("resumeequip",	OnCommand_ResumeEquip)
AUI_ON_COMMAND("type*",			OnCommand_ChangeCostume)
AUI_ON_COMMAND("save",			OnCommand_SaveQuickBar)
AUI_ON_COMMAND("help",			OnCommand_FashionPocketHelp)
AUI_ON_COMMAND("order",			OnCommand_AutoChangeType)
AUI_ON_COMMAND("random",		OnCommand_AutoChangeType)
AUI_ON_COMMAND("auto",			OnCommand_AutoChange)
AUI_ON_COMMAND("time",			OnCommand_ChangeTime)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgInventoryCostume, CDlgBase)

AUI_ON_EVENT("*",					WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT(NULL,					WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT("Img_Hat*",			WM_LBUTTONDOWN,		OnEventLButtonDownQuickHat)
AUI_ON_EVENT("Img_cloth*",			WM_LBUTTONDOWN,		OnEventLButtonDownQuickCloth)
AUI_ON_EVENT("Img_Shoe*",			WM_LBUTTONDOWN,		OnEventLButtonDownQuickShoe)
AUI_ON_EVENT("Img_Item*",			WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Img_Item*",			WM_RBUTTONUP,		OnLButtonDbClkItem)
AUI_ON_EVENT("Img_Item*",			WM_LBUTTONDBLCLK,	OnLButtonDbClkItem)

AUI_END_EVENT_MAP()

CDlgInventoryCostume::CDlgInventoryCostume()
{
	m_nAngle = 0;
}

CDlgInventoryCostume::~CDlgInventoryCostume()
{
	if (m_szName == "Win_Inventory_Costume")
		A3DRELEASE(m_pPlayer);
}

bool CDlgInventoryCostume::OnInitDialog()
{
	int i;
	char szItem[20];

	if (m_szName == "Win_Inventory_Costume")
	{
		m_pScl_Item = (PAUISCROLL)GetDlgItem("Scl_Item");
		m_pImg_Char = (PAUIIMAGEPICTURE)GetDlgItem("Pic_Part");
		m_pBtn_LeftTurn = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_LeftTurn");
		m_pBtn_RightTurn = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_RightTurn");
		m_pComboTime = (PAUICOMBOBOX)GetDlgItem("Combo_Time");
		for (i=0; i<4; i++)
		{
			ACHAR szText[10];
			a_sprintf(szText, _AL("%d"), a_ChangeTimes[i]);
			m_pComboTime->AddString(szText);
		}
		for( i = 0; i < CECDLGINVCOSTUME_PACKMAX; i++ )
		{
			sprintf(szItem, "Img_Item%d", i+1);
			m_pImgItem[i] = (PAUIIMAGEPICTURE)GetDlgItem(szItem);
		}
		
		m_ChangeTimeCnt.SetPeriod(1000*60*5);
		m_ChangeTimeCnt.Reset();
		m_bRandom = false;
		m_bAutoChange = false;
		m_iCurIndex = -1;
		m_nFirstLine = 0;
		if (!m_pPlayer)
		{
			ReLoadModel();
		}
	}
	else if (m_szName == "Win_Inventory_CostumeQuick")
	{
		for ( i=0;i<COSTUME_NUM;i++)
		{
//			for (int j=0;j<COSTUME_TYPE_NUM;j++)
//			{
//				m_nCostumeIds[i][j]=0;
//			}
			sprintf(szItem, "Img_Hat%d", i+1);
			m_pImgItemQuick[i][0] = (PAUIIMAGEPICTURE)GetDlgItem(szItem);
			sprintf(szItem, "Img_cloth%d", i+1);
			m_pImgItemQuick[i][1] = (PAUIIMAGEPICTURE)GetDlgItem(szItem);
			sprintf(szItem, "Img_Shoe%d", i+1);
			m_pImgItemQuick[i][2] = (PAUIIMAGEPICTURE)GetDlgItem(szItem);
		}
	}
	
	return true;
}

void CDlgInventoryCostume::OnShowDialog()
{
	if (m_szName == "Win_Inventory_Costume")
	{
		CheckRadioButton(1,m_bRandom? 2 : 1);
		PAUICHECKBOX pChk = (PAUICHECKBOX)GetDlgItem("Chk_Time");
		pChk->Check(m_bAutoChange);
		if (m_pPlayer)
		{
			CECHostPlayer* pHost = GetHostPlayer();
			pHost->SyncDemonstrationPlayer(m_pPlayer, 1);
			memcpy(m_aEquips, pHost->GetEquips(), sizeof(m_aEquips));
			m_pPlayer->ShowEquipments(m_aEquips, false, true);
			m_pPlayer->SetFashionMode(true);
		}
	}
	else if (m_szName == "Win_Inventory_CostumeQuick")
	{
		CDlgInventoryCostume *pInventoryCos = (CDlgInventoryCostume*)GetGameUIMan()->GetDialog("Win_Inventory_Costume");
		if (pInventoryCos->IsShow())
		{
			AlignTo(pInventoryCos, AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
						AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_BOTTOM, -1);
		}
	}
}

bool CDlgInventoryCostume::Release()
{
	return CDlgBase::Release();
}

void CDlgInventoryCostume::OnTick()
{
	if (m_szName == "Win_Inventory_Costume")
	{
		UpdateInventory();
		
		if (m_pPlayer)
		{
			m_pPlayer->Stand();
			m_pPlayer->Tick(g_pGame->GetTickTime());
			
			if( m_pBtn_LeftTurn->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
				m_nAngle = (m_nAngle + 360 - GetGame()->GetTickTime() / 6 ) % 360;
			else if( m_pBtn_RightTurn->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
				m_nAngle = (m_nAngle + GetGame()->GetTickTime() / 6) % 360;
			m_pImg_Char->SetRenderCallback(
				PlayerRenderDemonstration,
				(DWORD)m_pPlayer,
				m_nAngle);
		}
	}
	else if (m_szName == "Win_Inventory_CostumeQuick")
	{
		UpdateCostomeQuick();
		AlignTo(GetGameUIMan()->GetDialog("Win_Inventory_Costume"),AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
						AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_BOTTOM, -1);
	}
}

void CDlgInventoryCostume::OnCommand_ChangeQuickBar(const char * szCommand)
{
	CDlgInventoryCostume *pInventoryCosQB = (CDlgInventoryCostume*)GetGameUIMan()->GetDialog("Win_Inventory_CostumeQuick");
	pInventoryCosQB->Show(!pInventoryCosQB->IsShow());
}
void CDlgInventoryCostume::OnCommand_ResumeEquip(const char * szCommand)
{
	if (m_pPlayer)
	{
		CECHostPlayer* pHost = GetHostPlayer();
		memcpy(m_aEquips, pHost->GetEquips(), sizeof(m_aEquips));
		m_pPlayer->ShowEquipments(m_aEquips, false, true);
		m_pPlayer->SetFashionMode(true);
	}
}

void CDlgInventoryCostume::OnCommand_ChangeCostume(const char * szCommand)
{
	int typeId = atoi(szCommand + strlen("type"));
	CECHostPlayer::FASHION_HOTKEY fHotkey = GetHostPlayer()->GetFashionHotkey(typeId);
	m_aEquips[EQUIPIVTR_FASHION_HEAD] = fHotkey.id_head;
	m_aEquips[EQUIPIVTR_FASHION_BODY] = fHotkey.id_body;
	m_aEquips[EQUIPIVTR_FASHION_FOOT] = fHotkey.id_shoe;
	m_pPlayer->ShowEquipments(m_aEquips, false, true);
	m_pPlayer->SetFashionMode(true);
}

void CDlgInventoryCostume::OnCommand_FashionPocketHelp(const char * szCommand)
{
	//PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_InventoryCostume_Help");
	//pDlg->Show(!pDlg->IsShow());
	AString strDlg = "Win_InventoryCostume_Help";
	GetGameUIMan()->CommonMessage("Win_HelpInfo","open",(WPARAM)(&strDlg),0);
}

void CDlgInventoryCostume::OnCommand_AutoChangeType(const char * szCommand)
{
	if (0 == stricmp(szCommand, "order"))
	{
		m_bRandom = false;
	}
	else if (0 == stricmp(szCommand, "random"))
	{
		m_bRandom = true;
	}
}

void CDlgInventoryCostume::OnCommand_ChangeTime(const char * szCommand)
{
	m_ChangeTimeCnt.SetPeriod(1000*60*a_ChangeTimes[m_pComboTime->GetCurSel()]);
}

void CDlgInventoryCostume::OnCommand_AutoChange(const char * szCommand)
{
	PAUICHECKBOX pChk = (PAUICHECKBOX)GetDlgItem("Chk_Time");
	m_bAutoChange = pChk->IsChecked();
}

void CDlgInventoryCostume::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
	if (m_szName == "Win_Inventory_Costume")
	{
		//GetGameUIMan()->GetDialog("Win_InventoryCostume_Help")->Show(false);
		GetGameUIMan()->GetDialog("Win_Inventory_CostumeQuick")->Show(false);
	}
}

void CDlgInventoryCostume::ChangeCostume(int index)
{
	int iSlot = -1;
	CECInventory *pInventory = GetHostPlayer()->GetFashionPack();
	CECHostPlayer::FASHION_HOTKEY fHotkey = GetHostPlayer()->GetFashionHotkey(index);
	if ( fHotkey.id_head>0 && A3DCOLORRGB(128, 128, 128) != m_pImgItemQuick[index][0]->GetColor() )
	{
		m_aEquips[EQUIPIVTR_FASHION_HEAD] = fHotkey.id_head;
		iSlot = pInventory->FindItem(fHotkey.id_head);
		if ( iSlot != -1)
			GetHostPlayer()->UseItemInPack(IVTRTYPE_FASHION, iSlot);
	}
	if ( fHotkey.id_body>0 && A3DCOLORRGB(128, 128, 128) != m_pImgItemQuick[index][1]->GetColor())
	{
		m_aEquips[EQUIPIVTR_FASHION_BODY] = fHotkey.id_body;
		iSlot = pInventory->FindItem(fHotkey.id_body);
		if ( iSlot != -1)
			GetHostPlayer()->UseItemInPack(IVTRTYPE_FASHION, iSlot);
	}
	if ( fHotkey.id_shoe>0 && A3DCOLORRGB(128, 128, 128) != m_pImgItemQuick[index][2]->GetColor())
	{
		m_aEquips[EQUIPIVTR_FASHION_FOOT] = fHotkey.id_shoe;
		iSlot = pInventory->FindItem(fHotkey.id_shoe);
		if ( iSlot != -1)
			GetHostPlayer()->UseItemInPack(IVTRTYPE_FASHION, iSlot);
	}
}

void CDlgInventoryCostume::OnCommand_SaveQuickBar(const char * szCommand)
{
	GetGameSession()->c2s_CmdUpdateFashionHotkey();
}

void CDlgInventoryCostume::OnEventLButtonDownQuickHat(WPARAM, LPARAM lParam, AUIObject *pObj)
{
	int index = atoi(pObj->GetName()+strlen("Img_Hat"))-1;
	CECHostPlayer::FASHION_HOTKEY fHotkey = GetHostPlayer()->GetFashionHotkey(index);
	if (fHotkey.id_head>0)
		GetHostPlayer()->SetFashionHotkey(index,-1,fHotkey.id_body,fHotkey.id_shoe);
}

void CDlgInventoryCostume::OnEventLButtonDownQuickCloth(WPARAM, LPARAM lParam, AUIObject *pObj)
{
	int index = atoi(pObj->GetName()+strlen("Img_cloth"))-1;
	CECHostPlayer::FASHION_HOTKEY fHotkey = GetHostPlayer()->GetFashionHotkey(index);
	if (fHotkey.id_body>0)
		GetHostPlayer()->SetFashionHotkey(index,fHotkey.id_head,-1,fHotkey.id_shoe);
}

void CDlgInventoryCostume::OnEventLButtonDownQuickShoe(WPARAM, LPARAM lParam, AUIObject *pObj)
{
	int index = atoi(pObj->GetName()+strlen("Img_Shoe"))-1;
	CECHostPlayer::FASHION_HOTKEY fHotkey = GetHostPlayer()->GetFashionHotkey(index);
	if (fHotkey.id_shoe>0)
		GetHostPlayer()->SetFashionHotkey(index,fHotkey.id_head,fHotkey.id_body,-1);
}

void CDlgInventoryCostume::OnEventLButtonDown(WPARAM, LPARAM lParam, AUIObject *pObj)
{
	if( !pObj || !pObj->GetDataPtr("ptr_CECIvtrItem") )
		return;

	CECIvtrItem *pItem = (CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem");
	if (m_szName == "Win_Inventory_Costume")
	{
		if( !pItem->IsFrozen() )
		{
			A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
			
			GetGameUIMan()->m_ptLButtonDown.x = GET_X_LPARAM(lParam) - p->X;
			GetGameUIMan()->m_ptLButtonDown.y = GET_Y_LPARAM(lParam) - p->Y;
			GetGameUIMan()->InvokeDragDrop(this, pObj, GetGameUIMan()->m_ptLButtonDown);
			GetGameUIMan()->PlayItemSound((CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem"), false);
			GetGameUIMan()->m_pDlgDragDrop->SetData(GetFirstItem());
		}
	}
}

void CDlgInventoryCostume::OnLButtonDbClkItem(WPARAM, LPARAM lParam, AUIObject *pObj)
{
	if( !pObj || !pObj->GetDataPtr("ptr_CECIvtrItem") )
		return;
	CECIvtrItem* pIvtrSrc = (CECIvtrItem*)pObj->GetDataPtr("ptr_CECIvtrItem");
	int iSrc = atoi(pObj->GetName() + strlen("Img_Item"))-1;

//	CECIvtrItem *pIvtrSrc = (CECIvtrItem*)pObj->GetDataPtr("ptr_CECIvtrItem");
//	
//	int iSrc = atoi(pObj->GetName() + strlen("Img_Item"));
//	CECInventory *pInventory = GetHostPlayer()->GetPack();
//	int iDst = GetHostPlayer()->GetPack()->SearchEmpty();
//	if( iDst != -1 )
//		GetGameSession()->c2s_CmdExgIvtrFashionItem(iDst, iSrc);

	if( ((CECIvtrEquip*)pIvtrSrc)->GetStateFlag() & ITEM_STATE_BINDAFTEREQUIP )
	{
		PAUIDIALOG pMsgBox;
		GetGameUIMan()->MessageBox("Game_EquipBind4", GetStringFromTable(811), 
			MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pMsgBox->SetData(iSrc+GetFirstItem());
		return;
	}
	else
	{
		GetHostPlayer()->UseItemInPack(IVTRTYPE_FASHION, iSrc+GetFirstItem());
	}

}

void CDlgInventoryCostume::OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if (m_szName == "Win_Inventory_Costume")
	{
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
}

bool CDlgInventoryCostume::UpdateInventory()
{
	int i, nMax;
	AString strFile;
	ACHAR szText[40];
	AUIClockIcon *pClock;
	PAUIIMAGEPICTURE pObj;
	CECInventory *pInventory = GetHostPlayer()->GetFashionPack();
	CECIvtrItem *pItem;
	int nIvtrSize = pInventory->GetSize();
	a_sprintf(szText, _AL("%d/%d"), pInventory->GetEmptySlotNum(), nIvtrSize);
	
	if( nIvtrSize > CECDLGINVCOSTUME_PACKMAX )
	{
		m_pScl_Item->SetScrollRange(0, (nIvtrSize - 1) / CECDLGSHOP_PACKLINEMAX - 
			CECDLGINVCOSTUME_PACKMAX / CECDLGSHOP_PACKLINEMAX + 1);
		m_pScl_Item->Show(true);
		m_pScl_Item->SetBarLength(-1);
		m_pScl_Item->SetScrollStep(1);
	}
	else
	{
		m_pScl_Item->SetScrollRange(0, 0);
		m_pScl_Item->Show(false);
	}
	if( m_nFirstLine != m_pScl_Item->GetBarLevel() )
		m_nFirstLine = m_pScl_Item->GetBarLevel();
	int nStart = m_nFirstLine * CECDLGSHOP_PACKLINEMAX;

	for( i = 0; i < CECDLGINVCOSTUME_PACKMAX; i++ )
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
				pObj->SetColor(GetGameUIMan()->GetItemColor(pItem));
				if( pItem->GetCoolTime(&nMax) > 0 )
				{
					pClock->SetProgressRange(0, nMax);
					pClock->SetProgressPos(nMax - pItem->GetCoolTime());
					pClock->SetColor(A3DCOLORRGBA(0, 0, 0, 128));
				}
				pObj->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
					GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
				
				if (pItem->IsLocked())
				{
					pObj->SetCover(CDlgItemLock::m_pA2DSpriteLock, 0, 1);
				}

				if( pItem->GetClassID() == CECIvtrItem::ICID_EQUIP )
				{
					CECIvtrEquip *pEquip = (CECIvtrEquip *) pItem;
					WORD num_estone = pEquip->GetEssence().num_estone;
					if (num_estone >= 9)
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
			pObj->Show(false);
			pObj->SetText(_AL(""));
			pObj->SetDataPtr(NULL);
			pObj->SetCover(NULL, -1);
			pObj->SetColor(A3DCOLORRGB(255, 255, 255));
		}
	}

	return true;
}

bool CDlgInventoryCostume::UpdateCostomeQuick()
{
	CECHostPlayer::FASHION_HOTKEY fHotkey;
	AString strFile;
	PAUIIMAGEPICTURE pObj;
	CECIvtrItem *pItem;
	CDlgInventoryCostume *pDlgInventoryC = (CDlgInventoryCostume*)GetGameUIMan()->GetDialog("Win_Inventory_Costume");

	int i = 0;
	for (i=0;i<CECHostPlayer::FASHION_HOTKEY_NUM;i++)
	{
		fHotkey = GetHostPlayer()->GetFashionHotkey(i);
		pObj = m_pImgItemQuick[i][0];
		pObj->SetData(0);
		if (fHotkey.id_head>0)
		{
			pObj->SetData(fHotkey.id_head);
			pItem = CECIvtrItem::CreateItem(fHotkey.id_head, 0, 1);
			if (pItem)
			{
				af_GetFileTitle(pItem->GetIconFile(), strFile);
				strFile.MakeLower();
				if (!pDlgInventoryC->InCostumePacket(fHotkey.id_head))
				{
					pItem->Freeze(true);
				}
				pObj->SetColor(GetGameUIMan()->GetItemColor(pItem));
				pObj->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
					GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
				delete pItem;
			}
		}
		else
			pObj->SetCover(NULL, -1);
		pObj = m_pImgItemQuick[i][1];
		pObj->SetData(0);
		if (fHotkey.id_body>0)
		{
			pObj->SetData(fHotkey.id_body);
			pItem = CECIvtrItem::CreateItem(fHotkey.id_body, 0, 1);
			if (pItem)
			{
				af_GetFileTitle(pItem->GetIconFile(), strFile);
				strFile.MakeLower();
				if (!pDlgInventoryC->InCostumePacket(fHotkey.id_body))
				{
					pItem->Freeze(true);
				}
				pObj->SetColor(GetGameUIMan()->GetItemColor(pItem));
				pObj->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
					GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
				delete pItem;
			}
		}
		else
			pObj->SetCover(NULL, -1);
		pObj = m_pImgItemQuick[i][2];
		pObj->SetData(0);
		if (fHotkey.id_shoe>0)
		{
			pObj->SetData(fHotkey.id_shoe);
			pItem = CECIvtrItem::CreateItem(fHotkey.id_shoe, 0, 1);
			if (pItem)
			{
				af_GetFileTitle(pItem->GetIconFile(), strFile);
				strFile.MakeLower();
				if (!pDlgInventoryC->InCostumePacket(fHotkey.id_shoe))
				{
					pItem->Freeze(true);
				}
				pObj->SetColor(GetGameUIMan()->GetItemColor(pItem));
				pObj->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
					GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
				delete pItem;
			}
		}
		else
			pObj->SetCover(NULL, -1);
	}

	return true;
}

bool CDlgInventoryCostume::InCostumePacket(int id)
{
	int i;
	CECIvtrItem *pItem;
	CECInventory *pInventory = GetHostPlayer()->GetFashionPack();
	for( i = 0; i < CECDLGINVCOSTUME_PACKMAX; i++ )
	{
		if (pItem = pInventory->GetItem(i))
		{
			if (pItem->GetTemplateID() == id)
			{
				return true;
			}
		}
	}
	return false;
}

void CDlgInventoryCostume::UpdateAutoChange(DWORD dwTime)
{
	if (!m_bAutoChange)
	{
		return;
	}
	if (m_ChangeTimeCnt.IncCounter(dwTime))
	{
		m_ChangeTimeCnt.Reset();
		bool bEmpty[CECHostPlayer::FASHION_HOTKEY_NUM];
		int	 iUsefullNum = 0;
		for (int i=0; i<CECHostPlayer::FASHION_HOTKEY_NUM; i++)
		{
			CECHostPlayer::FASHION_HOTKEY fHotkey = GetHostPlayer()->GetFashionHotkey(i);
			if (fHotkey.id_body<=0&&fHotkey.id_head<=0&&fHotkey.id_shoe<=0)
			{
				bEmpty[i] = true;
			}
			else
			{
				bEmpty[i] = false;
				iUsefullNum++;
			}
		}
		if (iUsefullNum>1)
		{
			int index = 0;
			if (m_bRandom)
			{
				int iNext = a_Random(0,iUsefullNum-1);
				m_iCurIndex = 0;
				int i;
				for (i=0; i<CECHostPlayer::FASHION_HOTKEY_NUM; ++i)
				{
					if(bEmpty[i])
						continue;

					if (iNext==0)
					{
						m_iCurIndex = i;
						break;
					}
					iNext--;
				}
			}
			else
			{
				int i;
				for (i=0; i<CECHostPlayer::FASHION_HOTKEY_NUM; ++i)
				{
					int index = m_iCurIndex+i+1;
					if (index>=CECHostPlayer::FASHION_HOTKEY_NUM)
					{
						index = 0;
					}
					if(!bEmpty[index])
					{
						m_iCurIndex = index;
						break;
					}
				}
			}
			GetGameUIMan()->m_pDlgCharacter->ChangeFashion(m_iCurIndex);
		}
	}
}

void CDlgInventoryCostume::ReLoadModel()
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