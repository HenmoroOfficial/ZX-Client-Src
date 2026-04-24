// Filename	: DlgAttriUp.cpp
// Creator	: WuWeixin
// Date		: 2013/2/22

#include "DlgAttriUp.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIProgress.h"
#include "AUI\\AUIDef.h"
#include "elementdataman.h"
#include "ExpTypes.h"
#include "EC_Global.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_IvtrConsume.h"
#include "EC_HostPlayer.h"
#include "EC_Game.h"
#include "DlgInventory.h"
#include "A2DSprite.h"

////////////////////////////////////////////////////////////////////////////////////////
// CDlgAttriUpNature


AUI_BEGIN_COMMAND_MAP(CDlgAttriUpNature, CDlgBase)
AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgAttriUpNature, CDlgBase)
AUI_END_EVENT_MAP()

enum
{
	PROP_ADD_CONFIG_ID = 744, // 基础属性增益配置id
};

CDlgAttriUpNature::CDlgAttriUpNature()
{
}

CDlgAttriUpNature::~CDlgAttriUpNature()
{
}

bool CDlgAttriUpNature::OnInitDialog()
{
	CDlgBase::OnInitDialog();

	m_propValues[PROP_ADD_DAMAGE] = (AUILabel*)GetDlgItem("Txt_01");
	m_propValues[PROP_ADD_DEF] = (AUILabel*)GetDlgItem("Txt_04");
	m_propValues[PROP_ADD_HP] = (AUILabel*)GetDlgItem("Txt_02");
	m_propValues[PROP_ADD_MP] = (AUILabel*)GetDlgItem("Txt_03");
	m_propValues[PROP_ADD_ATTACK] = (AUILabel*)GetDlgItem("Txt_05");
	m_propValues[PROP_ADD_ARMOR] = (AUILabel*)GetDlgItem("Txt_06");
	m_propValues[PROP_ADD_CRIT_RATE] = (AUILabel*)GetDlgItem("Txt_07");
	m_propValues[PROP_ADD_CRIT_ANTI] = (AUILabel*)GetDlgItem("Txt_09");
	m_propValues[PROP_ADD_CRIT_DAMAGE] = (AUILabel*)GetDlgItem("Txt_08");
	m_propValues[PROP_ADD_CRIT_DAMAGE_ANTI] = (AUILabel*)GetDlgItem("Txt_10");
	m_propValues[PROP_ADD_SKILL_ATTACK_RATE] = (AUILabel*)GetDlgItem("Txt_11");
	m_propValues[PROP_ADD_SKILL_ARMOR_RATE] = (AUILabel*)GetDlgItem("Txt_12");

	m_propPercents[PROP_ADD_DAMAGE] = (AUILabel*)GetDlgItem("Txt_Prgs01");
	m_propPercents[PROP_ADD_DEF] = (AUILabel*)GetDlgItem("Txt_Prgs04");
	m_propPercents[PROP_ADD_HP] = (AUILabel*)GetDlgItem("Txt_Prgs02");
	m_propPercents[PROP_ADD_MP] = (AUILabel*)GetDlgItem("Txt_Prgs03");
	m_propPercents[PROP_ADD_ATTACK] = (AUILabel*)GetDlgItem("Txt_Prgs05");
	m_propPercents[PROP_ADD_ARMOR] = (AUILabel*)GetDlgItem("Txt_Prgs06");
	m_propPercents[PROP_ADD_CRIT_RATE] = (AUILabel*)GetDlgItem("Txt_Prgs07");
	m_propPercents[PROP_ADD_CRIT_ANTI] = (AUILabel*)GetDlgItem("Txt_Prgs09");
	m_propPercents[PROP_ADD_CRIT_DAMAGE] = (AUILabel*)GetDlgItem("Txt_Prgs08");
	m_propPercents[PROP_ADD_CRIT_DAMAGE_ANTI] = (AUILabel*)GetDlgItem("Txt_Prgs10");
	m_propPercents[PROP_ADD_SKILL_ATTACK_RATE] = (AUILabel*)GetDlgItem("Txt_Prgs11");
	m_propPercents[PROP_ADD_SKILL_ARMOR_RATE] = (AUILabel*)GetDlgItem("Txt_Prgs12");

	m_propProgress[PROP_ADD_DAMAGE] = (AUIProgress*)GetDlgItem("Prgs_01");
	m_propProgress[PROP_ADD_DEF] = (AUIProgress*)GetDlgItem("Prgs_04");
	m_propProgress[PROP_ADD_HP] = (AUIProgress*)GetDlgItem("Prgs_02");
	m_propProgress[PROP_ADD_MP] = (AUIProgress*)GetDlgItem("Prgs_03");
	m_propProgress[PROP_ADD_ATTACK] = (AUIProgress*)GetDlgItem("Prgs_05");
	m_propProgress[PROP_ADD_ARMOR] = (AUIProgress*)GetDlgItem("Prgs_06");
	m_propProgress[PROP_ADD_CRIT_RATE] = (AUIProgress*)GetDlgItem("Prgs_07");
	m_propProgress[PROP_ADD_CRIT_ANTI] = (AUIProgress*)GetDlgItem("Prgs_09");
	m_propProgress[PROP_ADD_CRIT_DAMAGE] = (AUIProgress*)GetDlgItem("Prgs_08");
	m_propProgress[PROP_ADD_CRIT_DAMAGE_ANTI] = (AUIProgress*)GetDlgItem("Prgs_10");
	m_propProgress[PROP_ADD_SKILL_ATTACK_RATE] = (AUIProgress*)GetDlgItem("Prgs_11");
	m_propProgress[PROP_ADD_SKILL_ARMOR_RATE] = (AUIProgress*)GetDlgItem("Prgs_12");

	m_propPreviewProgress[PROP_ADD_DAMAGE] = (AUIProgress*)GetDlgItem("Prgs_A01");
	m_propPreviewProgress[PROP_ADD_DEF] = (AUIProgress*)GetDlgItem("Prgs_A04");
	m_propPreviewProgress[PROP_ADD_HP] = (AUIProgress*)GetDlgItem("Prgs_A02");
	m_propPreviewProgress[PROP_ADD_MP] = (AUIProgress*)GetDlgItem("Prgs_A03");
	m_propPreviewProgress[PROP_ADD_ATTACK] = (AUIProgress*)GetDlgItem("Prgs_A05");
	m_propPreviewProgress[PROP_ADD_ARMOR] = (AUIProgress*)GetDlgItem("Prgs_A06");
	m_propPreviewProgress[PROP_ADD_CRIT_RATE] = (AUIProgress*)GetDlgItem("Prgs_A07");
	m_propPreviewProgress[PROP_ADD_CRIT_ANTI] = (AUIProgress*)GetDlgItem("Prgs_A09");
	m_propPreviewProgress[PROP_ADD_CRIT_DAMAGE] = (AUIProgress*)GetDlgItem("Prgs_A08");
	m_propPreviewProgress[PROP_ADD_CRIT_DAMAGE_ANTI] = (AUIProgress*)GetDlgItem("Prgs_A10");
	m_propPreviewProgress[PROP_ADD_SKILL_ATTACK_RATE] = (AUIProgress*)GetDlgItem("Prgs_A11");
	m_propPreviewProgress[PROP_ADD_SKILL_ARMOR_RATE] = (AUIProgress*)GetDlgItem("Prgs_A12");


	elementdataman* pDataMan = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	const void* pData = pDataMan->get_data_ptr(PROP_ADD_CONFIG_ID, ID_SPACE_CONFIG, DataType);
	if (pData && DataType == DT_PROP_ADD_CONFIG)
	{
		int limit = 0;
		PROP_ADD_CONFIG* pConfig = (PROP_ADD_CONFIG*)pData;
		for (int i = 0; i <= PROP_ADD_ATTR_COUNT; ++ i)
		{
			limit = pConfig->prop_add_limit[i];
			if (limit > 0)
			{
				m_propProgress[i]->SetRangeMax(limit);
				m_propPreviewProgress[i]->SetRangeMax(limit);
			}
		}
	}
	else
	{
		ASSERT(FALSE);
	}

	m_idPreviewItem = 0;
	
	return true;
}

void CDlgAttriUpNature::OnShowDialog()
{
	UpdatePropAddProgress();
}

void CDlgAttriUpNature::OnHideDialog()
{
	m_idPreviewItem = 0;
}

// 预览吃丹后属性
void CDlgAttriUpNature::PreviewPropAddItem(int id)
{
	if (id != m_idPreviewItem)
	{
		m_idPreviewItem = id;
		UpdatePropAddProgress();
	}
}

// 更新进度显示, m_idPreviewItem为0时紫条不显示, 不为0时, 紫条显示吃丹后属性增长情况
void CDlgAttriUpNature::UpdatePropAddProgress()
{
	if (!IsShow())
		return;

	PROP_ADD_ITEM_ESSENCE* pPreviewItemEssence = NULL;
	if (m_idPreviewItem > 0)
	{
		elementdataman* pDataMan = g_pGame->GetElementDataMan();
		DATA_TYPE DataType;
		const void* pData = pDataMan->get_data_ptr(m_idPreviewItem, ID_SPACE_ESSENCE, DataType);
		if (pData && DataType == DT_PROP_ADD_ITEM_ESSENCE)
		{
			pPreviewItemEssence = (PROP_ADD_ITEM_ESSENCE*)pData;
		}
	}

	CECHostPlayer* pHostPlayer = GetHostPlayer();
	ACString szText;
	unsigned int i;
	for ( i = 0; i <= PROP_ADD_ARMOR; ++ i)
	{
		SetIntValue(i, pHostPlayer->GetPropAddValue(i), 
			pPreviewItemEssence ? pPreviewItemEssence->prop_add[i] : 0);
	}
	for (; i < PROP_ADD_ATTR_COUNT; ++ i)
	{
		SetFloatValue(i, pHostPlayer->GetPropAddValue(i), 
			pPreviewItemEssence ? pPreviewItemEssence->prop_add[i] : 0);
	}
}

// 设置整型属性
void CDlgAttriUpNature::SetIntValue(int type, int value, int preadd)
{
		ACString szText;
		szText.Format(_AL("%d"), value);
		m_propValues[type]->SetText(szText);
		
		szText.Format(_AL("%d/%d"), value, m_propProgress[type]->GetRangeMax());
		m_propPercents[type]->SetText(szText);

		m_propProgress[type]->SetProgress(value);

		if (preadd > 0 && value + preadd < m_propProgress[type]->GetRangeMax())
		{
			m_propPreviewProgress[type]->Show(true);
			m_propPreviewProgress[type]->SetProgress(value + preadd);
		}
		else
		{
			m_propPreviewProgress[type]->Show(false);
		}
}

// 设置浮点型属性(value、preadd均为千分数整型表示)
void CDlgAttriUpNature::SetFloatValue(int type, int value, int preadd)
{
		ACString szText;
		if (type == PROP_ADD_SKILL_ATTACK_RATE || type == PROP_ADD_SKILL_ARMOR_RATE)
			szText.Format(_AL("%.1f"), value / 10.0f);
		else
			szText.Format(_AL("%.1f%%"), value / 10.0f);
		m_propValues[type]->SetText(szText);
		
		if (type == PROP_ADD_SKILL_ATTACK_RATE || type == PROP_ADD_SKILL_ARMOR_RATE)
			szText.Format(_AL("%.1f/%.1f"), value / 10.0f, m_propProgress[type]->GetRangeMax() / 10.0f);
		else
			szText.Format(_AL("%.1f%%/%.1f%%"), value / 10.0f, m_propProgress[type]->GetRangeMax() / 10.0f);
		m_propPercents[type]->SetText(szText);

		m_propProgress[type]->SetProgress(value);

		if (preadd > 0 && value + preadd < m_propProgress[type]->GetRangeMax())
		{
			m_propPreviewProgress[type]->Show(true);
			m_propPreviewProgress[type]->SetProgress(value + preadd);
		}
		else
		{
			m_propPreviewProgress[type]->Show(false);
		}
}

////////////////////////////////////////////////////////////////////////////////////////
// CDlgAttriUpNature

AUI_BEGIN_COMMAND_MAP(CDlgAttriUpEnchase, CDlgBase)
AUI_ON_COMMAND("Btn_Confirm",	OnCommand_Confirm)
AUI_ON_COMMAND("Btn_Cancel",	OnCommand_CANCEL)
AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgAttriUpEnchase, CDlgBase)
AUI_ON_EVENT("Img_Item",		WM_LBUTTONDOWN,			OnEventLButtonDown)
AUI_ON_EVENT("Img_Item",		WM_RBUTTONDOWN,			OnEventLButtonDown)
AUI_END_EVENT_MAP()


CDlgAttriUpEnchase::CDlgAttriUpEnchase()
{
}

CDlgAttriUpEnchase::~CDlgAttriUpEnchase()
{
}

bool CDlgAttriUpEnchase::OnInitDialog()
{
	CDlgBase::OnInitDialog();

	m_pMaterial = (AUIImagePicture *)GetDlgItem("Img_Item");
	m_pBtnConfirm = (AUIStillImageButton *)GetDlgItem("Btn_Confirm");
	m_nNeedForgeCount = 0;
	m_Forges.clear();
	m_idForge = 0;
	m_nForgeIndex = 0;

	return true;
}

void CDlgAttriUpEnchase::OnShowDialog()
{
	if (m_nNeedForgeCount <= 0)
	{
		ASSERT(FALSE);
		Show(false);
		return;
	}
	m_pBtnConfirm->Enable(false);
	CDlgAttriUp* pDlgAttriUp = (CDlgAttriUp*)GetGameUIMan()->GetDialog("Win_AttriUp");
	if (pDlgAttriUp->GetForgeItemId() > 0)
		CommonMessage("set_forge", pDlgAttriUp->GetForgeItemIndex(), 0);
}

void CDlgAttriUpEnchase::OnHideDialog()
{
	Clear();
}

// 放置重炼材料
int CDlgAttriUpEnchase::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg == "set_forge")
	{
		//wParam 包裹栏位置
		CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(wParam);
		if (pItem && pItem->GetClassID() == CECIvtrItem::ICID_TASKNMMATTER)
		{
			bool bMatch = false;
			int id = pItem->GetTemplateID();
			for (abase::vector<int>::iterator it = m_Forges.begin(), ite = m_Forges.end(); it != ite; ++ it)
			{
				if (id == *it)
				{
					bMatch = true;
					break;
				}
			}
			if (!bMatch)
			{
				ErrorForgeMsg();
				return 0;
			}

			CECIvtrPropAddMaterial* pMaterial = (CECIvtrPropAddMaterial*)pItem;
			GetGameUIMan()->SetIvtrInfo(m_pMaterial, pItem);
			m_pMaterial->SetData(wParam);
			if (m_pMaterial->GetDataPtr())
			{
				((CECIvtrItem*)m_pMaterial->GetDataPtr())->Freeze(false);
			}
			m_pMaterial->SetDataPtr(pItem);
			pItem->Freeze(true);

			m_nForgeIndex = wParam;
			m_idForge = pItem->GetTemplateID();
			int nForgeCount = min(pItem->GetCount(), m_nNeedForgeCount);

			ACString szText;
			szText.Format(_AL("%d/%d"), nForgeCount, m_nNeedForgeCount);
			m_pMaterial->SetText(szText);

			m_pBtnConfirm->Enable(nForgeCount >= m_nNeedForgeCount);

			return 0;
		}
		ErrorForgeMsg();
		return 1;
	}
	return 1;
}

void CDlgAttriUpEnchase::ErrorForgeMsg()
{
	elementdataman* pDataMan = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	const void* pData;
	abase::vector<ACString> names;
	for (abase::vector<int>::iterator it = m_Forges.begin(), ite = m_Forges.end(); it != ite; ++ it)
	{
		pData = pDataMan->get_data_ptr(*it, ID_SPACE_ESSENCE, DataType);
		if (!pData || DataType != DT_TASKNORMALMATTER_ESSENCE)
			continue;

		TASKNORMALMATTER_ESSENCE* pEssence = (TASKNORMALMATTER_ESSENCE*)pData;
		names.push_back(pEssence->name);
	}

	ACString szText1 = _AL("");
	if (names.size() == 1)
	{
		szText1.Format(GetStringFromTable(17700), names[0]);
		GetGameUIMan()->MessageBox("", szText1, MB_OK);
	}
	else if (names.size() == 2)
	{
		ACString szText1 = _AL("");
		ACString szText2 = _AL("");
		szText1.Format(GetStringFromTable(17702), names[0], names[1]);
		szText2.Format(GetStringFromTable(17700), szText1);
		GetGameUIMan()->MessageBox("", szText2, MB_OK);
	}
	else if (names.size() > 3)
	{
		ACString szText1 = _AL("");
		ACString szText2 = _AL("");
		szText1.Format(GetStringFromTable(17701), names[0], names[1], names[2]);
		szText2.Format(GetStringFromTable(17700), szText1);
		GetGameUIMan()->MessageBox("", szText2, MB_OK);
	}
}

void CDlgAttriUpEnchase::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if (m_idForge == 0)
		return;

	Clear();
	m_idForge = 0;
	m_nForgeIndex = 0;
	m_pBtnConfirm->Enable(false);
}

void CDlgAttriUpEnchase::OnCommand_Confirm(const char * szCommand)
{
	CDlgAttriUp* pDlgAttriUp = (CDlgAttriUp*)GetGameUIMan()->GetDialog("Win_AttriUp");
	pDlgAttriUp->SetForgeItem(m_idForge, m_nForgeIndex);
	Show(false);
	pDlgAttriUp->OnCommand_Start(NULL);
}

// 设置所需材料类型和数量
void CDlgAttriUpEnchase::SetForgesAndCount(const int* idMaterials, int nLen, int nNeedCount)
{
	m_nNeedForgeCount = nNeedCount;
	m_Forges.clear();
	for (int i = 0; i < nLen; ++ i)
	{
		m_Forges.push_back(idMaterials[i]);
	}
}

// 清除玩家操作
void CDlgAttriUpEnchase::Clear()
{
	PAUIIMAGEPICTURE pImg = m_pMaterial;
	if (pImg->GetDataPtr())
	{
		((CECIvtrItem*)pImg->GetDataPtr())->Freeze(false);
	}
	pImg->SetCover(NULL, -1, 0);
	pImg->SetHint(_AL(""));
	pImg->SetData(-1);
	pImg->SetDataPtr(NULL);
	pImg->SetText(_AL(""));

	m_pBtnConfirm->Enable(false);
}


////////////////////////////////////////////////////////////////////////////////////////
// CDlgAttriUp

AUI_BEGIN_COMMAND_MAP(CDlgAttriUp, CDlgBase)
AUI_ON_COMMAND("Btn_Start",			OnCommand_Start)
AUI_ON_COMMAND("Btn_Stop",			OnCommand_Stop)
AUI_ON_COMMAND("Btn_Retry",			OnCommand_Retry)
AUI_ON_COMMAND("Btn_Get",			OnCommand_Get)
AUI_ON_COMMAND("Btn_Expand",		OnCommand_Expand)
AUI_ON_COMMAND("Btn_Help",			OnCommand_Help)
AUI_END_COMMAND_MAP()



AUI_BEGIN_EVENT_MAP(CDlgAttriUp, CDlgBase)
AUI_ON_EVENT("Item_00",		WM_LBUTTONDOWN,			OnEventLButtonDown)
AUI_ON_EVENT("Item_00",		WM_RBUTTONDOWN,			OnEventLButtonDown)
AUI_END_EVENT_MAP()


enum
{
	ItemType_Null = 0,
	ItemType_PropAddItem,
	ItemType_PropAddMaterial
};

enum
{
	PROP_ADD_GEN_WHEEL_INTERVAL = 50, // 50ms切换一次
};

CDlgAttriUp::CDlgAttriUp()
{
}

CDlgAttriUp::~CDlgAttriUp()
{
}

bool CDlgAttriUp::OnInitDialog()
{
	CDlgBase::OnInitDialog();

	m_pItem0 = (AUIImagePicture *)GetDlgItem("Item_00");
	m_pItemProduce[0] = (AUIImagePicture *)GetDlgItem("Item_01");
	m_pItemProduce[1] = (AUIImagePicture *)GetDlgItem("Item_02");
	m_pItemProduce[2] = (AUIImagePicture *)GetDlgItem("Item_03");
	m_pItemProduce[3] = (AUIImagePicture *)GetDlgItem("Item_04");
	m_pItemProduce[4] = (AUIImagePicture *)GetDlgItem("Item_05");
	m_pItemProduce[5] = (AUIImagePicture *)GetDlgItem("Item_06");
	m_pImgHover[0] = (AUIImagePicture *)GetDlgItem("Img_Hover01");
	m_pImgHover[1] = (AUIImagePicture *)GetDlgItem("Img_Hover02");
	m_pImgHover[2] = (AUIImagePicture *)GetDlgItem("Img_Hover03");
	m_pImgHover[3] = (AUIImagePicture *)GetDlgItem("Img_Hover04");
	m_pImgHover[4] = (AUIImagePicture *)GetDlgItem("Img_Hover05");
	m_pImgHover[5] = (AUIImagePicture *)GetDlgItem("Img_Hover06");
	m_pBtn_Start = (AUIStillImageButton *)GetDlgItem("Btn_Start");
	m_pBtn_Stop = (AUIStillImageButton *)GetDlgItem("Btn_Stop");
	m_pBtn_Retry = (AUIStillImageButton *)GetDlgItem("Btn_Retry");
	m_pBtn_Get = (AUIStillImageButton *)GetDlgItem("Btn_Get");

	m_pSpriteCover = NULL;

	if( !m_pSpriteCover )
	{
		m_pSpriteCover = new A2DSprite;
		if( !m_pSpriteCover ) return AUI_ReportError(__LINE__, __FILE__);

		bool bval = m_pSpriteCover->Init(m_pA3DDevice, "Icon\\光效.tga", 0);
		if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
		m_pSpriteCover->SetLinearFilter(true);
	}


	return true;
}
bool CDlgAttriUp::Release()
{
    A3DRELEASE(m_pSpriteCover)
	return CDlgBase::Release();
}
void CDlgAttriUp::OnShowDialog()
{
	m_pBtn_Start->Enable(false);
	m_pBtn_Stop->Enable(false);
	m_pBtn_Retry->Enable(false);
	m_pBtn_Get->Enable(false);

	m_nItemType = ItemType_Null;
	m_nItemIndex = 0;
	m_idItem = 0;
	m_nItemCount = 0;
	m_nForgeIndex = 0;
	m_idForge = 0;
	m_bWheelRunning = false;
	m_tStartWheelTime = 0;
	m_idResult = 0;
	m_nResultIndex = 0;

	//for (int i = 0; i < PROP_ADD_ITEM_PRODUCE_COUNT; ++ i)
	//{
	//	m_pImgHover[i]->Show(false);
	//}

	for (int i=0;i<PROP_ADD_ITEM_PRODUCE_COUNT;++i)
	{
		m_pItemProduce[i]->SetCover(NULL,0,1);
	}

	OnCommand_Expand(NULL);
}

void CDlgAttriUp::OnHideDialog()
{
	TakeOffItem();
	
	CDlgAttriUpEnchase* pDlg = (CDlgAttriUpEnchase*)GetGameUIMan()->GetDialog("Win_AttriUp_Enchase");
	pDlg->SetForgesAndCount(m_idNeedForges, 3, 1);
	pDlg->Clear();
	pDlg->Show(false);
	GetGameUIMan()->GetDialog("Win_AttriUp_Nature")->Show(false);
	//GetGameUIMan()->GetDialog("Win_AttriUp_Help")->Show(false);
	m_nItemType = ItemType_Null;
}

void CDlgAttriUp::OnTick()
{
	if (m_nItemType == ItemType_PropAddItem)
	{
		// 检查是否需要重新获取基础属性增益
		CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(m_nItemIndex);
		if (!pItem || pItem->GetTemplateID() != m_idItem || pItem->GetCount() != m_nItemCount)
		{
			g_pGame->GetGameSession()->c2s_CmdGetPropAdd();
			OnEventLButtonDown(0, 0, m_pItem0);
			m_nItemType = ItemType_Null;
		}
	}

	if (m_bWheelRunning)
	{
		DWORD now = GetGame()->GetTickTime();
		if(m_pBtn_Stop->IsEnabled() && (GetTickCount() - m_tStartWheelTime) > 15000)
		{
		    OnCommand_Stop("");
		}
		if (now <= m_tTurnToNext)
		{
			
			
			//m_pImgHover[m_nCurrentTurn]->Show(false);
			m_nCurrentTurn ++;
			if (m_nCurrentTurn >= PROP_ADD_ITEM_PRODUCE_COUNT)
				m_nCurrentTurn = 0;
			//m_pImgHover[m_nCurrentTurn]->Show(true);
			m_tTurnToNext = now + PROP_ADD_GEN_WHEEL_INTERVAL;

			for (int i=0;i<PROP_ADD_ITEM_PRODUCE_COUNT;++i)
			{
				 m_pItemProduce[i]->SetCover(NULL,0,1);
			}
			m_pItemProduce[m_nCurrentTurn]->SetCover(m_pSpriteCover,0,1);

			if (m_idResult != 0)
			{
				if (m_pItemProduce[m_nCurrentTurn]->GetData() == (DWORD)m_idResult)
				{
					m_bWheelRunning = false;
					m_tStartWheelTime = 0;
					OnEventLButtonDown(0, 0, m_pItem0);
					CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(m_nResultIndex);
					if (pItem)
					{
						pItem->SetNeedUpdate(false);
						CommonMessage("set_propadd_item", m_nResultIndex, 0);
					}
				}
			}
		}
	}
}

void CDlgAttriUp::OnCommand_Start(const char * szCommand)
{
	CECInventory* pInventory = GetHostPlayer()->GetPack();
	if (pInventory->GetEmptySlotNum() < 5)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(17703), MB_OK);
		return;
	}
	
	StartWheel();
	m_pBtn_Start->Enable(false);
	m_pBtn_Retry->Enable(false);
	m_pBtn_Get->Enable(false);
	m_pBtn_Stop->Enable(true);
}

void CDlgAttriUp::OnCommand_Stop(const char * szCommand)
{
	m_idResult = 0;
	m_nResultIndex = 0;

	if (m_nItemType == ItemType_PropAddMaterial)
	{
		g_pGame->GetGameSession()->c2s_CmdGenPropAddItem(m_idItem, (short)m_nItemIndex, (short)m_nItemCount);
	}
	else if (m_nItemType == ItemType_PropAddItem)
	{
		g_pGame->GetGameSession()->c2s_CmdRebuildPropAddItem(m_idItem, (short)m_nItemIndex, m_idForge, (short)m_nForgeIndex);
	}
	m_pBtn_Stop->Enable(false);
}

void CDlgAttriUp::OnCommand_Retry(const char * szCommand)
{
	if (m_nItemType != ItemType_PropAddItem)
		return;

	if (GetGameUIMan()->GetDialog("Win_AttriUp_Enchase")->IsShow())
		return;

	CDlgAttriUpEnchase* pDlg = (CDlgAttriUpEnchase*)(GetGameUIMan()->GetDialog("Win_AttriUp_Enchase"));
	pDlg->SetForgesAndCount(m_idNeedForges, 3, 1);
	pDlg->Show(true);
}

void CDlgAttriUp::OnCommand_Get(const char * szCommand)
{
	if (m_nItemType != ItemType_PropAddItem)
		return;

	CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(m_nItemIndex);
	if (!pItem || pItem->GetClassID() != CECIvtrItem::ICID_PROP_ADD_ITEM)
		return;

	CECIvtrPropAddItem* pPropAddItem = (CECIvtrPropAddItem*)pItem;
	const PROP_ADD_ITEM_ESSENCE* pEssence = pPropAddItem->GetDBEssence();
	for (unsigned int i = 0; i < 12; ++ i)
	{
		if (pEssence->prop_add[i] > 0)
		{
			elementdataman* pDataMan = g_pGame->GetElementDataMan();
			DATA_TYPE DataType;
			const void* pData = pDataMan->get_data_ptr(PROP_ADD_CONFIG_ID, ID_SPACE_CONFIG, DataType);
			if (pData && DataType == DT_PROP_ADD_CONFIG)
			{
				PROP_ADD_CONFIG* pConfig = (PROP_ADD_CONFIG*)pData;
				if (GetHostPlayer()->GetPropAddValue(i) >= pConfig->prop_add_limit[i])
				{
					GetGameUIMan()->MessageBox(17704);
					return;
				}
			}
			break;
		}
	}
	
	if (m_pItem0->GetDataPtr())
		((CECIvtrItem*)m_pItem0->GetDataPtr())->Freeze(false);

	CDlgInventory* pDlgInv = (CDlgInventory*)GetGameUIMan()->GetDialog("Win_Inventory");
	GetHostPlayer()->UseItemInPack(IVTRTYPE_PACK, m_nItemIndex);
}

void CDlgAttriUp::OnCommand_Expand(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_AttriUp_Nature");
	pDlg->Show(!pDlg->IsShow());
	pDlg->AlignTo(this, AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
}

void CDlgAttriUp::OnCommand_Help(const char * szCommand)
{
	//PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_AttriUp_Help");
	//pDlg->Show(!pDlg->IsShow());
	//pDlg->AlignTo(this, AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
	AString strDlg = "Win_AttriUp_Help";
	GetGameUIMan()->CommonMessage("Win_HelpInfo","open",(WPARAM)(&strDlg),0);
}

// 放置炼丹材料或丹药
int CDlgAttriUp::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (m_bWheelRunning)
		return 1;

	if (strMsg == "set_material")
	{
		//wParam 包裹栏位置
		CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(wParam);
		if (pItem && pItem->GetClassID() == CECIvtrItem::ICID_PROP_ADD_MATERIAL_ITEM)
		{
			CECIvtrPropAddMaterial* pMaterial = (CECIvtrPropAddMaterial*)pItem;
			GetGameUIMan()->SetIvtrInfo(m_pItem0, pItem);
			m_pItem0->SetData(wParam);
			if (m_pItem0->GetDataPtr())
			{
				((CECIvtrItem*)m_pItem0->GetDataPtr())->Freeze(false);
			}
			m_pItem0->SetDataPtr(pItem);
			pItem->Freeze(true);

			m_nItemType = ItemType_PropAddMaterial;
			m_nItemIndex = wParam;
			m_idItem = pItem->GetTemplateID();
			m_nItemCount = pItem->GetCount();

			ACString szText;
			szText.Format(_AL("%d/%d"), m_nItemCount, pMaterial->GetDBEssence()->require_num);
			m_pItem0->SetText(szText);

			SetProduceItem(*pMaterial->GetDBEssence());

			m_pBtn_Start->Enable(m_nItemCount >= pMaterial->GetDBEssence()->require_num);
			m_pBtn_Stop->Enable(false);
			m_pBtn_Retry->Enable(false);
			m_pBtn_Get->Enable(false);

			return 0;
		}
		return 1;
	}
	else if (strMsg == "set_propadd_item")
	{
		//wParam 包裹栏位置
		CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(wParam);
		if (pItem && pItem->GetClassID() == CECIvtrItem::ICID_PROP_ADD_ITEM)
		{
			CECIvtrPropAddItem* pPropAddItem = (CECIvtrPropAddItem*)pItem;
			GetGameUIMan()->SetIvtrInfo(m_pItem0, pPropAddItem);
			m_pItem0->SetData(wParam);
			if (m_pItem0->GetDataPtr())
			{
				((CECIvtrItem*)m_pItem0->GetDataPtr())->Freeze(false);
			}
			m_pItem0->SetDataPtr(pItem);
			pItem->Freeze(true);

			m_nItemType = ItemType_PropAddItem;
			m_nItemIndex = wParam;
			m_idItem = pItem->GetTemplateID();
			m_nItemCount = pItem->GetCount();

			m_pItem0->SetText(_AL(""));

			m_pBtn_Retry->Enable(true);
			m_pBtn_Get->Enable(true);
			m_pBtn_Start->Enable(false);
			m_pBtn_Stop->Enable(false);

			const PROP_ADD_ITEM_ESSENCE* pItemEssence = pPropAddItem->GetDBEssence();
			elementdataman* pDataMan = g_pGame->GetElementDataMan();
			DATA_TYPE DataType;
			const void* pData = pDataMan->get_data_ptr(pItemEssence->material_id, ID_SPACE_ESSENCE, DataType);
			if (!pData || DataType != DT_PROP_ADD_MATERIAL_ITEM_ESSENCE)
			{
				ASSERT(false);
				return 0;
			}

			CDlgAttriUpNature* pDlg = (CDlgAttriUpNature*)GetGameUIMan()->GetDialog("Win_AttriUp_Nature");
			pDlg->PreviewPropAddItem(m_idItem);

			SetProduceItem(*(PROP_ADD_MATERIAL_ITEM_ESSENCE*)pData);

			return 0;
		}
		return 1;
	}
	return 1;
}

void CDlgAttriUp::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if (m_bWheelRunning)
		return;

	TakeOffItem();
}

// 取下材料
void CDlgAttriUp::TakeOffItem()
{
	PAUIIMAGEPICTURE pImg = m_pItem0;
	if (pImg->GetDataPtr())
	{
		((CECIvtrItem*)pImg->GetDataPtr())->Freeze(false);
	}
	pImg->SetCover(NULL, -1, 0);
	pImg->SetHint(_AL(""));
	pImg->SetData(-1);
	pImg->SetDataPtr(NULL);
	pImg->SetText(_AL(""));

	m_nItemType = ItemType_Null;
	m_nItemIndex = 0;
	m_idItem = 0;
	m_nItemCount = 0;
	m_nForgeIndex = 0;
	m_idForge = 0;

	m_idNeedForges[0] = 0;
	m_idNeedForges[1] = 0;
	m_idNeedForges[2] = 0;

	CDlgAttriUpEnchase* pDlg = (CDlgAttriUpEnchase*)GetGameUIMan()->GetDialog("Win_AttriUp_Enchase");
	pDlg->Clear();
	pDlg->Show(false);


	CDlgAttriUpNature* pDlgNature = (CDlgAttriUpNature*)GetGameUIMan()->GetDialog("Win_AttriUp_Nature");
	pDlgNature->PreviewPropAddItem(m_idItem);

	m_pBtn_Get->Enable(false);
	m_pBtn_Retry->Enable(false);
	m_pBtn_Start->Enable(false);
	m_pBtn_Stop->Enable(false);

	for (int i = 0; i < 6; ++ i)
	{
		AUIImagePicture* pImg = m_pItemProduce[i];
		pImg->SetCover(NULL, -1, 0);
		pImg->SetHint(_AL(""));
		pImg->SetData(-1);
		pImg->SetDataPtr(NULL);
		pImg->SetText(_AL(""));
	}
}

// 设置材料
void CDlgAttriUp::SetForgeItem(int idForge, int nIndex)
{
	if (m_nItemType != ItemType_PropAddItem)
		return;

	m_idForge = idForge;
	m_nForgeIndex = nIndex;

	m_pBtn_Start->Enable(idForge != 0);
}

// 根据原材料配置, 显示产出物品
void CDlgAttriUp::SetProduceItem(const PROP_ADD_MATERIAL_ITEM_ESSENCE& essence)
{
	for (int i = 0; i < 6; ++ i)
	{
		AUIImagePicture* pImg = m_pItemProduce[i];
		int itemId = essence.forge_result[i].id;
		if (itemId <= 0)
			continue;

		CECIvtrItem *pItem = CECIvtrItem::CreateItem(itemId, 0, 1);
		if (pItem)
		{
			pItem->SetNeedUpdate(false);
			pItem->GetDetailDataFromLocal();		
			GetGameUIMan()->SetIvtrInfo(pImg, pItem);
			pImg->SetData(pItem->GetTemplateID());
			delete pItem;
		}
	}

	if (m_nItemType == ItemType_PropAddItem)
	{
		m_idNeedForges[0] = essence.forge_item[0];
		m_idNeedForges[1] = essence.forge_item[1];
		m_idNeedForges[2] = essence.forge_item[2];
	}
}

// 开始转动效果
void CDlgAttriUp::StartWheel()
{
	m_nCurrentTurn = 0;
	m_idResult = 0;
	m_tStartWheelTime = GetTickCount();
	m_tTurnToNext = GetGame()->GetTickTime() + PROP_ADD_GEN_WHEEL_INTERVAL;
	m_bWheelRunning = true;
}

// 合成结果
void CDlgAttriUp::OnPropAddItemResult(int item_id, int item_index)
{
	m_idResult = item_id;
	m_nResultIndex = item_index;
}