// Filename	:	DlgEquipJack.cpp
// Creator	:	Fu Chonggang
// Date		:	Mar 15, 2011

#include "DlgEquipJack.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUICheckBox.h"

#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrStone.h"
#include "EC_IvtrEquip.h"
#include "EC_IvtrTypes.h"

#include "ExpTypes.h"

#define new A_DEBUG_NEW

//////////////////////////////////////////////////////////////////////////
// CDlgEquipJack
//////////////////////////////////////////////////////////////////////////

AUI_BEGIN_COMMAND_MAP(CDlgEquipJack, CDlgItemSvrBase)

AUI_ON_COMMAND("Chk_Lock*",			OnCommand_Lock)

AUI_END_COMMAND_MAP()

CDlgEquipJack::CDlgEquipJack()
{
}

CDlgEquipJack::~CDlgEquipJack()
{
}

void CDlgEquipJack::OnCommand_Lock(const char * szCommand)
{
	AString strName;
	bool bAllLock = true;
	for (int i=0; i<EQUIP_JACK_NUM; ++i)
	{
		strName.Format("Chk_Lock%d", i+1);
		PAUICHECKBOX pChk = (PAUICHECKBOX)GetDlgItem(strName);
		if (!pChk->IsChecked())
			bAllLock = false;
	}
	PAUICHECKBOX pChk = (PAUICHECKBOX)GetDlgItem(szCommand);
	if (bAllLock)
	{
		pChk->Check(false);
		GetGameUIMan()->MessageBox(1543);
	}
	else
	{
		pChk->SetText(GetGameUIMan()->GetStringFromTable(pChk->IsChecked()?1411:1410));
		int index = atoi(szCommand+strlen("Chk_Lock"));
		strName.Format("Img_Lock%d", index);
		GetDlgItem(strName)->Show(pChk->IsChecked());
	}
}

bool CDlgEquipJack::OnInitDialog()
{
	AString strName;
	for (int i=0; i<EQUIP_JACK_NUM; ++i)
	{
		strName.Format("Img_Hole%d", i+1);
		m_pImg_Jack[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		strName.Format("Img_Props%d", i+1);
		m_pImg_Props[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		strName.Format("Lab_Hole%d", i+1);
		m_pLab_Names[i] = (PAUILABEL)GetDlgItem(strName);
		strName.Format("Img_Bg%d", i+1);
		m_pImg_Back[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		strName.Format("Img_Bg%d", i+4);
		m_pImg_BackGem[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		strName.Format("Img_PropsBg%d", i+1);
		m_pImg_BackProp[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		strName.Format("Img_Gfx%d", i+1);
		m_pImg_Gfx[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		if( m_pImg_Gfx[i] )
		{
			m_pImg_Gfx[i]->SetTransparent(true);
			m_pImg_Gfx[i]->SetAcceptMouseMessage(false);
		}
	}
	return CDlgItemSvrBase::OnInitDialog();
}

void CDlgEquipJack::OnTick()
{
	CDlgItemSvrBase::OnTick();
	if ( 0==stricmp(m_szName, "Win_Accessory_Drill") || 0==stricmp(m_szName, "Win_Accessory_ReDrill") )
	{
		static int gem_slot_shape[EQUIP_JACK_NUM] = {-1, -1, -1};
		if (m_pImg_Item->GetDataPtr("ptr_CECIvtrItem"))
		{
			CECIvtrEquip *pEquip = (CECIvtrEquip*)GetHostPlayer()->GetPack()->GetItem(m_pImg_Item->GetData());
			if( pEquip != m_pImg_Item->GetDataPtr("ptr_CECIvtrItem")) // 同步包裹信息，更新物品
			{
				m_pImg_Item->SetDataPtr(pEquip, "ptr_CECIvtrItem");
				pEquip->Freeze(true);
				UpdateDisplay();
			}
			if ( gem_slot_shape[0] != pEquip->GetNewEssenceForGem().gem_slot_shape[0] ||
				gem_slot_shape[1] != pEquip->GetNewEssenceForGem().gem_slot_shape[1] ||
				gem_slot_shape[2] != pEquip->GetNewEssenceForGem().gem_slot_shape[2] )
			{
				gem_slot_shape[0] = pEquip->GetNewEssenceForGem().gem_slot_shape[0];
				gem_slot_shape[1] = pEquip->GetNewEssenceForGem().gem_slot_shape[1];
				gem_slot_shape[2] = pEquip->GetNewEssenceForGem().gem_slot_shape[2];
				UpdateDisplay();
			}
		}
	}
}

void CDlgEquipJack::SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg)
{
	// 只有饰品可以右键点击包裹，其它需要拖动 pImg 不为空
	if (!pImg||pImg==m_pImg_Item)
	{
		if (pItem->GetClassID()!=CECIvtrItem::ICID_EQUIP)
			return;

		CECIvtrEquip *pEquip = (CECIvtrEquip*)pItem;
		if (!pEquip->GetDBEssence()->gem_slot_identify)	// 不能开槽
		{
			if(m_szName=="Win_Accessory_Drill")
				GetGameUIMan()->MessageBox(1532);
			else if(m_szName=="Win_Accessory_ReDrill")
				GetGameUIMan()->MessageBox(1533);
			return;
		}
		if ((m_szName=="Win_Accessory_ReDrill"||m_szName=="Win_Accessory_Custom")&&
			pEquip->GetNewEssenceForGem().gem_slot_shape[0]<0)
		{
			GetGameUIMan()->MessageBox(1534);
			return;
		}

		CECIvtrItem *pItemOrg = (CECIvtrItem*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem");
		if( pItemOrg )
			pItemOrg->Freeze(false);
		
		m_pImg_Item->SetData(nSlot);
		m_pImg_Item->SetDataPtr(pItem, "ptr_CECIvtrItem");
		pItem->Freeze(true);
		if (m_szName=="Win_Accessory_ReDrill")
		{
			AString strName;
			for (int i=0; i<EQUIP_JACK_NUM; ++i)
			{
				strName.Format("Chk_Lock%d", i+1);
				PAUICHECKBOX pChk = (PAUICHECKBOX)GetDlgItem(strName);
				pChk->Check(false);
				pChk->SetText(GetGameUIMan()->GetStringFromTable(1410));
				strName.Format("Img_Lock%d", i+1);
				GetDlgItem(strName)->Show(false);
			}
		}
	}
	else if ( m_szName=="Win_Accessory_Custom" && 
		strstr(pImg->GetName(), "Img_Props") &&
		NULL==strstr(pImg->GetName(), "Img_PropsBg") &&
		pItem->GetClassID()==CECIvtrItem::ICID_GEM_SEAL)
	{
		CECIvtrItem *pItemOrg = (CECIvtrItem*)pImg->GetDataPtr("ptr_CECIvtrItem");
		if( pItemOrg )
			pItemOrg->Freeze(false);

		pImg->SetData(nSlot);
		pImg->SetDataPtr(pItem, "ptr_CECIvtrItem");
		pItem->Freeze(true);
	}
	else
		return;

	UpdateDisplay();
}

void CDlgEquipJack::ServerStart()
{
	if (!UpdateMoney())
	{
		GetGameUIMan()->MessageBox(7191);
		return;
	}
	CECIvtrEquip* pEquip = (CECIvtrEquip*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem");
	int iTemplId = pEquip->GetTemplateID();
	if ( 0==stricmp(m_szName, "Win_Accessory_Drill") )
	{
		PAUIDIALOG pDlg;
		GetGameUIMan()->MessageBox("Game_Hole_Drill", GetGameUIMan()->GetStringFromTable(1540), MB_YESNO, A3DCOLORRGBA(255,255,255,160), &pDlg);
		pDlg->SetData(m_pImg_Item->GetData());
		pDlg->SetDataPtr((void*)iTemplId);
	}
	else if ( 0==stricmp(m_szName, "Win_Accessory_ReDrill") )
	{
		bool bLocked[EQUIP_JACK_NUM];
		PAUICHECKBOX pChk;
		AString strName;
		int numPropNeed = 0;
		int i;
		for (i=0; i<EQUIP_JACK_NUM; ++i)
		{
			strName.Format("Chk_Lock%d", i+1);
			pChk = (PAUICHECKBOX)GetDlgItem(strName);
			bLocked[i] = pChk->IsChecked();
			if(bLocked[i])
				numPropNeed++;
		}
		CECInventory* pInventory = GetHostPlayer()->GetPack();
		int numProp = pInventory->GetItemTotalNum(GetGameRun()->GetSpecialIDConfig().id_gem_slot_lock_article);
		numProp += pInventory->GetItemTotalNum(GetGameRun()->GetSpecialIDConfig().id_gem_slot_lock_article_1);
		numProp += pInventory->GetItemTotalNum(GetGameRun()->GetSpecialIDConfig().id_gem_slot_lock_article_2);
		bool reDrillProp = false;
		for(i=0; i<3; ++i)
		{
			if(GetGameRun()->GetSpecialIDConfig().id_gem_slot_rebuild_article[i]>0 &&
				pInventory->GetItemTotalNum(GetGameRun()->GetSpecialIDConfig().id_gem_slot_rebuild_article[i])>0)
			{
				reDrillProp = true;
				break;
			}
		}
		if(!reDrillProp)
			GetGameUIMan()->MessageBox(1547);
		else if(numProp<numPropNeed)
			GetGameUIMan()->MessageBox(1531);
		else
		{
			GetGameSession()->c2s_CmdNPCSevRebuildGemSlots(m_pImg_Item->GetData(), iTemplId, bLocked);
		}
	}
	else if ( 0==stricmp(m_szName, "Win_Accessory_Custom") )
	{
		for (int i=0; i<EQUIP_JACK_NUM; ++i)
		{
			if(m_pImg_Props[i]->GetDataPtr("ptr_CECIvtrItem"))
			{
				if(((CECIvtrItem*)m_pImg_Props[i]->GetDataPtr("ptr_CECIvtrItem"))->IsLocked())
				{
					GetGameUIMan()->MessageBox(7035);
					return;
				}
			}
		}
		if(m_bEnable)
			GetGameUIMan()->MessageBox("Game_Accessory_Custom", GetGameUIMan()->GetStringFromTable(1541), MB_YESNO);
		else
			GetGameUIMan()->MessageBox(1542);
	}
}

int CDlgEquipJack::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="confirm")
	{
		int iTemplId = ((CECIvtrItem*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem"))->GetTemplateID();
		int nGemSealSlots[EQUIP_JACK_NUM];
		int nGemSealTemplId[EQUIP_JACK_NUM];
		for (int i=0; i<EQUIP_JACK_NUM; ++i)
		{
			if(m_pImg_Props[i]->GetDataPtr("ptr_CECIvtrItem"))
			{
				nGemSealSlots[i] = m_pImg_Props[i]->GetData();
				nGemSealTemplId[i] = ((CECIvtrItem*)m_pImg_Props[i]->GetDataPtr("ptr_CECIvtrItem"))->GetTemplateID();
			}
			else
			{
				nGemSealSlots[i] = -1;
				nGemSealTemplId[i] = 0;
			}
		}		
		
		GetGameSession()->c2s_CmdNPCSevCustomizeGemSlots(m_pImg_Item->GetData(), iTemplId, nGemSealSlots, nGemSealTemplId);
		OnEventLButtonDown(0, 0, m_pImg_Item);
		OnEventLButtonDown(0, 0, m_pImg_Props[0]);
		OnEventLButtonDown(0, 0, m_pImg_Props[1]);
		OnEventLButtonDown(0, 0, m_pImg_Props[2]);
	}
	return 0;
}

void CDlgEquipJack::UpdateDisplay()
{
	m_iMoney = 0;
	bool bEnable = false;
	int i;
	if( m_pImg_Item->GetDataPtr("ptr_CECIvtrItem") )
	{
		CECIvtrEquip *pEquip = (CECIvtrEquip*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem");
		GetGameUIMan()->SetIvtrInfo(m_pImg_Item, pEquip);
		for(i=0; i<EQUIP_JACK_NUM; i++)
		{
			m_pImg_Jack[i]->FixFrame(pEquip->GetNewEssenceForGem().gem_slot_shape[i]+1);		
			m_pImg_Back[i]->Show(false);
			if (m_pImg_Gfx[i])
				m_pImg_Gfx[i]->Show(false);
			if(m_pImg_BackGem[i])
				m_pImg_BackGem[i]->Show(false);
			if (pEquip->GetNewEssenceForGem().gem_slot_shape[0]>=0)
				m_pLab_Names[i]->SetText(GetGameUIMan()->GetStringFromTable(1550+pEquip->GetNewEssenceForGem().gem_slot_shape[i]));
			else
				m_pLab_Names[i]->SetText(GetGameUIMan()->GetStringFromTable(1549));
			if(pEquip->GetNewEssenceForGem().gem_embeded[i]>0)
			{
				CECIvtrItem* pItem = CECIvtrItem::CreateItem(pEquip->GetNewEssenceForGem().gem_embeded[i], 0, 1);
				if (pItem)
				{
					CECIvtrGem	 *pGem = (CECIvtrGem*)pItem;
					bool bSuit= pGem->GetDBEssence()->shape_type==pEquip->GetNewEssenceForGem().gem_slot_shape[i] ||
						pGem->GetDBEssence()->shape_type==0 ||
						pEquip->GetNewEssenceForGem().gem_slot_shape[i]==0;
					pGem->SetSuitShapeType(bSuit);
					GetGameUIMan()->SetIvtrInfo(m_pImg_Jack[i], pGem);
					if (m_pImg_Gfx[i])
					{
						m_pImg_Gfx[i]->Show(bSuit);
					}
					delete pItem;
				}
			}
			else
			{
				m_pImg_Jack[i]->ClearCover();
				m_pImg_Jack[i]->SetHint(_AL(""));
			}
		}
		if ( 0==stricmp(m_szName, "Win_Accessory_Drill") )
		{
			m_iMoney = GetGameRun()->GetSpecialIDConfig().fee_gem_slot_identify;
			bEnable = pEquip->GetNewEssenceForGem().gem_slot_shape[0]<0;
			for (int i=0; i<EQUIP_JACK_NUM; ++i)
			{
				m_pImg_Back[i]->Show(!bEnable);
			}
		}
		else if ( 0==stricmp(m_szName, "Win_Accessory_ReDrill") )
		{
			m_iMoney = GetGameRun()->GetSpecialIDConfig().fee_gem_slot_rebuild;
			bEnable = pEquip->GetNewEssenceForGem().gem_slot_shape[0]>=0;
			GetDlgItem("Chk_Lock1")->Enable(bEnable);
			GetDlgItem("Chk_Lock2")->Enable(bEnable);
			GetDlgItem("Chk_Lock3")->Enable(bEnable);
			for (int i=0; i<EQUIP_JACK_NUM; ++i)
			{
				if (pEquip->GetNewEssenceForGem().gem_embeded[i]>0)
					m_pImg_BackGem[i]->Show(true);
				else
					m_pImg_Back[i]->Show(true);
			}
		}
		else if ( 0==stricmp(m_szName, "Win_Accessory_Custom") )
		{
			m_iMoney = GetGameRun()->GetSpecialIDConfig().fee_gem_slot_customize;
			for (int i=0; i<EQUIP_JACK_NUM; ++i)
			{
				if (pEquip->GetNewEssenceForGem().gem_embeded[i]>0)
					m_pImg_BackGem[i]->Show(true);
				else
					m_pImg_Back[i]->Show(true);
			}
		}
	}
	else
	{
		m_pImg_Item->ClearCover();
		m_pImg_Item->SetHint(_AL(""));
		for(i=0; i<EQUIP_JACK_NUM; i++)
		{
			m_pImg_Jack[i]->FixFrame(0);
			m_pImg_Jack[i]->ClearCover();
			m_pImg_Jack[i]->SetHint(_AL(""));
			m_pLab_Names[i]->SetText(_AL(""));
			m_pImg_Back[i]->Show(false);
			if(m_pImg_BackGem[i])
				m_pImg_BackGem[i]->Show(false);
			if(m_pImg_Gfx[i])
				m_pImg_Gfx[i]->Show(false);
		}
		if ( 0==stricmp(m_szName, "Win_Accessory_ReDrill") )
		{
			GetDlgItem("Chk_Lock1")->Enable(false);
			GetDlgItem("Chk_Lock2")->Enable(false);
			GetDlgItem("Chk_Lock3")->Enable(false);
		}
		bEnable = false;
	}

	if ( 0==stricmp(m_szName, "Win_Accessory_Custom") )
	{
		bEnable = false;
		m_bEnable = true;
		CECIvtrEquip *pEquip = (CECIvtrEquip*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem");
		for(i=0; i<EQUIP_JACK_NUM; i++)
		{
			if (m_pImg_Props[i]->GetDataPtr("ptr_CECIvtrItem"))
			{
				GetGameUIMan()->SetIvtrInfo(m_pImg_Props[i], (CECIvtrItem*)m_pImg_Props[i]->GetDataPtr("ptr_CECIvtrItem"));
				if(!bEnable&&m_pImg_Item->GetDataPtr("ptr_CECIvtrItem"))
					bEnable = true;

				if (m_bEnable)
				{
					CECIvtrGemSeal* pGemSeal = (CECIvtrGemSeal*)m_pImg_Props[i]->GetDataPtr("ptr_CECIvtrItem");
					for (int j=0; j<EQUIP_JACK_NUM; ++j)
					{
						if (!pEquip||pGemSeal->GetDBEssence()->shape_type==pEquip->GetNewEssenceForGem().gem_slot_shape[j])
						{
							m_bEnable = false;
						}
					}
				}
			}
			else
			{
				m_pImg_Props[i]->ClearCover();
				m_pImg_Props[i]->SetHint(_AL(""));
			}
			m_pImg_BackProp[i]->Show(m_pImg_Item->GetDataPtr("ptr_CECIvtrItem")!=NULL);
		}
	}

	if (!UpdateMoney())
	{
		bEnable = false;
	}

	m_pBtn_Confirm->Enable(bEnable);
}

void CDlgEquipJack::Reset()
{
	if ( 0==stricmp(m_szName, "Win_Accessory_ReDrill") )
	{
		AString strName;
		for (int i=0; i<EQUIP_JACK_NUM; ++i)
		{
			strName.Format("Chk_Lock%d", i+1);
			PAUICHECKBOX pChk = (PAUICHECKBOX)GetDlgItem(strName);
			pChk->Check(false);
			pChk->SetText(GetGameUIMan()->GetStringFromTable(1410));
			pChk->Enable(false);
			strName.Format("Img_Lock%d", i+1);
			GetDlgItem(strName)->Show(false);
			m_pImg_Gfx[i]->Show(false);
		}
	}
	else if ( m_szName=="Win_Accessory_Custom")
	{
		for(int i=0; i<EQUIP_JACK_NUM; i++)
		{
			OnEventLButtonDown(0, 0, m_pImg_Props[i]);
			m_pImg_Gfx[i]->Show(false);
			m_pImg_BackProp[i]->Show(false);
		}
	}
	for(int i=0; i<EQUIP_JACK_NUM; i++)
	{
		if(m_pImg_Back[i])
			m_pImg_Back[i]->Show(false);
		if(m_pImg_BackGem[i])
			m_pImg_BackGem[i]->Show(false);
	}
}


//////////////////////////////////////////////////////////////////////////
// CDlgEquipJack
//////////////////////////////////////////////////////////////////////////


AUI_BEGIN_COMMAND_MAP(CDlgEquipGem, CDlgItemSvrBase)

AUI_ON_COMMAND("Btn_Detach*",			OnCommand_Detach)

AUI_END_COMMAND_MAP()

CDlgEquipGem::CDlgEquipGem()
{
}

CDlgEquipGem::~CDlgEquipGem()
{
}

bool CDlgEquipGem::OnInitDialog()
{
	AString strName;
	for (int i=0; i<EQUIP_JACK_NUM; ++i)
	{
		strName.Format("Img_Hole%d", i+1);
		m_pImg_Jack[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		strName.Format("Lab_Hole%d", i+1);
		m_pLab_Names[i] = (PAUILABEL)GetDlgItem(strName);
		strName.Format("Img_Bg%d", i+1);
		m_pImg_Back[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		strName.Format("Img_Bg%d", i+4);
		m_pImg_BackGem[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		strName.Format("Img_Gfx%d", i+1);
		m_pImg_Gfx[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		if( m_pImg_Gfx[i] )
		{
			m_pImg_Gfx[i]->SetTransparent(true);
			m_pImg_Gfx[i]->SetAcceptMouseMessage(false);
		}
	}
	return CDlgItemSvrBase::OnInitDialog();
}

void CDlgEquipGem::OnTick()
{
	CDlgItemSvrBase::OnTick();
	if ( 0==stricmp(m_szName, "Win_Accessory_Detach") )
	{
		static int gem_embeded[EQUIP_JACK_NUM] = {-1, -1, -1};
		if (m_pImg_Item->GetDataPtr("ptr_CECIvtrItem"))
		{
			CECIvtrEquip *pEquip = (CECIvtrEquip*)GetHostPlayer()->GetPack()->GetItem(m_pImg_Item->GetData());
			if( pEquip != m_pImg_Item->GetDataPtr("ptr_CECIvtrItem")) // 同步包裹信息，更新物品
			{
				m_pImg_Item->SetDataPtr(pEquip, "ptr_CECIvtrItem");
				pEquip->Freeze(true);
				UpdateDisplay();
			}
			if ( gem_embeded[0] != pEquip->GetNewEssenceForGem().gem_embeded[0] ||
				gem_embeded[1] != pEquip->GetNewEssenceForGem().gem_embeded[1] ||
				gem_embeded[2] != pEquip->GetNewEssenceForGem().gem_embeded[2] )
			{
				gem_embeded[0] = pEquip->GetNewEssenceForGem().gem_embeded[0];
				gem_embeded[1] = pEquip->GetNewEssenceForGem().gem_embeded[1];
				gem_embeded[2] = pEquip->GetNewEssenceForGem().gem_embeded[2];
				UpdateDisplay();
			}
		}
	}
}

void CDlgEquipGem::OnCommand_Detach(const char * szCommand)
{
	if (GetHostPlayer()->GetPack()->SearchEmpty()<0)
	{
		GetGameUIMan()->MessageBox(7076);
		return;
	}
	int index = atoi(szCommand+strlen("Btn_Detach"));
	PAUIDIALOG pDlg;
	GetGameUIMan()->MessageBox("Game_Accessory_Detach", GetGameUIMan()->GetStringFromTable(1545), MB_YESNO, 
		A3DCOLORRGBA(255, 255, 255, 160), &pDlg);
	pDlg->SetData(1<<(index-1));	// bool bRemove[EQUIP_JACK_NUM] = {true, true, true};
}

void CDlgEquipGem::SetGemConfirm(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg)
{
	if ( m_szName=="Win_Accessory_Attach" && pImg &&
		strstr(pImg->GetName(), "Img_Hole") &&
		pItem->GetClassID()==CECIvtrItem::ICID_GEM)
	{
		if (!m_pImg_Item->GetDataPtr("ptr_CECIvtrItem"))
			return;
		
		CECIvtrItem *pItemOrg = (CECIvtrItem*)pImg->GetDataPtr("ptr_CECIvtrItem");
		if( pItemOrg )
			pItemOrg->Freeze(false);
		
		pImg->SetData(nSlot);
		pImg->SetDataPtr(pItem, "ptr_CECIvtrItem");
		pItem->Freeze(true);
	}
}

void CDlgEquipGem::SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg)
{
	// 只有饰品可以右键点击包裹，其它需要拖动 pImg 不为空
	if (pImg==m_pImg_Item)
	{
		if (pItem->GetClassID()!=CECIvtrItem::ICID_EQUIP)
			return;

		CECIvtrEquip *pEquip = (CECIvtrEquip*)pItem;
		if (!pEquip->GetDBEssence()->gem_slot_identify)
		{
			return;
		}

		if (pEquip->GetNewEssenceForGem().gem_slot_shape[0]<0)
		{
			if(m_szName=="Win_Accessory_Attach")
				GetGameUIMan()->MessageBox(1535);
			else if(m_szName=="Win_Accessory_Detach")
				GetGameUIMan()->MessageBox(1536);
			return;
		}
		if(m_szName=="Win_Accessory_Detach")
		{
			if (pEquip->GetNewEssenceForGem().gem_embeded[0]<=0 && 
				pEquip->GetNewEssenceForGem().gem_embeded[1]<=0 && 
				pEquip->GetNewEssenceForGem().gem_embeded[2]<=0)
			{
				GetGameUIMan()->MessageBox(1536);
				return;
			}
		}

		CECIvtrItem *pItemOrg = (CECIvtrItem*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem");
		if( pItemOrg )
			pItemOrg->Freeze(false);
		
		m_pImg_Item->SetData(nSlot);
		m_pImg_Item->SetDataPtr(pItem, "ptr_CECIvtrItem");
		pItem->Freeze(true);
	}
	else if ( m_szName=="Win_Accessory_Attach" && pImg &&
		strstr(pImg->GetName(), "Img_Hole") &&
		pItem->GetClassID()==CECIvtrItem::ICID_GEM)
	{
		if (!m_pImg_Item->GetDataPtr("ptr_CECIvtrItem"))
			return;

		CECIvtrEquip *pEquip = (CECIvtrEquip*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem");
		int iSlot = atoi(pImg->GetName() + strlen("Img_Hole"))-1;
		if (pEquip->GetNewEssenceForGem().gem_embeded[iSlot]>=0)//已经镶嵌宝石
			return;

		CECIvtrGem* pGem = (CECIvtrGem*)pItem;
		if (pEquip->GetNewEssenceForGem().gem_slot_shape[iSlot]==pGem->GetDBEssence()->shape_type)
			SetGemConfirm(pItem, nSlot, pImg);
		else
		{
			PAUIDIALOG pDlg;
			GetGameUIMan()->MessageBox("Game_Accessory_Attach_Confirm", GetGameUIMan()->GetStringFromTable(1548), MB_YESNO, 
				A3DCOLORRGBA(255, 255, 255, 160), &pDlg);
			pDlg->SetData(nSlot);
			pDlg->SetDataPtr(pImg);
			return;
		}
	}
	else if (!pImg)
	{
		if (pItem->GetClassID()==CECIvtrItem::ICID_EQUIP)
		{
			SetItem(pItem, nSlot, m_pImg_Item);
		}
		else if (pItem->GetClassID()==CECIvtrItem::ICID_GEM && m_szName=="Win_Accessory_Attach")
		{
			if (!m_pImg_Item->GetDataPtr("ptr_CECIvtrItem"))
				return;

			int iSuitSlot = FindSuitSlot((CECIvtrGem*)pItem);
			if(iSuitSlot>=0)
				SetItem(pItem, nSlot, m_pImg_Jack[iSuitSlot]);
			else
			{
				CECIvtrEquip *pEquip = (CECIvtrEquip*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem");
				for (int i=0; i<EQUIP_JACK_NUM; ++i)
				{
					if (pEquip->GetNewEssenceForGem().gem_embeded[i]<0&&!m_pImg_Jack[i]->GetDataPtr("ptr_CECIvtrItem"))
					{						
						PAUIDIALOG pDlg;
						GetGameUIMan()->MessageBox("Game_Accessory_Attach_Confirm", GetGameUIMan()->GetStringFromTable(1548), MB_YESNO, 
							A3DCOLORRGBA(255, 255, 255, 160), &pDlg);
						pDlg->SetData(nSlot);
						pDlg->SetDataPtr(m_pImg_Jack[i]);
						break;
					}
				}
			}
		}
	}
	else
		return;

	UpdateDisplay();
}

int CDlgEquipGem::FindSuitSlot(CECIvtrGem *pGem)
{
	CECIvtrEquip *pEquip = (CECIvtrEquip*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem");
	assert(pEquip);

	for (int i=0; i<EQUIP_JACK_NUM; ++i)
	{
		if( pEquip->GetNewEssenceForGem().gem_embeded[i]==-1 &&
			pGem->GetDBEssence()->shape_type==pEquip->GetNewEssenceForGem().gem_slot_shape[i] &&
			m_pImg_Jack[i]->GetDataPtr("ptr_CECIvtrItem")==0)
			return i;
	}

	return -1;
}

void CDlgEquipGem::ServerStart()
{
	CECIvtrEquip* pEquip = (CECIvtrEquip*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem");
	int iTemplId = pEquip->GetTemplateID();
	if ( 0==stricmp(m_szName, "Win_Accessory_Attach") )
	{
		int nGemSlots[EQUIP_JACK_NUM];
		int nGemTemplId[EQUIP_JACK_NUM];
		for (int i=0; i<EQUIP_JACK_NUM; ++i)
		{
			if(m_pImg_Jack[i]->GetDataPtr("ptr_CECIvtrItem"))
			{
				nGemSlots[i] = m_pImg_Jack[i]->GetData();
				nGemTemplId[i] = ((CECIvtrItem*)m_pImg_Jack[i]->GetDataPtr("ptr_CECIvtrItem"))->GetTemplateID();
				if (((CECIvtrItem*)m_pImg_Jack[i]->GetDataPtr("ptr_CECIvtrItem"))->IsLocked())
				{
					GetGameUIMan()->MessageBox(7035);
					return;
				}

			}
			else
			{
				nGemSlots[i] = 0;
				nGemTemplId[i] = 0;
			}
		}
		
		GetGameSession()->c2s_CmdNPCSevEmbedGems(m_pImg_Item->GetData(), iTemplId, nGemSlots, nGemTemplId);
		OnEventLButtonDown(0, 0, m_pImg_Item);
		OnEventLButtonDown(0, 0, m_pImg_Jack[0]);
		OnEventLButtonDown(0, 0, m_pImg_Jack[1]);
		OnEventLButtonDown(0, 0, m_pImg_Jack[2]);
	}
	else if ( 0==stricmp(m_szName, "Win_Accessory_Detach") )
	{
		if (GetHostPlayer()->GetPack()->SearchEmpty()<0)
		{
			GetGameUIMan()->MessageBox(7076);
			return;
		}
		PAUIDIALOG pDlg;
		GetGameUIMan()->MessageBox("Game_Accessory_Detach", GetGameUIMan()->GetStringFromTable(1545), MB_YESNO, 
			A3DCOLORRGBA(255, 255, 255, 160), &pDlg);
		pDlg->SetData(7);	// bool bRemove[EQUIP_JACK_NUM] = {true, true, true};
	}
}

void CDlgEquipGem::UpdateDisplay()
{
	m_iMoney = 0;
	bool bEnable = true;
	int i;
	for (i=0; i<EQUIP_JACK_NUM; ++i)
	{
		m_pImg_Back[i]->Show(false);
		m_pImg_BackGem[i]->Show(false);
	}
	if( m_pImg_Item->GetDataPtr("ptr_CECIvtrItem") )
	{
		CECIvtrEquip *pEquip = (CECIvtrEquip*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem");
		GetGameUIMan()->SetIvtrInfo(m_pImg_Item, pEquip);
		for(i=0; i<EQUIP_JACK_NUM; i++)
		{
			if ( 0==stricmp(m_szName, "Win_Accessory_Attach") )
				m_pLab_Names[i]->SetText(GetGameUIMan()->GetStringFromTable(1550+pEquip->GetNewEssenceForGem().gem_slot_shape[i]));
			m_pImg_Jack[i]->FixFrame(pEquip->GetNewEssenceForGem().gem_slot_shape[i]+1);
			if (pEquip->GetNewEssenceForGem().gem_embeded[i]>0)
				m_pImg_BackGem[i]->Show(true);
			else
				m_pImg_Back[i]->Show(true);
			if (m_pImg_Gfx[i])
				m_pImg_Gfx[i]->Show(false);
			if(pEquip->GetNewEssenceForGem().gem_embeded[i]>0)
			{
				CECIvtrItem* pItem = CECIvtrItem::CreateItem(pEquip->GetNewEssenceForGem().gem_embeded[i], 0, 1);
				if (pItem)
				{
					CECIvtrGem	 *pGem = (CECIvtrGem*)pItem;
					bool bSuit= pGem->GetDBEssence()->shape_type==pEquip->GetNewEssenceForGem().gem_slot_shape[i] ||
						pGem->GetDBEssence()->shape_type==0 ||
						pEquip->GetNewEssenceForGem().gem_slot_shape[i]==0;
					pGem->SetSuitShapeType(bSuit);
					GetGameUIMan()->SetIvtrInfo(m_pImg_Jack[i], pGem);
					if (m_pImg_Gfx[i])
					{
						m_pImg_Gfx[i]->Show(bSuit);
					}
					delete pItem;
				}
			}
			else
			{
				m_pImg_Jack[i]->ClearCover();
				m_pImg_Jack[i]->SetHint(_AL(""));
			}
		}

		if ( 0==stricmp(m_szName, "Win_Accessory_Attach") )
		{
			m_iMoney = GetGameRun()->GetSpecialIDConfig().fee_gem_tessellation;
		}
		else if ( 0==stricmp(m_szName, "Win_Accessory_Detach") )
		{
			m_iMoney = GetGameRun()->GetSpecialIDConfig().fee_gem_single_dismantle;
			GetDlgItem("Btn_Detach1")->Enable(pEquip->GetNewEssenceForGem().gem_embeded[0]>0);
			GetDlgItem("Btn_Detach2")->Enable(pEquip->GetNewEssenceForGem().gem_embeded[1]>0);
			GetDlgItem("Btn_Detach3")->Enable(pEquip->GetNewEssenceForGem().gem_embeded[2]>0);
			bEnable = pEquip->GetNewEssenceForGem().gem_embeded[0]>0 ||
				pEquip->GetNewEssenceForGem().gem_embeded[1]>0 ||
				pEquip->GetNewEssenceForGem().gem_embeded[2]>0;
		}
	}
	else
	{
		m_pImg_Item->ClearCover();
		m_pImg_Item->SetHint(_AL(""));
		for(i=0; i<EQUIP_JACK_NUM; i++)
		{
			m_pImg_Jack[i]->FixFrame(0);
			m_pImg_Jack[i]->ClearCover();	
			m_pImg_Jack[i]->SetHint(_AL(""));
			if ( 0==stricmp(m_szName, "Win_Accessory_Attach") )
				m_pLab_Names[i]->SetText(_AL(""));
			m_pImg_Back[i]->Show(false);
			m_pImg_BackGem[i]->Show(false);
			m_pImg_Gfx[i]->Show(false);
		}
		if ( 0==stricmp(m_szName, "Win_Accessory_Detach") )
		{
			GetDlgItem("Btn_Detach1")->Enable(false);
			GetDlgItem("Btn_Detach2")->Enable(false);
			GetDlgItem("Btn_Detach3")->Enable(false);
		}
		bEnable = false;
	}
	
	if ( 0==stricmp(m_szName, "Win_Accessory_Attach") )
	{
		bEnable = false;
		for(i=0; i<EQUIP_JACK_NUM; i++)
		{
			if (m_pImg_Jack[i]->GetDataPtr("ptr_CECIvtrItem"))
			{
				GetGameUIMan()->SetIvtrInfo(m_pImg_Jack[i], (CECIvtrItem*)m_pImg_Jack[i]->GetDataPtr("ptr_CECIvtrItem"));
				if(!bEnable&&m_pImg_Item->GetDataPtr("ptr_CECIvtrItem"))
					bEnable = true;

				CECIvtrEquip *pEquip = (CECIvtrEquip*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem");
				CECIvtrGem	 *pGem = (CECIvtrGem*)m_pImg_Jack[i]->GetDataPtr("ptr_CECIvtrItem");
				if (pEquip)
				{					
					bool bSuit= pGem->GetDBEssence()->shape_type==pEquip->GetNewEssenceForGem().gem_slot_shape[i] ||
						pGem->GetDBEssence()->shape_type==0 ||
						pEquip->GetNewEssenceForGem().gem_slot_shape[i]==0;
					pGem->SetSuitShapeType(bSuit);
					m_pImg_Gfx[i]->Show(bSuit);
				}
			}
		}
	}

	if (!UpdateMoney())
	{
		bEnable = false;
	}
	m_pBtn_Confirm->Enable(bEnable);
}

void CDlgEquipGem::Reset()
{
	if ( m_szName=="Win_Accessory_Attach" )
	{
		for(int i=0; i<EQUIP_JACK_NUM; i++)
		{
			OnEventLButtonDown(0, 0, m_pImg_Jack[i]);
		}
	}
	else if( m_szName=="Win_Accessory_Detach" )
	{		
		GetDlgItem("Btn_Detach1")->Enable(false);
		GetDlgItem("Btn_Detach2")->Enable(false);
		GetDlgItem("Btn_Detach3")->Enable(false);
	}
	for(int i=0; i<EQUIP_JACK_NUM; i++)
	{
		m_pImg_Gfx[i]->Show(false);
		m_pImg_Back[i]->Show(false);
		m_pImg_BackGem[i]->Show(false);
	}
}

int CDlgEquipGem::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="detach_confirm")
	{
		int iTemplId = ((CECIvtrItem*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem"))->GetTemplateID();
		int iRemove = wParam;
		bool bRemove[EQUIP_JACK_NUM] = {(iRemove&1)>0, (iRemove&2)>0, (iRemove&4)>0};
		GetGameSession()->c2s_CmdNPCSevRemoveGems(m_pImg_Item->GetData(), iTemplId, bRemove);
	}
	else if (strMsg=="attach_confirm")
	{
		int nSlot = wParam;
		PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)lParam;
		CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(nSlot);
		
		SetGemConfirm(pItem, nSlot, pImg);
		UpdateDisplay();
	}
	return 0;
}