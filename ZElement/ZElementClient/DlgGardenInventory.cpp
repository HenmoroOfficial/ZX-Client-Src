// Filename	: DlgGardenInventory.cpp
// Creator	: Fu Chonggang
// Date		: 2009/08/21

#include "AFI.h"
#include "DlgGardenInventory.h"
#include "DlgItemLock.h"
#include "DlgDragDrop.h"
#include "DlgGardenMan.h"
#include "DlgInputNO.h"

#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_HTC2SRawCmds.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrHTItem.h"
#include "EC_Global.h"
#include "EC_Game.h"

#include "hometowntemplate.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgGardenInventory, CDlgBase)

AUI_ON_COMMAND("Btn_Close",		OnCommand_Close)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgGardenInventory, CDlgBase)

AUI_ON_EVENT("Item_*",			WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Item_*",			WM_LBUTTONDBLCLK,	OnEventLButtonDblclk)
AUI_ON_EVENT("Item_*",			WM_RBUTTONUP,		OnEventLButtonDblclk)

AUI_END_EVENT_MAP()

using namespace HT_S2C;
using namespace HT_S2C::CMD;


CDlgGardenInventory::CDlgGardenInventory()
{
}

CDlgGardenInventory::~CDlgGardenInventory()
{
}

bool CDlgGardenInventory::OnInitDialog()
{
	int i;
	char szItem[40];
	m_pScl_Item = (PAUISCROLL)GetDlgItem("Scl_Item");
	m_pTxt_BagNum = (PAUILABEL)GetDlgItem("Txt_BagNum");
	for( i = 0; i < CECDLGSHOP_PACKMAX; i++ )
	{
		sprintf(szItem, "Item_%02d", i);
		m_pImgItem[i] = (PAUIIMAGEPICTURE)GetDlgItem(szItem);
	}
	m_pTxt_Money = (PAUILABEL)GetDlgItem("Txt_Money");
	m_pTxt_Money->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_iFirstLine = 0;
	
	return true;
}

void CDlgGardenInventory::OnCommand_Close(const char * szCommand)
{
	Show(false);
}

void CDlgGardenInventory::OnTick()
{
	UpdateInventory();
}

void CDlgGardenInventory::OnPrtc_UseItem(void *pData)
{
	
}

void CDlgGardenInventory::OnPrtc_GetPackage(void *pData)
{
	package_list* pPackList = (package_list*)pData;
	pocket_info &pPackInfo = pPackList->pktinfo;
	m_iCapacity = pPackInfo.capacity;

	UpdateInventory();
}

void CDlgGardenInventory::UpdateInventory()
{
	int i, nMax;
	AString strFile;
	ACHAR szText[40];
	AUIClockIcon *pClock;
	PAUIIMAGEPICTURE pObj;
	CECInventory *pInventory;
	CECIvtrItem *pItem;

	int nIvtrSize = 0;
	pInventory = GetHostPlayer()->GetHTFarmPack();
	nIvtrSize = pInventory->GetSize();

	a_sprintf(szText, _AL("%d/%d"), pInventory->GetEmptySlotNum(), nIvtrSize);
	m_pTxt_BagNum->SetText(szText);
	
	if( nIvtrSize > CECDLGSHOP_PACKMAX )
	{
		m_pScl_Item->SetScrollRange(0, (nIvtrSize - 1) / CECDLGSHOP_PACKLINEMAX - 
			CECDLGSHOP_PACKMAX / CECDLGSHOP_PACKLINEMAX + 1);
		m_pScl_Item->Show(true);
		m_pScl_Item->SetBarLength(-1);
		m_pScl_Item->SetScrollStep(1);
	}
	else
	{
		m_pScl_Item->SetScrollRange(0, 0);
		m_pScl_Item->Show(false);
	}

	if( m_iFirstLine != m_pScl_Item->GetBarLevel() )
		m_iFirstLine = m_pScl_Item->GetBarLevel();
	int nStart = m_iFirstLine * CECDLGSHOP_PACKLINEMAX;

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
				pObj->SetDataPtr(pItem,"ptr_CECIvtrItem");
				pObj->SetData(CECGameUIMan::ICONS_GARDEN);
				if( pItem->GetCount() > 1 )
				{
					a_sprintf(szText, _AL("%d"), pItem->GetCount());
					pObj->SetText(szText);
				}
				else
					pObj->SetText(_AL(""));

				//af_GetFileTitle(pItem->GetIconFile(), strFile);
				strFile = pItem->GetIconFile();
				int iFirst = strFile.Find('/');
				if (iFirst>0)
					strFile.CutLeft(iFirst+1);
				strFile.MakeLower();
				pObj->SetColor(GetGameUIMan()->GetItemColor(pItem));
				if( pItem->GetCoolTime(&nMax) > 0 )
				{
					pClock->SetProgressRange(0, nMax);
					pClock->SetProgressPos(nMax - pItem->GetCoolTime());
					pClock->SetColor(A3DCOLORRGBA(0, 0, 0, 128));
				}
				int index1 = GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_GARDEN][strFile];
				pObj->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_GARDEN],
					index1);
				
				if (pItem->IsLocked())
				{
					pObj->SetCover(CDlgItemLock::m_pA2DSpriteLock, 0, 1);
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
	
	if (0==stricmp(m_szName,"Win_Garden_Inventory"))
	{
		INT64 nMoney = CDlgGardenMan::GetGardenMoney();
		ACString strMoney;
		GetGameUIMan()->Int64ToStr(nMoney, strMoney);
		m_pTxt_Money->SetText(strMoney);
	}

}

void CDlgGardenInventory::UpdateHint()
{

}

void CDlgGardenInventory::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	
	if( !strstr(pObj->GetName(), "Item_") )
		return;
	if (pObj->GetDataPtr("ptr_CECIvtrItem")==NULL)
	{
		return;
	}
	CECIvtrItem *pItem = (CECIvtrItem*)pObj->GetDataPtr("ptr_CECIvtrItem");
	int index = atoi(pObj->GetName()+strlen("Item_"));
	if (pItem->GetClassID()==CECIvtrItem::ICID_HOMETOWN_TOOL)
	{
		CECIvtrHTTool *htTool = (CECIvtrHTTool*)pItem;
		A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
		POINT pt = 
		{
			GET_X_LPARAM(lParam) - p->X,
			GET_Y_LPARAM(lParam) - p->Y,
		};
		GetGameUIMan()->m_pDlgDragDrop->SetData(GetFirstItem());
		GetGameUIMan()->InvokeDragDrop(this, pObj,pt);
		m_pos = index+GetFirstItem();
		m_itemid = pItem->GetTemplateID();
		m_targettype = HOMETOWN::FARM_TARGET_FIELD;
	}
}

void CDlgGardenInventory::OnEventLButtonDblclk(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( !strstr(pObj->GetName(), "Item_") )
		return;
	if (pObj->GetDataPtr("ptr_CECIvtrItem")==NULL)
	{
		return;
	}
	CECIvtrItem *pItem = (CECIvtrItem*)pObj->GetDataPtr("ptr_CECIvtrItem");
	int index = atoi(pObj->GetName()+strlen("Item_"));
	if (pItem->GetClassID()==CECIvtrItem::ICID_HOMETOWN_FRUIT)
	{
		int pos = GetFirstItem()+index;
		int id = pItem->GetTemplateID();
		GetGameUIMan()->InvokeNumberDialog((PAUIOBJECT)pos, (PAUIOBJECT)id, CDlgInputNO::INPUTNO_GARDENQSHOPITEM_SELL, pItem->GetCount());
	}
	else if (pItem->GetClassID()==CECIvtrItem::ICID_HOMETOWN_TOOL && GetGameUIMan()->GetDialog("Win_Garden")->IsShow())
	{
		CECIvtrHTTool *htTool = (CECIvtrHTTool*)pItem;
		m_targettype = htTool->GetTargetType();
		if (m_targettype == HOMETOWN::FARM_TARGET_FARM|| m_targettype == HOMETOWN::FARM_TARGET_SELF)
		{
			PAUIDIALOG pMsgBox;
			GetGameUIMan()->MessageBox("Game_GardenUseItemWholeFarm", GetGameUIMan()->GetStringFromTable(8598), 
				MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
			m_pos = index+GetFirstItem();
			m_itemid = pItem->GetTemplateID();
		}

	}
}

void CDlgGardenInventory::UseItem()
{
	CDlgGardenMan *pDlgGarden = (CDlgGardenMan*)GetGameUIMan()->GetDialog("Win_Garden");
	htc2s_SendCmdUseItem(m_pos,m_itemid, m_targettype, pDlgGarden->GetOwnerId(), m_fieldid);
}
