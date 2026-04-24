// Filename	: DlgFengshenUpgrade.cpp
// Creator	: WuWeixin
// Date		: 2012/8/24

#include "DlgFengshenUpgrade.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUILabel.h"
#include "EC_IvtrEquip.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_GameUIMan.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_IvtrConsume.h"
#include "EC_IvtrRune.h"
#include "EC_FixedMsg.h"
#include "elementdataman.h"


static const int MAX_PROP_NUM = 5;

AUI_BEGIN_COMMAND_MAP(CDlgFengshenUpgrade, CDlgBase)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_Upgrade",	OnCommand_upgrade)
AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgFengshenUpgrade, CDlgBase)
AUI_END_EVENT_MAP()




CDlgFengshenUpgrade::CDlgFengshenUpgrade()
{
}

CDlgFengshenUpgrade::~CDlgFengshenUpgrade()
{
}

bool CDlgFengshenUpgrade::OnInitDialog()
{
	m_pTitle = (AUILabel *)GetDlgItem("Title");
	m_pTxt_Part = (AUILabel *)GetDlgItem("Txt_Part");
	m_pLab_Type = (AUILabel *)GetDlgItem("Lab_Type");
	m_pTxt_Lv = (AUILabel *)GetDlgItem("Txt_Lv");
	m_pTxt_EquipLv_A = (AUILabel *)GetDlgItem("Txt_EquipLv_A");
	m_pTxt_EquipLv_B = (AUILabel *)GetDlgItem("Txt_EquipLv_B");
	m_pTxt_Score = (AUILabel *)GetDlgItem("Txt_Score");

	m_Lab_Nature_A[0] = (AUILabel *)GetDlgItem("Lab_Nature1_A");
	m_Lab_Nature_A[1] = (AUILabel *)GetDlgItem("Lab_Nature2_A");
	m_Lab_Nature_A[2] = (AUILabel *)GetDlgItem("Lab_Nature3_A");
	m_Lab_Nature_A[3] = (AUILabel *)GetDlgItem("Lab_Nature4_A");
	m_Lab_Nature_A[4] = (AUILabel *)GetDlgItem("Lab_Nature5_A");
	m_Txt_Nature_A[0] = (AUILabel *)GetDlgItem("Txt_Nature1_A");
	m_Txt_Nature_A[1] = (AUILabel *)GetDlgItem("Txt_Nature2_A");
	m_Txt_Nature_A[2] = (AUILabel *)GetDlgItem("Txt_Nature3_A");
	m_Txt_Nature_A[3] = (AUILabel *)GetDlgItem("Txt_Nature4_A");
	m_Txt_Nature_A[4] = (AUILabel *)GetDlgItem("Txt_Nature5_A");
	m_Lab_Nature_B[0] = (AUILabel *)GetDlgItem("Lab_Nature1_B");
	m_Lab_Nature_B[1] = (AUILabel *)GetDlgItem("Lab_Nature2_B");
	m_Lab_Nature_B[2] = (AUILabel *)GetDlgItem("Lab_Nature3_B");
	m_Lab_Nature_B[3] = (AUILabel *)GetDlgItem("Lab_Nature4_B");
	m_Lab_Nature_B[4] = (AUILabel *)GetDlgItem("Lab_Nature5_B");
	m_Txt_Nature_B[0] = (AUILabel *)GetDlgItem("Txt_Nature1_B");
	m_Txt_Nature_B[1] = (AUILabel *)GetDlgItem("Txt_Nature2_B");
	m_Txt_Nature_B[2] = (AUILabel *)GetDlgItem("Txt_Nature3_B");
	m_Txt_Nature_B[3] = (AUILabel *)GetDlgItem("Txt_Nature4_B");
	m_Txt_Nature_B[4] = (AUILabel *)GetDlgItem("Txt_Nature5_B");

	m_pBtn_Upgrade = (AUIStillImageButton *)GetDlgItem("Btn_Upgrade");
	m_pBtn_Upgrade->Enable(false);
	m_pBtn_Upgrade->SetHint(GetStringFromTable(16030));

	m_nItemIndex = 0;
	m_idItem = 0;
	m_nStoneIndex = 0;
	m_idStone = 0;

	return true;
}

void CDlgFengshenUpgrade::OnHideDialog()
{
	m_nItemIndex = 0;
	m_idItem = 0;
	m_nStoneIndex = 0;
	m_idStone = 0;

	CECIvtrItem* pItem = (CECIvtrItem*)GetDataPtr();
	if (pItem)
		pItem->Freeze(false);
	SetDataPtr(NULL);
}

void CDlgFengshenUpgrade::OnTick()
{
	if (m_idItem == 0)
		return;

	// 只有包裹中有升级道具时, "升级"按钮才可以点击
	m_idStone = 0;
	for (int i = 0; i < 3; ++ i)
	{
		int index = GetHostPlayer()->GetPack()->FindItem(
			g_pGame->GetGameRun()->GetSpecialIDConfig().id_xingzuo_levelup[i]);
		if (index != -1)
		{
			m_nStoneIndex = index;
			CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(m_nStoneIndex);
			m_idStone = pItem->GetTemplateID();
			break;
		}
	}

	UpdateUpgradeButton();
}

bool CDlgFengshenUpgrade::SetAstrology(int nItemIndex)
{
	UpdateUpgradeButton();
	
	//wParam 包裹栏位置
	CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(nItemIndex);
	if (!pItem || pItem->GetClassID() != CECIvtrItem::ICID_ASTROLOGY)
		return false;

	CECIvtrAstrology* pAstrology = (CECIvtrAstrology*)pItem;
	if (pAstrology->GetEssence().level >= 0) // 未鉴定的服务器不会发essence
	{
		if (GetDataPtr() != NULL)
		{
			CECIvtrItem* pOldItem = (CECIvtrItem*)GetDataPtr();
			pOldItem->Freeze(false);
		}

		SetDataPtr(pItem);
		pItem->Freeze(true);

		m_nItemIndex = nItemIndex;
		m_idItem = pItem->GetTemplateID();

		UpdateProps();
		UpdateUpgradeButton();

		Show(true);

		return true;
	}

	return false;
}

// 刷新界面显示
void CDlgFengshenUpgrade::UpdateProps()
{
	ClearProps();

	CECIvtrAstrology* pAstrology = (CECIvtrAstrology*)GetDataPtr();
	if (!pAstrology)
		return;

	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	const XINGZUO_ITEM_ESSENCE* pDbEssence = pAstrology->GetDBEssence();
	const IVTR_ASTROLOGY_ESSENCE& essence = pAstrology->GetEssence();
	ACString szValue;

	m_pTitle->SetText(pDbEssence->name);

	//可镶嵌部位，目前限定在武器、帽子、衣服、鞋子上面，如果扩展，这里也需要修改，并且需要将部位名字抽取出来放到itemdesc.txt中
	if (pDbEssence->equip_mask & EQUIP_MASK_WEAPON)
		m_pTxt_Part->SetText(pDescTab->GetWideString(ITEMDESC_WEAPON));
	else if (pDbEssence->equip_mask & EQUIP_MASK_HEAD)
		m_pTxt_Part->SetText(pDescTab->GetWideString(ITEMDESC_HAT));
	else if (pDbEssence->equip_mask & EQUIP_MASK_BODY)
		m_pTxt_Part->SetText(pDescTab->GetWideString(ITEMDESC_CLOTHES));
	else if (pDbEssence->equip_mask & EQUIP_MASK_FOOT)
		m_pTxt_Part->SetText(pDescTab->GetWideString(ITEMDESC_SHOE));
	
	if (pDbEssence->reborn_count == 0)
		m_pLab_Type->SetText(pDescTab->GetWideString(ITEMDESC_ASTROLOGY_NOTFLY));
	else
		m_pLab_Type->SetText(pDescTab->GetWideString(ITEMDESC_ASTROLOGY_FLY));

	szValue.Format(_AL("%d/10"), essence.level);
	m_pTxt_Lv->SetText(szValue);

	szValue.Format(_AL("%d"), CECIvtrAstrology::GetEquipLevelLimit(essence.level));
	m_pTxt_EquipLv_A->SetText(szValue);
	if (essence.level < 10)
	{
		szValue.Format(_AL("%d"), CECIvtrAstrology::GetEquipLevelLimit(essence.level+1));
		m_pTxt_EquipLv_B->SetText(szValue);
	}

	int iColor = CECIvtrAstrology::GetScoreColor(essence.score);
	szValue.Format(_AL("%s%d"), g_pGame->GetItemDesc()->GetWideString(iColor), essence.score);
	m_pTxt_Score->SetText(szValue);

	const ASTROLOGY_PROP* props = pAstrology->GetEssence().prop;
	
	for (int i = 0; i < MAX_PROP_NUM; ++ i)
	{
		const ASTROLOGY_PROP& prop = props[i];
		if (prop.id == 0)
			break;

		const wchar_t* szText = pDescTab->GetWideString(ITEMDESC_EQUIPADDON_61+prop.id);
		m_Lab_Nature_A[i]->SetText(szText);

		int value = prop.value;
		szValue.Format(_AL(" +%s"), CECIvtrRune::GetPropString(prop.id, int(value+value*essence.level*0.25f)));
		m_Txt_Nature_A[i]->SetText(szValue);

		if (essence.level < 10)
		{
			m_Lab_Nature_B[i]->SetText(szText);
			szValue.Format(_AL(" +%s"), CECIvtrRune::GetPropString(prop.id, int(value+value*(essence.level+1)*0.25f)));
			m_Txt_Nature_B[i]->SetText(szValue);
		}
	}
}

void CDlgFengshenUpgrade::ClearProps()
{
	for (int i = 0; i < MAX_PROP_NUM; ++ i)
	{
		m_Lab_Nature_A[i]->SetText(_AL(""));
		m_Txt_Nature_A[i]->SetText(_AL(""));
		m_Lab_Nature_B[i]->SetText(_AL(""));
		m_Txt_Nature_B[i]->SetText(_AL(""));
	}
	m_pTxt_EquipLv_A->SetText(_AL(""));
	m_pTxt_EquipLv_B->SetText(_AL(""));
}

void CDlgFengshenUpgrade::UpdateUpgradeButton()
{
	bool bMaxLevel = false;
	if (GetDataPtr())
	{
		CECIvtrAstrology* pAstrology = (CECIvtrAstrology*)GetDataPtr();
		bMaxLevel = pAstrology->GetEssence().level < 10;
	}
	m_pBtn_Upgrade->Enable(bMaxLevel && !(m_idItem == 0 || m_idStone == 0));
}

void CDlgFengshenUpgrade::OnCommand_upgrade(const char * szCommand)
{
	if (m_idItem == 0 || m_idStone == 0)
		return;

	GetGameUIMan()->MessageBox("Game_FengshenUpgrade", GetStringFromTable(16032), MB_OKCANCEL);
}

void CDlgFengshenUpgrade::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
}	

// 玩家确认升级, 发送协议
void CDlgFengshenUpgrade::SendCmd()
{
	if (m_idItem == 0 || m_idStone == 0)
		return;
	
	g_pGame->GetGameSession()->c2s_CmdAstrologyUpgrade(m_idItem, m_nItemIndex, m_idStone, m_nStoneIndex);
}