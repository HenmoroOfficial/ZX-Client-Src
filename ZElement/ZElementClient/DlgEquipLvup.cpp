// Filename	:	DlgEquipLvup.cpp
// Creator	:	Fu Chonggang
// Date		:	Jul 5, 2010

#include "DlgEquipLvup.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIImagePicture.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrConsume.h"
#include "EC_Game.h"
#include "EC_SendC2SCmds.h"
#include "elementdataman.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgEquipLvup, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",				OnCommand_CANCEL)
AUI_ON_COMMAND("depart",				OnCommand_depart)       //装备（首饰除外）升级
AUI_ON_COMMAND("Btn_Start",				OnCommand_JewelryStart) //首饰升级

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgEquipLvup, CDlgBase)

AUI_ON_EVENT("Item_*",		WM_LBUTTONDOWN,			OnEventLButtonDown)
AUI_ON_EVENT("Item_*",		WM_RBUTTONDOWN,			OnEventLButtonDown)

AUI_END_EVENT_MAP()

CDlgEquipLvup::CDlgEquipLvup()
{
}

CDlgEquipLvup::~CDlgEquipLvup()
{
}

bool CDlgEquipLvup::Release()
{
	return CDlgBase::Release();
}

bool CDlgEquipLvup::OnInitDialog()
{
	if(m_szName == "Win_EquipLvup")
	{
		m_pTitle = (AUILabel *)GetDlgItem("Title");
		m_pItem_1 = (AUIImagePicture *)GetDlgItem("Item_1");
		m_pBtn_Confirm = (AUIStillImageButton *)GetDlgItem("Btn_Confirm");
		m_pLab_1 = (AUILabel *)GetDlgItem("Lab_1");
		m_pLab_2 = (AUILabel *)GetDlgItem("Lab_2");
		m_pItem_2 = (AUIImagePicture *)GetDlgItem("Item_2");
		m_pLab_3 = (AUILabel *)GetDlgItem("Lab_3");
		m_pItem_3 = (AUIImagePicture *)GetDlgItem("Item_3");
		m_pItem_1->SetData(-1);
		m_pItem_2->SetData(-1);
		m_pItem_3->SetData(-1);
    }
	else if(m_szName == "Win_Equip_JewelUp")
	{
		m_pBtn_Confirm = (AUIStillImageButton *)GetDlgItem("Btn_Start");
		m_pItem_1 = (AUIImagePicture *)GetDlgItem("Item_01");
		m_pItem_2 = (AUIImagePicture *)GetDlgItem("Item_02");
		m_pItem_1->SetData(-1);
		m_pItem_2->SetData(-1);
	}

	m_nIdProps = 0;
	m_nIdHelper = 0;
	LoadConfigData();
	return true;
}

void CDlgEquipLvup::OnShowDialog()
{
	if (m_pItem_1->GetDataPtr())
	{
		((CECIvtrItem*)m_pItem_1->GetDataPtr())->Freeze(true);
	}
	if (m_pItem_2->GetDataPtr())
	{
		((CECIvtrItem*)m_pItem_2->GetDataPtr())->Freeze(true);
	}
	if(m_szName == "Win_EquipLvup")
	{ 
		if (m_pItem_3->GetDataPtr())
		{
			((CECIvtrItem*)m_pItem_3->GetDataPtr())->Freeze(true);
		}
	}
}

void CDlgEquipLvup::OnTick()
{
	m_pBtn_Confirm->Enable(m_pItem_1->GetDataPtr()&&m_pItem_2->GetDataPtr());
	return;
	CECInventory* pInventory = GetHostPlayer()->GetPack();
	if (m_pItem_1->GetDataPtr())
	{
		int index = m_pItem_1->GetData();
		if (pInventory->GetItem(index)==NULL)
		{
			OnEventLButtonDown(0, 0, m_pItem_1);
			OnEventLButtonDown(0, 0, m_pItem_2);
			if(m_szName == "Win_EquipLvup")
			  OnEventLButtonDown(0, 0, m_pItem_3);
		}
		else
			GetGameUIMan()->SetIvtrInfo(m_pItem_1, pInventory->GetItem(index));
	}
	if (m_pItem_2->GetDataPtr())
	{
		if (m_pItem_1->GetDataPtr()==NULL)
		{
			OnEventLButtonDown(0, 0, m_pItem_2);
			return;
		}
		
		int index = m_pItem_2->GetData();
		if (pInventory->GetItem(index)==NULL)
		{
			int propsIndex = pInventory->FindItem(m_nIdProps);
			if (propsIndex>=0)
			{
				m_pItem_2->SetData(propsIndex);
				m_pItem_2->SetDataPtr(pInventory->GetItem(propsIndex));
				GetGameUIMan()->SetIvtrInfo(m_pItem_2, pInventory->GetItem(propsIndex));
			}
			else
			{
				OnEventLButtonDown(0, 0, m_pItem_2);
			}
		}
	}
	if(m_szName == "Win_EquipLvup")
	{
		if (m_pItem_3->GetDataPtr())
		{
			if (m_pItem_1->GetDataPtr()==NULL )
			{
				OnEventLButtonDown(0, 0, m_pItem_3);
				return;
			}
			
			int index = m_pItem_3->GetData();
			if (pInventory->GetItem(index)==NULL)
			{
				int helperIndex = pInventory->FindItem(m_nIdHelper);
				if (helperIndex>=0)
				{
					m_pItem_3->SetData(helperIndex);
					m_pItem_3->SetDataPtr(pInventory->GetItem(helperIndex));
					GetGameUIMan()->SetIvtrInfo(m_pItem_3, pInventory->GetItem(helperIndex));
				}
				else
				{
					OnEventLButtonDown(0, 0, m_pItem_3);
				}
			}
		}
	}
}

bool CDlgEquipLvup::Render()
{
	if( !CDlgBase::Render() )
		return false;
	return true;
}

int CDlgEquipLvup::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg == "set_equip")
	{
		//wParam 包裹栏位置
		CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(wParam);
		if (pItem->GetExpireDate()!=0)
		{
			return 1;
		}
		int idItem = pItem->GetTemplateID();
		UINT nSize = m_vecInfo.size();
		for (UINT i=0; i<nSize; i++)
		{
			if (idItem == m_vecInfo[i].equip_id)
			{
                if (pItem->GetClassID() == CECIvtrItem::ICID_EQUIP	&&
					(((CECIvtrEquip *)pItem)->GetDBEssence()->equip_type == 2) &&
					((CECIvtrEquip *)pItem)->GetDBEssence()->equip_mask & (EQUIP_MASK_NECK  |
					EQUIP_MASK_FINGER1|
					EQUIP_MASK_FINGER2))
				{
				        if(m_szName == "Win_EquipLvup")
							return 1;
				}
				else
				{
					if(m_szName == "Win_Equip_JewelUp")
							return 1;
				}
				//简单起见，当更改当前装备时，将其余的格子全部清空，以保证其对应性
                OnEventLButtonDown(0, 0, m_pItem_1);
				GetGameUIMan()->SetIvtrInfo(m_pItem_1, pItem);
				m_pItem_1->SetData(wParam);
				if (m_pItem_1->GetDataPtr())
				{
					((CECIvtrItem*)m_pItem_1->GetDataPtr())->Freeze(false);
				}
				m_pItem_1->SetDataPtr(pItem);
				pItem->Freeze(true);
				return 0;
			}
		}
		
		return 1;
	}
	else if (strMsg == "set_props")
	{
		if (m_pItem_1->GetDataPtr()==NULL)
		{
			if(m_szName == "Win_EquipLvup")
			{
			   GetGameUIMan()->MessageBox(3650);
			}
			else if(m_szName == "Win_Equip_JewelUp")
			{
			   GetGameUIMan()->MessageBox(3658);
			}
			return 1;
		}
		
		int idItem = ((CECIvtrItem*)m_pItem_1->GetDataPtr())->GetTemplateID();
		CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(wParam);
		int idProps = pItem->GetTemplateID();

		
		UINT nSize = m_vecInfo.size();
		UINT i;
		for (i=0; i<nSize; i++)
		{
			if (idItem == m_vecInfo[i].equip_id)
			{
				if (idProps==m_vecInfo[i].stone_id)
				{
                     if(pItem->GetCount() < m_vecInfo[i].num)
					 {
						 if(m_szName == "Win_EquipLvup")
						 {
					         GetGameUIMan()->MessageBox(3657);
						 }
						 else if(m_szName == "Win_Equip_JewelUp")
						 {
							 GetGameUIMan()->MessageBox(3660);
						 }
						return 1;
					 }
				     break;
				}
				else
				{
					if(m_szName == "Win_EquipLvup")
					{
						GetGameUIMan()->MessageBox(3651);
					}
					else if(m_szName == "Win_Equip_JewelUp")
					{
						GetGameUIMan()->MessageBox(3659);
					}
					return 1;
				}
			}
		}
		if (i==nSize)
			return 1;

		pItem->Freeze(true);
		m_pItem_2->SetData(wParam);
		if (m_pItem_2->GetDataPtr())
		{
			((CECIvtrItem*)m_pItem_2->GetDataPtr())->Freeze(false);
		}
		m_pItem_2->SetDataPtr(pItem);
		GetGameUIMan()->SetIvtrInfo(m_pItem_2, pItem);
		m_nIdProps = pItem->GetTemplateID();
	}
	else if (strMsg == "set_helper")
	{
		if (m_pItem_1->GetDataPtr()==NULL)
		{
			GetGameUIMan()->MessageBox(3650);
			return 1;
		}
		CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(wParam);
		if (pItem->GetClassID() != CECIvtrItem::ICID_UPGRADE_EQUIP_STONE)
		{
			GetGameUIMan()->MessageBox(3652);
			return 1;
		}
		const UPGRADE_EQUIP_STONE_ESSENCE* conf	= ((CECIvtrUpgradeEquipStone*)pItem)->GetDBEssence();
		CECIvtrEquip* pEquip = (CECIvtrEquip*)m_pItem_1->GetDataPtr();

		if (conf->level > 10) // 2012.9月版本开始, 策划要求10级以上混元天晶不能用
			return 1;
		
		WORD num_estone = pEquip->GetEssence().num_estone;
		if ( conf->type==1 && (num_estone > conf->level && conf->level < 10) ) // 精炼保留道具10级封顶
		{
			GetGameUIMan()->MessageBox(3653);
			return 1;
		}

		pItem->Freeze(true);
		m_pItem_3->SetData(wParam);
		if (m_pItem_3->GetDataPtr())
		{
			((CECIvtrItem*)m_pItem_3->GetDataPtr())->Freeze(false);
		}
		m_pItem_3->SetDataPtr(pItem);
		GetGameUIMan()->SetIvtrInfo(m_pItem_3, pItem);
		m_nIdHelper = pItem->GetTemplateID();
	}
	else if (strMsg == "lv_up")
	{
		c2s_SendCmdNPCSevUpgradeEquip(m_pItem_1->GetData(), ((CECIvtrItem*)m_pItem_1->GetDataPtr())->GetTemplateID(), m_pItem_2->GetData(),m_pItem_3->GetData());
		
		OnEventLButtonDown(0, 0, m_pItem_1);
	}
	return 0;
}

void CDlgEquipLvup::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
	GetGameUIMan()->GetDialog("Win_Inventory")->Show(false);
	GetGameUIMan()->EndNPCService();
	OnEventLButtonDown(0, 0, m_pItem_1);
	GetGameUIMan()->UnfreezeAllItems();
}

void CDlgEquipLvup::OnCommand_depart(const char * szCommand)
{
	if (m_pItem_3->GetData()==-1)
	{
		GetGameUIMan()->MessageBox("Game_EquipLvUp1", GetGameUIMan()->GetStringFromTable(3655), MB_YESNO);
		return;
	}

	const UPGRADE_EQUIP_STONE_ESSENCE* conf	= ((CECIvtrUpgradeEquipStone*)m_pItem_3->GetDataPtr())->GetDBEssence();
	bool bHelpItemBound = !!(conf->proc_type & (1 << 4));
	bool bEquipBound = !!(((CECIvtrEquip*)m_pItem_1->GetDataPtr())->GetStateFlag() & ITEM_STATE_BINDED);
	if (bHelpItemBound && !bEquipBound) // 绑定型的精炼保留道具与非绑定装备
	{
		GetGameUIMan()->MessageBox("Game_EquipLvUpBind", GetGameUIMan()->GetStringFromTable(16028), MB_YESNO);
		return;
	}

	SendLvupCmd();
}

void CDlgEquipLvup::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)pObj;
	if (pObj->GetDataPtr())
	{
		((CECIvtrItem*)pObj->GetDataPtr())->Freeze(false);
	}
	pImg->SetCover(NULL, -1, 0);
	pImg->SetHint(_AL(""));
	pImg->SetData(-1);
	pImg->SetDataPtr(NULL);

	if (pObj==m_pItem_1)
	{
		OnEventLButtonDown(0, 0, m_pItem_2);
		if(m_szName == "Win_EquipLvup")
		OnEventLButtonDown(0, 0, m_pItem_3);
	}
	else if (pObj==m_pItem_2)
	{
		m_nIdProps = 0;
	}
	else if (pObj==m_pItem_3)
	{
		m_nIdHelper = 0;
	}
}

void CDlgEquipLvup::SendLvupCmd()
{
	c2s_SendCmdNPCSevUpgradeEquip(m_pItem_1->GetData(), ((CECIvtrItem*)m_pItem_1->GetDataPtr())->GetTemplateID(), m_pItem_2->GetData(),m_pItem_3->GetData());
	
	OnEventLButtonDown(0, 0, m_pItem_1);

}

void CDlgEquipLvup::LoadConfigData()
{
	m_vecInfo.clear();
	elementdataman* pDB = GetGame()->GetElementDataMan();
	DATA_TYPE DataType = DT_INVALID;
	
	int tid = pDB->get_first_data_id(ID_SPACE_CONFIG, DataType);
	while (tid)
	{
		const void* pData = pDB->get_data_ptr(tid, ID_SPACE_CONFIG, DataType);
		
		if (DataType == DT_UPGRADE_EQUIP_CONFIG)
		{
			const UPGRADE_EQUIP_CONFIG* pConf = (const UPGRADE_EQUIP_CONFIG*)pData;
			
			for (int i=0; i < 200; i++)
			{
				if (pConf->upgrade_config[i].equip_id>0)
				{
					EquipLvUpConfig info;
					info.equip_id = pConf->upgrade_config[i].equip_id;
					info.stone_id = pConf->upgrade_config[i].stone_id;
					info.num      = pConf->upgrade_config[i].num;
					info.out_equip_id = pConf->upgrade_config[i].out_equip_id;
					
					m_vecInfo.push_back(info);
				}
			}
		}
		
		//	Get next item
		tid = pDB->get_next_data_id(ID_SPACE_CONFIG, DataType);
	}
}
void CDlgEquipLvup::OnCommand_JewelryStart(const char * szCommand)
{
	c2s_SendCmdNPCSevUpgradeEquip(m_pItem_1->GetData(), ((CECIvtrItem*)m_pItem_1->GetDataPtr())->GetTemplateID(), m_pItem_2->GetData(),-1);
	
	OnEventLButtonDown(0, 0, m_pItem_1);
}
