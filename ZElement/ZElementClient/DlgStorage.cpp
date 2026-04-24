/********************************************************************
	created:	2005/08/15
	created:	15:8:2005   19:38
	file name:	DlgStorage.cpp
	author:		yaojun
	
	purpose:	
*********************************************************************/

#include "AFI.h"
#include "AUI\\AUIDef.h"
#include "AUI\\AUIEditBox.h"
#include "DlgStorage.h"
#include "DlgInputNO.h"
#include "DlgDragDrop.h"
#include "DlgSkill.h"
#include "DlgItemLock.h"
#include "DlgInventoryExpand.h"
#include "DlgBooth.h"
#include "DlgTrade.h"

#include "EC_Algorithm.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrTalismanMain.h"
#include "EC_IvtrRune.h"
#include "EC_Inventory.h"

#define new A_DEBUG_NEW

static const int ARRANGE_ITEM_MAX_DURATION_SEC = 15; // Max duration of each Arrange
DWORD CDlgStorage::ARRANGE_ITEM_INTERVEL_SEC = 300; // interval of each Arrange

#define  LOCAL_DIR "userdata\\layout"

AUI_BEGIN_COMMAND_MAP(CDlgStorage, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",		OnCommandIDCANCEL)
AUI_ON_COMMAND("choosemoney",	OnCommandChooseMoney)
AUI_ON_COMMAND("arrange",		OnCommand_AutoArrangeItems)
AUI_ON_COMMAND("Btn_ArrangeAll",OnCommand_AutoArrangeItems)
AUI_ON_COMMAND("Rdo_Page*",		OnCommandPage)
AUI_ON_COMMAND("Btn_Expand",    OnCommandExpand)
AUI_ON_COMMAND("Btn_Switch",    OnCommandModeSwitch)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgStorage, CDlgBase)

AUI_ON_EVENT("*",				WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT(NULL,				WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT("Img_FrameMoney",	WM_LBUTTONDOWN,		OnLButtonDownItem_Money)
AUI_ON_EVENT("Item_*",			WM_LBUTTONDOWN,		OnLButtonDownItem)
AUI_ON_EVENT("Item_*",			WM_RBUTTONUP,		OnLButtonDbClkItem)
AUI_ON_EVENT("Item_*",			WM_LBUTTONDBLCLK,	OnLButtonDbClkItem)

AUI_END_EVENT_MAP()

CDlgStorage::CDlgStorage()
{
	m_nLastSendTime = 0;
	m_nSendInterval = 0;
	m_nCurSendNum = -1;
	m_bPageMode = true;
}

CDlgStorage::~CDlgStorage()
{
}

bool CDlgStorage::OnInitDialog()
{
	int i;
	char szItem[20];

	m_pTxt_Money1 = (PAUILABEL)GetDlgItem("Txt_Money1");
	m_pTxt_Money2 = (PAUILABEL)GetDlgItem("Txt_Money2");
	m_pTxt_Money3 = (PAUILABEL)GetDlgItem("Txt_Money3");
	m_pTxt_Money1->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Money2->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Money3->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_BagNum = (PAUILABEL)GetDlgItem("Txt_BagNum");
	m_pLab_Expand = (PAUILABEL)GetDlgItem("Lab_Expand");
	m_pScl_Item = (PAUISCROLL)GetDlgItem("Scl_Item");
	m_pBtn_Arrange = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Arrange");
	m_pBtn_ArrangeAll = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_ArrangeAll");
	m_pBtn_Expand = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Expand");
	m_pBtn_Switch = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Switch");

	for(i = 0; i < 6; i++)
	{
		sprintf(szItem, "Rdo_Page%02d", i + 1);
		m_pRdo_Page[i] = (PAUIRADIOBUTTON)GetDlgItem(szItem);
	}
	CheckRadioButton(3, 1);

	for( i = 0; i < CECDLGSTORAGE_PACKMAX; i++ )
	{
		sprintf(szItem, "Item_%02d", i);
		m_pImgItem[i] = (PAUIIMAGEPICTURE)GetDlgItem(szItem);
	}

	m_pLab_Money = (PAUILABEL)GetDlgItem("Lab_Money");
	if( !m_pLab_Money ) return false;
	m_pImg_Money = (PAUIIMAGEPICTURE)GetDlgItem("Img_FrameMoney");
	if( !m_pImg_Money ) return false;
	m_pTxt_Title1 = (PAUILABEL)GetDlgItem("Title1");
	if( !m_pTxt_Title1 ) return false;
	m_pTxt_Title2 = (PAUILABEL)GetDlgItem("Title2");
	if( !m_pTxt_Title2 ) return false;

	m_nFirstLine = 0;
	m_bFaction = false;

	ARRANGE_ITEM_INTERVEL_SEC = 60;
	m_pBtn_Arrange->SetHint(GetStringFromTable(3553));
	m_pBtn_ArrangeAll->SetHint(GetStringFromTable(18771));

	ACHAR strText[50];
	GetPrivateProfileString(_AL("Mode"), _AL("Storage"), _AL(""), 
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

	return true;
}

void CDlgStorage::OnShowDialog()
{

}

void CDlgStorage::OnCommandIDCANCEL(const char *szCommand)
{
	if (m_aExchanges2Send.size() > 0) {
		// Force End arrange send.
		GetGameRun()->AddChatMessage(GetGameUIMan()->GetStringFromTable(3558), GP_CHAT_MISC);

		m_nCurSendNum = (int)m_aExchanges2Send.size();
		UpdateArrangeSend();
	}

	GetGameSession()->c2s_CmdCancelAction();
	Show(false);
}

void CDlgStorage::OnCommandChooseMoney(const char *szCommand)
{
	CDlgInputNO *pDlg = GetGameUIMan()->m_pDlgInputMoney;
	pDlg->Show(true, true);
	pDlg->SetType(CDlgInputNO::INPUTNO_STORAGE_TRASH_MONEY);
	
	PAUIEDITBOX pEdit = (PAUIEDITBOX)pDlg->GetDlgItem("DEFAULT_Txt_Money1");
	pEdit->SetData(GetHostPlayer()->GetTrashBoxMoneyCnt());
}

void CDlgStorage::OnCommandPage(const char *szCommand)
{
		int nPage = GetCheckedRadioButton(3) - 1;
		m_pScl_Item->SetBarLevel(nPage * 8);
}

void CDlgStorage::OnCommandExpand(const char *szCommand)
{
	bool bCrsServer = (GetHostPlayer()->GetSrcServerID() != 0);
	if(bCrsServer)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(18706), MB_OK,
			A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	CDlgInventoryExPand* pDlgInventoryExpand = (CDlgInventoryExPand*)GetGameUIMan()->GetDialog("Win_Inventory_Expand01");
	bool bShow = pDlgInventoryExpand->IsShow();
	pDlgInventoryExpand->Show(!bShow);
	if(bShow)
		pDlgInventoryExpand->OnCommandCancel("");
}

void CDlgStorage::OnCommandModeSwitch(const char *szCommand)
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
		WritePrivateProfileString(_AL("Mode"), _AL("Storage"), strText, 
		_AL("userdata\\pagemode.ini"));
	}
	else
	{
		m_pBtn_Switch->SetText(GetStringFromTable(18820));
		m_pBtn_Switch->SetHint(GetStringFromTable(18822));
		strText = _AL("classic");
		WritePrivateProfileString(_AL("Mode"), _AL("Storage"), strText, 
			_AL("userdata\\pagemode.ini"));
	}
}

void CDlgStorage::SetPage(int iPage)
{
	if(m_bPageMode)
	{
		CheckRadioButton(3, iPage + 1);
		m_pScl_Item->SetBarLevel(iPage * 8);
	}
}

void CDlgStorage::OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
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
		if(m_szName=="Win_Storage")
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

void CDlgStorage::OnLButtonDownItem_Money(WPARAM, LPARAM lParam, AUIObject *pObj)
{
	OnCommandChooseMoney("");
}

void CDlgStorage::OnLButtonDownItem(WPARAM, LPARAM lParam, AUIObject *pObj)
{
	if( !pObj || !pObj->GetDataPtr("ptr_CECIvtrItem") )
		return;

	if( ((CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem"))->IsFrozen() )
		return;

	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	
	POINT pt = 
	{
		GET_X_LPARAM(lParam) - p->X,
		GET_Y_LPARAM(lParam) - p->Y
	};

	GetGameUIMan()->InvokeDragDrop(this, pObj, pt);
	GetGameUIMan()->PlayItemSound((CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem"), false);
	GetGameUIMan()->m_pDlgDragDrop->SetData(GetFirstItem());
}

void CDlgStorage::OnLButtonDbClkItem(WPARAM, LPARAM lParam, AUIObject *pObj)
{
	if( !pObj || !pObj->GetDataPtr("ptr_CECIvtrItem") )
		return;

	CECIvtrItem *pIvtrSrc = (CECIvtrItem*)pObj->GetDataPtr("ptr_CECIvtrItem");
	
	int iSrc = atoi(pObj->GetName() + strlen("Item_"));
	CECInventory *pInventory = GetHostPlayer()->GetPack();
	if( pIvtrSrc->GetPileLimit() > 1 )
	{
		int i;
		int nCount = pIvtrSrc->GetCount();
		for(i = 0; i < pInventory->GetSize(); i++ )
		{
			CECIvtrItem *pItem = pInventory->GetItem(i);
			if( pItem && pItem->GetTemplateID() == pIvtrSrc->GetTemplateID() && pItem->GetStateFlag() == pIvtrSrc->GetStateFlag()
				&& pItem->GetCount() < pItem->GetPileLimit() )
			{
				int nMove = min(pItem->GetPileLimit() - pItem->GetCount(), nCount);
				GetGameSession()->c2s_CmdMoveTrashBoxToIvtr(GetGameUIMan()->m_pDlgStorage->IsFactionStorage(), GetFirstItem() + iSrc, i, nMove);
				nCount -= nMove;
				if( nCount == 0 )
					return;
			}
		}
	}
	int iDst = GetHostPlayer()->GetPack()->SearchEmpty();
	if( iDst != -1 )
		GetGameSession()->c2s_CmdExgTrashBoxIvtrItem(GetGameUIMan()->m_pDlgStorage->IsFactionStorage(), GetFirstItem() + iSrc, iDst);
}

bool CDlgStorage::Tick(void)
{
	UpdateStorage();

	// Update Arrange
	if (!m_aExchanges2Send.empty() &&
		GetTickCount() - m_nLastSendTime >= ARRANGE_ITEM_MAX_DURATION_SEC * 1000)
	{
		// Force End arrange send.
		GetGameRun()->AddChatMessage(GetGameUIMan()->GetStringFromTable(3555), GP_CHAT_MISC);

		m_nCurSendNum = (int)m_aExchanges2Send.size();
		UpdateArrangeSend();
	}
	m_pBtn_Arrange->Show(m_bPageMode);
	m_pBtn_Arrange->Enable(GetHostPlayer()->GetCoolTime(COOLDOWN_INDEX_ARRANGE_TRASHBOX) <= 0);
	m_pBtn_ArrangeAll->Enable(GetHostPlayer()->GetCoolTime(COOLDOWN_INDEX_ARRANGE_TRASHBOX) <= 0);
	m_pBtn_Expand->Enable(GetHostPlayer()->GetTrashBox()->GetSize() < CECDLGSTORAGE_SIZEMAX && 
		!GetGameUIMan()->m_pDlgTrade->IsShow() &&
		GetHostPlayer()->GetBoothState() == 0);	
	m_pBtn_Switch->Enable(!GetGameUIMan()->m_pDlgTrade->IsShow() && GetHostPlayer()->GetBoothState() == 0);
	return CDlgBase::Tick();
}

void CDlgStorage::OnItemExchanged(int iSlot1, int iSlot2)
{
	if (m_aExchanges2Send.empty() || m_nCurSendNum < 0)
		return;

	if (m_nCurSendNum <= (int)m_aExchanges2Send.size()) {
		int iSrc = m_aExchanges2Send[m_nCurSendNum - 1].iSrc;
		int iDst = m_aExchanges2Send[m_nCurSendNum - 1].iDst;
		if (iSrc == iSlot1 && iDst == iSlot2)
			UpdateArrangeSend();
		else {
			ASSERT(0);
		}
	}
}
void CDlgStorage::UpdateArrangeSend()
{
	m_pBtn_Arrange->Enable(GetTickCount() - m_nLastSendTime >= ARRANGE_ITEM_INTERVEL_SEC*1000);

	if (m_aExchanges2Send.empty() || m_nCurSendNum < 0)
		return;

	if (m_nCurSendNum >= (int)m_aExchanges2Send.size())
	{
		// All Arrange Exchanges sent. Unfreeze All Items
		CECHostPlayer* pHost = GetHostPlayer();
		CECInventory* pIvtr =  m_bFaction ? pHost->GetFactionTrashBox() : pHost->GetTrashBox();
		if (pIvtr) pIvtr->UnfreezeAllItems();

#ifdef _DEBUG
		CreateDirectoryA(LOCAL_DIR, NULL);
		AString sub_dir;
		sub_dir.Format("%s\\%d", LOCAL_DIR, pHost->GetCharacterID());
		CreateDirectoryA(sub_dir, NULL);
		AString file_name;
		file_name.Format("%s\\ArrangeStorage.txt", sub_dir);
		AFile afile;
		if (afile.Open(file_name, AFILE_OPENAPPEND|AFILE_TEXT))
		{
			afile.WriteLine("\nAfter Arrange: \n");
			int i;
			char sz[16];
			AString str;
			for (i = 0; i < pIvtr->GetSize(); ++i)
			{
				CECIvtrItem* pItem = pIvtr->GetItem(i);
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
		GetGameSession()->c2s_CmdExgTrashBoxItem(
			GetGameUIMan()->m_pDlgStorage->IsFactionStorage(),
			m_aExchanges2Send[m_nCurSendNum].iSrc,
			m_aExchanges2Send[m_nCurSendNum].iDst);
		++m_nCurSendNum;
		m_nLastSendTime = GetTickCount();
	}
}

bool CDlgStorage::UpdateStorage()
{
	int i;
	AString strFile;
	ACHAR szText[40];
	CECIvtrItem *pItem;
	PAUIIMAGEPICTURE pObj;
	CECHostPlayer *pHost = GetHostPlayer();
	CECInventory *pInventory;
	if( m_bFaction )
	{
		m_pTxt_Title1->Show(false);
		m_pTxt_Title2->Show(true);
		m_pLab_Money->Show(false);
		m_pImg_Money->Show(false);
		m_pTxt_Money1->Show(false);
		m_pTxt_Money2->Show(false);
		m_pTxt_Money3->Show(false);
		pInventory = pHost->GetFactionTrashBox();
	}
	else
	{
		m_pTxt_Title1->Show(true);
		m_pTxt_Title2->Show(false);
		m_pLab_Money->Show(true);
		m_pImg_Money->Show(true);
		m_pTxt_Money1->Show(true);
		m_pTxt_Money2->Show(true);
		m_pTxt_Money3->Show(true);
		pInventory = pHost->GetTrashBox();
	}

	for(i = 0; i < 6; i++)
	{
		m_pRdo_Page[i]->Show(false);
	}
	if(m_bPageMode)
		m_pRdo_Page[0]->Show(true);

	int nIvtrSize = pInventory->GetSize();
	a_sprintf(szText, _AL("%d/%d"), nIvtrSize, 252);
	m_pTxt_BagNum->SetText(szText);
	if( nIvtrSize > CECDLGSTORAGE_PACKMAX )
	{		
		if(m_bPageMode)
		{
			int nRange = (nIvtrSize - 1) / CECDLGSTORAGE_PACKMAX * 8;
			m_pScl_Item->SetScrollRange(0, nRange);
			m_pScl_Item->Show(false);
			m_pScl_Item->SetBarLength(-1);
			m_pScl_Item->SetScrollStep(1);
			for(i = 0; i <= (nIvtrSize - 1) / CECDLGSTORAGE_PACKMAX; i++)
			{
				m_pRdo_Page[i]->Show(true);
			}
		}
		else
		{
			m_pScl_Item->SetScrollRange(0, (nIvtrSize - 1) / CECDLGSTORAGE_PACKLINEMAX - 
				CECDLGSTORAGE_PACKMAX / CECDLGSTORAGE_PACKLINEMAX + 1);
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

	m_pLab_Expand->Show(false);
	if( m_nFirstLine != m_pScl_Item->GetBarLevel() )
		m_nFirstLine = m_pScl_Item->GetBarLevel();
	int nStart = m_nFirstLine * CECDLGSTORAGE_PACKLINEMAX;

	for( i = 0; i < CECDLGSTORAGE_PACKMAX; i++ )
	{
		pObj = m_pImgItem[i];

		if( i + nStart < pInventory->GetSize() )
		{
			pObj->Show(true);
			pObj->SetCover(NULL, -1, 1);

			pItem = pInventory->GetItem(i + nStart);
			if( pItem )
			{
				pObj->SetDataPtr(pItem,"ptr_CECIvtrItem");
				if( pItem->GetCount() > 1 )
				{
					a_sprintf(szText, _AL("%d"), pItem->GetCount());
					pObj->SetText(szText);
				}
				else
					pObj->SetText(_AL(""));

				af_GetFileTitle(pItem->GetIconFile(), strFile);
				strFile.MakeLower();
				pObj->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
					GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
				pObj->SetColor(GetGameUIMan()->GetItemColor(pItem));
				
				if (pItem->IsLocked())
				{
					pObj->SetCover(CDlgItemLock::m_pA2DSpriteLock, 0, 1);
				}
				
				if( ( pItem->GetClassID()==CECIvtrItem::ICID_EQUIP && ((CECIvtrEquip*)pItem)->GetEssence().num_estone>=9 ) ||
					( pItem->GetClassID()==CECIvtrItem::ICID_TALISMAN_MAINPART && ((CECIvtrTalismanMain*)pItem)->GetRebornCount()>0 ) ||
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
			}
			else
			{
				pObj->ClearCover();
				pObj->SetText(_AL(""));
				pObj->SetDataPtr(NULL);
				pObj->SetColor(A3DCOLORRGB(255, 255, 255));
			}
		}
		else
			pObj->Show(false);
	}

	int nMoney = pHost->GetTrashBoxMoneyCnt();
	ACHAR szMoney[20];
	a_sprintf(szMoney, _AL("%d"), nMoney % 100);
	m_pTxt_Money3->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), (nMoney % 10000) / 100);
	m_pTxt_Money2->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), nMoney / 10000);
	m_pTxt_Money1->SetText(szMoney);

	return true;
}

void CDlgStorage::OnCommand_AutoArrangeItems(const char * szCommand)
{
	int page = GetCheckedRadioButton(3);
	if(0 == stricmp(szCommand, "arrange"))
		GetGameSession()->c2s_CmdArrangeTrashBox(page);
	else 
		GetGameSession()->c2s_CmdArrangeTrashBox(0);
	return;
	
	
	if(0 == stricmp(szCommand, "arrange"))
		m_bPageArrange = true;
	else 
		m_bPageArrange = false;
	
	CECInventory* pIvtr =  m_bFaction ?
		GetHostPlayer()->GetFactionTrashBox() :
		GetHostPlayer()->GetTrashBox();

	// size < 2: No need to arrange
	if (!pIvtr || pIvtr->GetSize() == pIvtr->GetEmptySlotNum()) {
		GetGameRun()->AddChatMessage(GetGameUIMan()->GetStringFromTable(3556), GP_CHAT_MISC);
		return;
	}

	// Calc Exchanges
	bool bSucc = CalcArrangeExchanges();
	if (bSucc  == 0 && !m_aExchanges2Send.empty()) {
		int nPage = GetCheckedRadioButton(3) - 1;
		int nBegin = nPage * 48;
		int nEnd = (nPage + 1) * 48 - 1;
		int nPackSize = pIvtr->GetSize();
		if(nEnd >= nPackSize)
			nEnd = nPackSize - 1;
		// Freeze All Items
		for (int i = 0; i < pIvtr->GetSize(); ++i) {
			if (pIvtr->GetItem(i))
			{
				if(m_bPageArrange)
				{
					if(i >= nBegin && i <= nEnd)
						pIvtr->GetItem(i)->Freeze(true);
				}
				else
					pIvtr->GetItem(i)->Freeze(true);
			}
		}
		// Send Exchanges
		SendArrangeExchanges();
	}
	else {
		m_nSendInterval = 0;
		m_nCurSendNum = -1;

		if (bSucc < 0)
			GetGameRun()->AddChatMessage(GetGameUIMan()->GetStringFromTable(3557), GP_CHAT_MISC);
		else if (m_aExchanges2Send.empty())
			GetGameRun()->AddChatMessage(GetGameUIMan()->GetStringFromTable(3554), GP_CHAT_MISC);
	}
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

int CDlgStorage::CalcArrangeExchanges()
{
	int i;
	CECHostPlayer* pHost = GetHostPlayer();
	CECInventory* pPack = m_bFaction ? pHost->GetFactionTrashBox() : pHost->GetTrashBox();
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
			//	if (!pItem->IsFrozen())
				{
					CECGameUIMan::IDOrderMap::const_iterator it = mapIDOrder.find(pItem->GetTemplateID());
					if (it == mapIDOrder.end())
						oi.iType = TYPE_ORDER_NOT_DEFINED;
					else
						oi.iOrder = it->second;

					aPreFrozenItemNum[Orders2OldSlots.size()] = nLastTotalFrozenItem;
					Orders2OldSlots.push_back(oi);
				}
			//	else
			//	{
					// Frozen Item: Do Not Sort!
			//		++nLastTotalFrozenItem;
			//	}
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
	for (i = 0; i < nPackSize; ++i) {
		CECIvtrItem* pItem = pPack->GetItem(i);
		if (/*pItem && pItem->IsFrozen() ||*/ ((m_bPageArrange && i < nBegin) || (m_bPageArrange && i > nEnd))) {
			aPreFrozenItemNum[i] = ++nLastTotalFrozenItem;
		}
		else {
			aPreFrozenItemNum[i] = nLastTotalFrozenItem;
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
		int a = NewOrders2OldSlots[i].iOrder;
		int b = NewOrders2OldSlots[i].iSlot;
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

//			if (/*((pItemSrc && pItemSrc->IsFrozen()) || */(oldSlot < nBegin || oldSlot > nEnd) )/* || ((pItemDst && pItemDst->IsFrozen()) */|| (newSlot < nBegin || newSlot > nEnd)))
			if (((pItemSrc && pItemSrc->IsFrozen()) || ((m_bPageArrange && oldSlot < nBegin) || (m_bPageArrange && oldSlot > nEnd)) ) || ((pItemDst && pItemDst->IsFrozen()) || ((m_bPageArrange && newSlot < nBegin) || (m_bPageArrange && newSlot > nEnd))))

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

void CDlgStorage::SendArrangeExchanges()
{
	// Send the first Exchange:
	m_nCurSendNum = 0;
	GetGameSession()->c2s_CmdExgTrashBoxItem(
		GetGameUIMan()->m_pDlgStorage->IsFactionStorage(),
		m_aExchanges2Send[m_nCurSendNum].iSrc,
		m_aExchanges2Send[m_nCurSendNum].iDst);
	++m_nCurSendNum;
	m_nLastSendTime = GetTickCount();
}