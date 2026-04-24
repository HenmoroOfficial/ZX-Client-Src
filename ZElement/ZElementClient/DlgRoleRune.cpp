// Filename	:	DlgRune.cpp
// Creator	:	Wu Weixin
// Date		:	2013/04/11
// Description:	符文界面

#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "DlgRoleRune.h"
#include "ExpTypes.h"
#include "EC_HostPlayer.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "elementdataman.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrConsume.h"
#include "EC_Configs.h"
#include "ExpTypes.h"
#include "AUI\\AUIDef.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIScroll.h"
#include "AUI\\AUICheckBox.h"

#define new A_DEBUG_NEW

const int PACK_SIZE = 36;
const int PACK_COL = 6;
const int PACK_ROW = 6;
const int COMBINE_COST_COUNT = 10;
const int COMBINE_TIMES_LIMIT = 10;

// 符文格子总数
const int RUNE_CELL_COUNT = 6;

// 符文格子开放等级
int RUNE_CELL_OPEN_LEVELS[RUNE_CELL_COUNT] = {150, 152, 154, 156, 158,160};

// 一个符文碎片能提供的符文升级经验
const int FRAGEMENT_EXP = 1;

//////////////////////////////////////////////////////////////////////////
// 符文界面
AUI_BEGIN_COMMAND_MAP(CDlgRoleRune, CDlgBase)
AUI_ON_COMMAND("Btn_Move",	OnCommand_Move)
AUI_ON_COMMAND("Btn_Help",	OnCommand_Help)
AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgRoleRune, CDlgBase)
AUI_ON_EVENT("Item_*",			WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Item_*",			WM_RBUTTONDOWN,		OnEventRButtonDown)
AUI_END_EVENT_MAP()


CDlgRoleRune::CDlgRoleRune()
: m_nCurrentSel(-1)
{
}

CDlgRoleRune::~CDlgRoleRune()
{
}

bool CDlgRoleRune::OnInitDialog()
{
	CDlgBase::OnInitDialog();

	this->SetCanMove(false);

	char szItem[40];
	for (int i = 0; i < 6; ++ i)
	{
		sprintf(szItem, "Item_%02d", i+1);
		m_pItem[i] = (AUIImagePicture*)GetDlgItem(szItem);
		m_pItem[i]->SetData(i);

		sprintf(szItem, "Img_Hover%02d", i+1);
		m_pImg_Hover[i] = GetDlgItem(szItem);
		m_pImg_Hover[i]->SetTransparent(true);

		sprintf(szItem, "Shape_%02d", i+1);
		m_pShape[i] = GetDlgItem(szItem);
		m_pShape[i]->SetTransparent(true);

		sprintf(szItem, "Lab_A%02d", i+1);
		m_pLab[i] = (AUILabel*)GetDlgItem(szItem);
		m_pLab[i]->Show(false);

		sprintf(szItem, "Txt_A%02d", i+1);
		m_pTxt[i] = (AUILabel*)GetDlgItem(szItem);
		m_pTxt[i]->Show(false);
	}
	
	return true;
}

void CDlgRoleRune::OnShowDialog()
{
	CECHostPlayer* pHostPlayer = GetHostPlayer();
	if (pHostPlayer->GetBasicProps().iLevel < 150 || pHostPlayer->GetRebornCount() < 1)
	{
		Show(false);
		return;
	}
	AUIDialog* pDlg = GetGameUIMan()->GetDialog("Win_Char_Nature");
	if (!pDlg || !pDlg->IsShow())
	{
		Show(false);
		return;
	}
	
	m_iSlotPack = -1;
	m_iSlotRune = -1;
	m_nCurrentSel = -1;
	ShowEnchantingGfx();
	UpdateView();
	ShowRuneTipPop();
}

void CDlgRoleRune::OnHideDialog()
{
	//GetGameUIMan()->GetDialog("Win_Equip_Rune_Help")->Show(false);
	CDlgBase::OnHideDialog();
}

void CDlgRoleRune::OnTick()
{
	AUIDialog* pDlg = GetGameUIMan()->GetDialog("Win_Char_Nature");
	if (!pDlg || !pDlg->IsShow())
	{
		Show(false);
		return;
	}
	AlignTo(pDlg, AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);

	if (m_nCurrentSel >= 0)
	{
		CECIvtrItem* pItem = GetHostPlayer()->GetRune2013Pack()->GetItem(m_nCurrentSel);
		if (!pItem)
		{
			m_nCurrentSel = -1;
		}
	}
	UpdateView();
}

int CDlgRoleRune::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg == "set_install")
	{
		m_iSlotPack = -1;
		m_iSlotRune = -1;

		AUIObject* pObj = (AUIObject*)lParam;
		int src_index = (int)wParam;
		int dst_index = -1;
		int i;
		for (i = 0; i < 6; ++ i)
		{
			if (m_pItem[i] == pObj)
			{
				dst_index = i;
				break;
			}
		}

		if (dst_index < 0 || dst_index >= 6)
			return 1;

		if (GetHostPlayer()->GetRune2013Pack()->GetItem(dst_index))
			return 1;

		CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(src_index);
		if (pItem->GetClassID() != CECIvtrItem::ICID_RUNE2013_ITEM)
			return 1;

		CECIvtrRune2013* pSrcRune = (CECIvtrRune2013*)pItem;
		CECInventory* pPack = GetHostPlayer()->GetRune2013Pack();
		for (i = 0; i < m_nCurrentSize; ++ i)
		{
			if (CECIvtrItem* pItem = pPack->GetItem(i))
			{
				if (pItem->GetClassID() == CECIvtrItem::ICID_RUNE2013_ITEM)
				{
					CECIvtrRune2013* pEquipedRune = (CECIvtrRune2013*)pItem;
					if (pEquipedRune->GetDBEssence()->prop_type == pSrcRune->GetDBEssence()->prop_type)
					{
						GetGameUIMan()->MessageBox("", GetStringFromTable(18303));
						return 1;
					}
				}
			}
		}

		m_iSlotPack = dst_index;
		m_iSlotRune = src_index;

		GetGameUIMan()->MessageBox("Game_Rune2013_Install", GetStringFromTable(18302), MB_YESNO);

		return 0;
	}
	else if (strMsg == "install")
	{
		if (m_iSlotRune >= 0 && m_iSlotPack >= 0)
			GetGameSession()->c2s_CmdFuWenInstall(m_iSlotRune, m_iSlotPack);
		m_iSlotPack = -1;
		m_iSlotRune = -1;
		return 0;
	}
	return 1;
}

void CDlgRoleRune::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if (!pObj->GetDataPtr("ptr_CECIvtrItem"))
		return;

	int index = pObj->GetData();
	if (pObj != m_pItem[index])
		return;

	m_nCurrentSel = -1;
	GetDlgItem("Btn_Move")->Enable(false);

	for (int i = 0; i < 6; ++ i)
	{
		if (i == index)
		{
			m_pImg_Hover[i]->Show(true);
			m_nCurrentSel = i;
			GetDlgItem("Btn_Move")->Enable(true);
		}
		else
		{
			m_pImg_Hover[i]->Show(false);
		}
	}
}

void CDlgRoleRune::OnEventRButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if (pObj->GetData() < 0)
		return;

	CDlgRoleRuneEnchanting* pDlg = (CDlgRoleRuneEnchanting*)GetGameUIMan()->GetDialog("Win_Equip_Rune_Enchanting");
	if (pDlg)
		pDlg->EnchantingRune(pObj->GetData(), IVTRTYPE_FUWEN);

	OnEventLButtonDown(wParam, lParam, pObj);
}

// 移除符文
void CDlgRoleRune::OnCommand_Move(const char * szCommand)
{
	CECHostPlayer* pHostPlayer = GetHostPlayer();
	CECInventory* pPack = pHostPlayer->GetPack();
	int nEmptySlot = pPack->SearchEmpty();
	if (nEmptySlot < 0)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(7076));
		return;
	}

	const SPECIAL_ID_CONFIG& cfg = GetGame()->GetGameRun()->GetSpecialIDConfig();
	int nCount = pPack->GetSize();
	int nAssistIndex = pPack->FindItem(cfg.id_rune2013_erase[0]);
	if (nAssistIndex < 0)
	{
		nAssistIndex = pPack->FindItem(cfg.id_rune2013_erase[1]);
		if (nAssistIndex < 0)
		{
			ACString szText;
			ACString szName = _AL("");
			const SPECIAL_ID_CONFIG& cfg = GetGame()->GetGameRun()->GetSpecialIDConfig();
			if (CECIvtrItem *pItem = CECIvtrItem::CreateItem(cfg.id_rune2013_erase[0], 0, 1))
			{
				szName = pItem->GetName();
				delete pItem;
			}
			szText.Format(GetStringFromTable(18301), szName);
			GetGameUIMan()->MessageBox("", szText);
			return;
		}
	}
	GetGameSession()->c2s_CmdFuWenUninstall(m_nCurrentSel, nAssistIndex, nEmptySlot);
}

void CDlgRoleRune::OnCommand_Help(const char * szCommand)
{
	//PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Equip_Rune_Help");
	//pDlg->Show(!pDlg->IsShow());
	//pDlg->AlignTo(this, AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
	AString strDlg = "Win_Equip_Rune_Help";
	GetGameUIMan()->CommonMessage("Win_HelpInfo","open",(WPARAM)(&strDlg),0);

	CECConfigs* pConfig = g_pGame->GetConfigs();
	EC_LOCAL_SETTING ls = pConfig->GetLocalSettings();
	ls.bFuWenPop2Showed = true;
	pConfig->SetLocalSettings(ls);
	pConfig->SaveLocalSetting();
	ShowRuneTipPop();
}
	
// 更新显示
void CDlgRoleRune::UpdateView()
{
	CECHostPlayer* pHostPlayer = GetHostPlayer();
	int level = pHostPlayer->GetBasicProps().iLevel;
	int i;
	m_nCurrentSize = 0;
	for (i = 0; i < RUNE_CELL_COUNT; ++ i)
	{
		if (level >= RUNE_CELL_OPEN_LEVELS[i])
			m_nCurrentSize ++;
		else
			break;
	}

	if (m_nCurrentSize <= 0)
	{
		Show(false);
		return;
	}

	CECInventory* pPack = pHostPlayer->GetRune2013Pack();
	CECIvtrItem* pItem;
	AUIImagePicture* pImg;
	abase::vector<CECIvtrRune2013*> vecTemp;
	for (i = 0; i < m_nCurrentSize; ++ i)
	{
		pImg = m_pItem[i];
		pImg->Show(true);
		m_pShape[i]->Show(true);
		m_pImg_Hover[i]->Show(i == m_nCurrentSel);

		pItem = pPack->GetItem(i);
		if (pItem)
		{
			pImg->SetDataPtr(pItem, "ptr_CECIvtrItem");
			pImg->SetData(i);
			GetGameUIMan()->SetIvtrInfo(pImg, pItem);
			vecTemp.push_back((CECIvtrRune2013*)pItem);
		}
		else
		{
			pImg->SetDataPtr(NULL);
			pImg->SetData(-1);
			GetGameUIMan()->SetIvtrInfo(pImg, NULL);
		}
	}

	for (; i < 6; ++ i)
	{
		pImg = m_pItem[i];
		pImg->Show(false);
		m_pShape[i]->Show(false);
		m_pImg_Hover[i]->Show(false);
		pImg->SetDataPtr(NULL);
		pImg->SetData(-1);
		GetGameUIMan()->SetIvtrInfo(pImg, NULL);
	}

	GetDlgItem("Btn_Move")->Enable(vecTemp.size() > 0 && m_nCurrentSel != -1);

	ACString szText;
	CECIvtrRune2013* pItemRune;
	int count = (int)vecTemp.size();
	for (i = 0; i < count; ++ i)
	{
		pItemRune = vecTemp[i];
		
		szText.Format(GetStringFromTable(18325), GetStringFromTable(18400+pItemRune->GetDBEssence()->prop_type-PROP_TYPE_DAMAGE));
		m_pLab[i]->SetText(szText);
		m_pLab[i]->Show(true);

		szText.Format(_AL("+%s"), pItemRune->GetPropString());
		m_pTxt[i]->SetText(szText);
		m_pTxt[i]->Show(true);
	}

	for (; i < 6; ++ i)
	{
		m_pLab[i]->Show(false);
		m_pTxt[i]->Show(false);
	}
}

// 飞升150后，第一次打开时需要显示符文泡泡提示
void CDlgRoleRune::ShowRuneTipPop()
{
	bool needShow = !g_pGame->GetConfigs()->GetLocalSettings().bFuWenPop2Showed;
	GetDlgItem("Img_Arrow01")->Show(needShow);
	GetDlgItem("Txt_PopHint01")->Show(needShow);
	GetDlgItem("Shape_PopHint01")->Show(needShow);
}

// 显示融合特效
void CDlgRoleRune::ShowEnchantingGfx()
{
	if (!IsShow())
		return;

	GetDlgItem("Gfx_01")->Show(false);
	GetDlgItem("Gfx_02")->Show(false);
	GetDlgItem("Gfx_03")->Show(false);
	GetDlgItem("Gfx_04")->Show(false);
	GetDlgItem("Gfx_05")->Show(false);
	GetDlgItem("Gfx_06")->Show(false);
	
	if (m_nCurrentSel == -1)
		return;

	char szItem[16];
	sprintf(szItem, "Gfx_%02d", m_nCurrentSel+1);
	GetDlgItem(szItem)->Show(true);
}

//////////////////////////////////////////////////////////////////////////
// 符文合成界面

AUI_BEGIN_EVENT_MAP(CDlgRoleRuneCombine, CDlgBase)
AUI_ON_EVENT("*",			WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT(NULL,			WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_END_EVENT_MAP()

AUI_BEGIN_COMMAND_MAP(CDlgRoleRuneCombine, CDlgBase)
AUI_ON_COMMAND("top",			OnCommand_Top)
AUI_ON_COMMAND("bottom",		OnCommand_Bottom)
AUI_ON_COMMAND("prec",			OnCommand_Prec)
AUI_ON_COMMAND("next",			OnCommand_Next)
AUI_ON_COMMAND("Btn_One",		OnCommand_One)
AUI_ON_COMMAND("Btn_All",		OnCommand_All)
AUI_ON_COMMAND("Btn_Cancel",	OnCommand_Cancel)
AUI_ON_COMMAND("Btn_Help",		OnCommand_Help)
AUI_END_COMMAND_MAP()

CDlgRoleRuneCombine::CDlgRoleRuneCombine()
: m_nCombineTiems(0), m_nExtraTimes(0), m_bNeedUpdateItem(false)
{
}

CDlgRoleRuneCombine::~CDlgRoleRuneCombine()
{
}

bool CDlgRoleRuneCombine::OnInitDialog()
{
	CDlgBase::OnInitDialog();

	char szItem[40];
	for (int i = 0; i < PACK_SIZE; ++ i)
	{
		sprintf(szItem, "Item_%02d", i);
		m_pImgItems[i] = (AUIImagePicture*)GetDlgItem(szItem);
	}
	m_pImgItemMatter = (AUIImagePicture*)GetDlgItem("Img_Item");
	m_pTxt_Time = (AUILabel*)GetDlgItem("Txt_Time");
	m_pTxt_TimeLimit = (AUILabel*)GetDlgItem("Txt_TimeLimit");
	m_pScl_Item = (AUIScroll*)GetDlgItem("Scl_Item");
	
	return true;
}

void CDlgRoleRuneCombine::OnShowDialog()
{
	GetGameUIMan()->GetDialog("Win_Equip_Rune_Enchanting")->Show(false);
	m_bProducing = false;
	m_bNeedUpdateItem = false;
	m_nProduceCount = 0;
	UpdateRuneFragment();
	ShowRuneTipPop();
}

void CDlgRoleRuneCombine::OnHideDialog()
{
	ClearRuneFragment();
	//GetGameUIMan()->GetDialog("Win_Equip_Rune_Combine_Help")->Show(false);
	CDlgBase::OnHideDialog();
}

bool CDlgRoleRuneCombine::Tick(void)
{
	CDlgBase::Tick();

	CECHostPlayer* pHost = GetHostPlayer();
	if( !pHost->IsGathering())
		m_bProducing = 0;

	if (m_bNeedUpdateItem)
		UpdateRuneFragment();
	else
		UpdateView();

	return true;
}

void CDlgRoleRuneCombine::OnCommand_Help(const char * szCommand)
{
	//PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Equip_Rune_Combine_Help");
	//pDlg->Show(!pDlg->IsShow());
	//pDlg->AlignTo(this, AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
	AString strDlg = "Win_Equip_Rune_Combine_Help";
	GetGameUIMan()->CommonMessage("Win_HelpInfo","open",(WPARAM)(&strDlg),0);


	CECConfigs* pConfig = g_pGame->GetConfigs();
	EC_LOCAL_SETTING ls = pConfig->GetLocalSettings();
	ls.bFuWenPop3Showed = true;
	pConfig->SetLocalSettings(ls);
	pConfig->SaveLocalSetting();
	ShowRuneTipPop();
}

void CDlgRoleRuneCombine::OnCommand_Top(const char * szCommand)
{
	m_pScl_Item->SetBarLevel(0);
}

void CDlgRoleRuneCombine::OnCommand_Bottom(const char * szCommand)
{
	int count = m_vecFragmentIndies.size();
	m_pScl_Item->SetBarLevel(count / 3 - 2);
}

void CDlgRoleRuneCombine::OnCommand_Prec(const char * szCommand)
{
	m_pScl_Item->SetBarLevel(m_pScl_Item->GetBarLevel() - 3);
}

void CDlgRoleRuneCombine::OnCommand_Next(const char * szCommand)
{
	m_pScl_Item->SetBarLevel(m_pScl_Item->GetBarLevel() + 3);
}

// 合成一个
void CDlgRoleRuneCombine::OnCommand_One(const char * szCommand)
{
	if (m_bProducing)
		return;
	m_nProduceCount = 1;
	ProduceOne();
}
	
// 合成全部
void CDlgRoleRuneCombine::OnCommand_All(const char * szCommand)
{
	if (m_bProducing)
		return;
	m_nProduceCount = m_nTotalFragmentsCount / COMBINE_COST_COUNT;
	ProduceOne();
}

// 取消
void CDlgRoleRuneCombine::OnCommand_Cancel(const char * szCommand)
{
	Show(false);
}

// 更新合成次数
void CDlgRoleRuneCombine::UpdateCombineTimes(int times, int extraTimes)
{
	m_nCombineTiems = times;
	m_nExtraTimes = extraTimes;
	if (IsShow())
		UpdateRuneFragment();
}

// 向服务器请求合成一个
void CDlgRoleRuneCombine::ProduceOne()
{
	if (m_bProducing)
		UpdateRuneFragment();

	if (m_nTotalFragmentsCount < COMBINE_COST_COUNT)
		m_nProduceCount = 0;

	m_bProducing = m_nProduceCount > 0;
	if (!m_bProducing)
		return;

	if (GetHostPlayer()->GetPack()->GetEmptySlotNum() == 0)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(7076));
		m_nProduceCount = 0;
		return;
	}

	m_nProduceCount--;

	if (m_nCombineTiems >= 20)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(18323));
		m_nProduceCount = 0;
		return;
	}

	const SPECIAL_ID_CONFIG& cfg = GetGame()->GetGameRun()->GetSpecialIDConfig();
	CECInventory* pPack = GetHostPlayer()->GetPack();
	int extra_item_index = pPack->FindItem(cfg.id_rune2013_merge_extra_num[0]);
	if (extra_item_index < 0)
	{
		extra_item_index = pPack->FindItem(cfg.id_rune2013_merge_extra_num[1]);
		if (extra_item_index < 0)
		{
			if (m_nCombineTiems >= COMBINE_TIMES_LIMIT )
			{
				ACString szText;
				ACString szName = _AL("");
				const SPECIAL_ID_CONFIG& cfg = GetGame()->GetGameRun()->GetSpecialIDConfig();
				if (CECIvtrItem *pItem = CECIvtrItem::CreateItem(cfg.id_rune2013_merge_extra_num[0], 0, 1))
				{
					szName = pItem->GetName();
					delete pItem;
				}
				szText.Format(GetStringFromTable(18324), szName);
				GetGameUIMan()->MessageBox("", szText);

				m_nProduceCount = 0;
				return;
			}
			extra_item_index = 0;
		}
	}

	GetGameSession()->c2s_CmdFuWenCompose(m_nCombineTiems >= COMBINE_TIMES_LIMIT, extra_item_index);
}

// 更新碎片显示
void CDlgRoleRuneCombine::UpdateRuneFragment()
{
	ClearRuneFragment();

	const SPECIAL_ID_CONFIG& cfg = GetGame()->GetGameRun()->GetSpecialIDConfig();
	CECInventory* pPack = GetHostPlayer()->GetPack();
	int size = pPack->GetSize();
	m_nTotalFragmentsCount = 0;
	m_nExtItemCount = 0;

	for (int i = 0; i < size; ++ i)
	{
		CECIvtrItem* pItem = pPack->GetItem(i);
		if (!pItem)
			continue;

		if (pItem->GetTemplateID() == cfg.id_rune2013_fragment[0] || pItem->GetTemplateID() == cfg.id_rune2013_fragment[1])
		{
			m_nTotalFragmentsCount += pItem->GetCount();
			m_vecFragmentIndies.push_back(i);
		}
		else if (pItem->GetTemplateID() == cfg.id_rune2013_merge_extra_num[0] || pItem->GetTemplateID() == cfg.id_rune2013_merge_extra_num[1])
		{
			m_nExtItemCount += pItem->GetCount();
			pItem->Freeze(true);
		}
	}

	CECIvtrItem *pItem = CECIvtrItem::CreateItem(cfg.id_rune2013_fragment[0], 0, m_nTotalFragmentsCount);
	if (pItem)
	{
		if (m_vecFragmentIndies.size() > 0)
		{
			pItem->GetDetailDataFromLocal();
			GetGameUIMan()->SetIvtrInfo(m_pImgItemMatter, pItem, pItem->GetCount());
		}
		else
		{
			GetGameUIMan()->SetIvtrInfo(m_pImgItemMatter, NULL, 0);
		}
		delete pItem;
	}
	else
	{
		GetGameUIMan()->SetIvtrInfo(m_pImgItemMatter, NULL, 0);
	}
	pItem = NULL;

	int nExtraTimes = min(10, m_nExtItemCount+m_nExtraTimes);
	wchar_t buffer[64];
	swprintf(buffer, L"%d+%d", COMBINE_TIMES_LIMIT, nExtraTimes);
	m_pTxt_TimeLimit->SetText(buffer);

	swprintf(buffer, L"%d", m_nCombineTiems); 
	m_pTxt_Time->SetText(buffer);

	m_pScl_Item->SetScrollRange(0, (m_vecFragmentIndies.size() - 1) / PACK_COL - PACK_SIZE / PACK_COL + 1);
	m_pScl_Item->Show(m_vecFragmentIndies.size() > PACK_SIZE);
	m_pScl_Item->SetBarLength(-1);
	m_pScl_Item->SetScrollStep(1);
	m_nFirstLine = 0;

	GetDlgItem("Btn_One")->Enable(m_nTotalFragmentsCount >= COMBINE_COST_COUNT);
	GetDlgItem("Btn_All")->Enable(m_nTotalFragmentsCount >= COMBINE_COST_COUNT);

	UpdateView();
}

// 清除碎片信息
void CDlgRoleRuneCombine::ClearRuneFragment()
{
	const SPECIAL_ID_CONFIG& cfg = GetGame()->GetGameRun()->GetSpecialIDConfig();
	CECInventory* pPack = GetHostPlayer()->GetPack();
	int size = pPack->GetSize();

	abase::vector<int>::iterator it = m_vecFragmentIndies.begin();
	abase::vector<int>::iterator ite = m_vecFragmentIndies.end();
	for ( ; it != ite; ++ it )
	{
		if (CECIvtrItem* pItem = pPack->GetItem(*it))
			pItem->Freeze(false);
	}
	m_vecFragmentIndies.clear();

	for (int i = 0; i < size; ++ i)
	{
		CECIvtrItem* pItem = pPack->GetItem(i);
		if (!pItem)
			continue;

		if (pItem->GetTemplateID() == cfg.id_rune2013_merge_extra_num[0] || pItem->GetTemplateID() == cfg.id_rune2013_merge_extra_num[1])
			pItem->Freeze(false);
	}
}

// 更新显示
void CDlgRoleRuneCombine::UpdateView()
{
	if( m_nFirstLine != m_pScl_Item->GetBarLevel() )
		m_nFirstLine = m_pScl_Item->GetBarLevel();
	
	int nStart = m_nFirstLine * PACK_COL;
	unsigned int iIvtr;
	unsigned int iImg = 0;
	unsigned int len = m_vecFragmentIndies.size();
	AUIImagePicture* pImg = NULL;
	CECIvtrItem* pItem;
	CECInventory* pInventory = GetHostPlayer()->GetPack();
	const SPECIAL_ID_CONFIG& cfg = GetGame()->GetGameRun()->GetSpecialIDConfig();

	for (iIvtr = nStart; iIvtr < len && iImg < PACK_SIZE; ++ iIvtr, ++ iImg)
	{
		pItem = pInventory->GetItem(m_vecFragmentIndies[iIvtr]);
		if (!pItem || !(pItem->GetTemplateID() == cfg.id_rune2013_fragment[0] || pItem->GetTemplateID() == cfg.id_rune2013_fragment[1]))
		{
			m_bNeedUpdateItem = true;
			return;
		}
		pImg = m_pImgItems[iImg];
		pItem->Freeze(true);
		GetGameUIMan()->SetIvtrInfo(pImg, pItem, pItem->GetCount());
	}

	for (; iImg < PACK_SIZE; ++ iImg)
	{
		pImg = m_pImgItems[iImg];
		GetGameUIMan()->SetIvtrInfo(pImg, NULL);
	}
}

void CDlgRoleRuneCombine::OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
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

// 飞升150后，第一次打开时需要显示符文泡泡提示
void CDlgRoleRuneCombine::ShowRuneTipPop()
{
	bool needShow = !g_pGame->GetConfigs()->GetLocalSettings().bFuWenPop3Showed;
	GetDlgItem("Img_Arrow01")->Show(needShow);
	GetDlgItem("Txt_PopHint01")->Show(needShow);
	GetDlgItem("Shape_PopHint01")->Show(needShow);
}

//////////////////////////////////////////////////////////////////////////
// 符文融合界面

AUI_BEGIN_EVENT_MAP(CDlgRoleRuneEnchanting, CDlgBase)
AUI_ON_EVENT("*",			WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT(NULL,			WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT("Item_*",		WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Item_*",		WM_RBUTTONDOWN,		OnEventRButtonDown)
AUI_END_EVENT_MAP()

AUI_BEGIN_COMMAND_MAP(CDlgRoleRuneEnchanting, CDlgBase)
AUI_ON_COMMAND("Btn_Enchanting",	OnCommandEnchanting)
AUI_ON_COMMAND("Btn_Cancel",		OnCommand_Cancel)
AUI_ON_COMMAND("Chk_01",			OnCommand_FreeSel)
AUI_ON_COMMAND("Chk_02",			OnCommand_SelLv1Rune)
AUI_ON_COMMAND("Chk_03",			OnCommand_SelLv2Rune)
AUI_ON_COMMAND("Chk_04",			OnCommand_SelFragment)
AUI_ON_COMMAND("Btn_Help",			OnCommand_Help)
AUI_END_COMMAND_MAP()

CDlgRoleRuneEnchanting::CDlgRoleRuneEnchanting()
: m_bNeedUpdateItem(false)
{
}

CDlgRoleRuneEnchanting::~CDlgRoleRuneEnchanting()
{
}

bool CDlgRoleRuneEnchanting::OnInitDialog()
{
	CDlgBase::OnInitDialog();

	char szItem[40];
	for (int i = 0; i < PACK_SIZE; ++ i)
	{
		sprintf(szItem, "Item_%02d", i);
		m_pImgItems[i] = (AUIImagePicture*)GetDlgItem(szItem);

		sprintf(szItem, "Img_Hover%02d", i);
		m_pImgHovers[i] = (AUIImagePicture*)GetDlgItem(szItem);
		m_pImgHovers[i]->SetTransparent(true);
		m_pImgHovers[i]->Show(false);
	}
	m_pImgMain = (AUIImagePicture*)GetDlgItem("Img_Item");
	m_pTxt_Exp = (AUILabel*)GetDlgItem("Txt_Exp");
	m_pChk_01 = (AUICheckBox*)GetDlgItem("Chk_01");
	m_pChk_02 = (AUICheckBox*)GetDlgItem("Chk_02");
	m_pChk_03 = (AUICheckBox*)GetDlgItem("Chk_03");
	m_pChk_04 = (AUICheckBox*)GetDlgItem("Chk_04");
	m_pScl_Item = (AUIScroll*)GetDlgItem("Scl_Item");
	
	GetDlgItem("Img_Arrow")->SetTransparent(true);

	return true;
}

void CDlgRoleRuneEnchanting::OnShowDialog()
{
	if (m_nMainRuneIndex < 0)
	{
		Show(false);
		return;
	}

	GetDlgItem("Gfx_01")->Show(false);
	GetGameUIMan()->GetDialog("Win_Equip_Rune_Combine")->Show(false);
	m_pChk_01->Check(true);
	GetDlgItem("Btn_Enchanting")->Enable(false);
	OnCommand_FreeSel(NULL);
	UpdateRuneItem();
	ShowRuneTipPop();
}

void CDlgRoleRuneEnchanting::OnHideDialog()
{
	ClearRuneItem();
	m_nMainRuneIndex = 0;
	//GetGameUIMan()->GetDialog("Win_Equip_Rune_Enchanting_Help")->Show(false);
	CDlgBase::OnHideDialog();
}

void CDlgRoleRuneEnchanting::OnTick()
{
	CDlgBase::OnTick();

	if (m_bNeedUpdateItem)
		UpdateRuneItem();
	else
		UpdateView();
}

void CDlgRoleRuneEnchanting::OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
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

void CDlgRoleRuneEnchanting::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	POINT pt = 
	{
		GET_X_LPARAM(lParam) - p->X,
		GET_Y_LPARAM(lParam) - p->Y,
	};
	GetGameUIMan()->m_ptLButtonDown = pt;
	GetGameUIMan()->InvokeDragDrop(this, pObj, pt);

	if (!m_pChk_01->IsChecked())
		return;

	int index = pObj->GetData();

	if (index < 0 || index >= (int)m_vecItems.size())
		return;

	RuneItem& runeItem = m_vecItems[index];
	runeItem.m_bSel = true;
	m_pImgHovers[index-m_nFirstLine*PACK_COL]->Show(runeItem.m_bSel);
}

void CDlgRoleRuneEnchanting::OnEventRButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if (!m_pChk_01->IsChecked())
		return;

	int index = pObj->GetData();

	if (index < 0 || index >= (int)m_vecItems.size())
		return;

	RuneItem& runeItem = m_vecItems[index];
	runeItem.m_bSel = false;
	m_pImgHovers[index-m_nFirstLine*PACK_COL]->Show(runeItem.m_bSel);
}

void CDlgRoleRuneEnchanting::OnCommand_Top(const char * szCommand)
{
	m_pScl_Item->SetBarLevel(0);
}

void CDlgRoleRuneEnchanting::OnCommand_Bottom(const char * szCommand)
{
	int count = m_vecItems.size();
	m_pScl_Item->SetBarLevel(count / 3 - 2);
}

void CDlgRoleRuneEnchanting::OnCommand_Prec(const char * szCommand)
{
	m_pScl_Item->SetBarLevel(m_pScl_Item->GetBarLevel() - 3);
}

void CDlgRoleRuneEnchanting::OnCommand_Next(const char * szCommand)
{
	m_pScl_Item->SetBarLevel(m_pScl_Item->GetBarLevel() + 3);
}

// 融合
void CDlgRoleRuneEnchanting::OnCommandEnchanting(const char* szCommand)
{
	CECInventory* pPack = m_nMainRuneWhere == 0 ? GetHostPlayer()->GetPack() : GetHostPlayer()->GetRune2013Pack();
	CECIvtrItem* pItem = pPack->GetItem(m_nMainRuneIndex);
	if (pItem->GetClassID() != CECIvtrItem::ICID_RUNE2013_ITEM)
		return;

	CECIvtrRune2013* pMainRune = (CECIvtrRune2013*)pItem;
	if (pMainRune->GetLevel() >= 10) // 到达等级上限10
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(18304));
		return;
	}
	int nMainQuality = pMainRune->GetDBEssence()->quality;
	int nMainLevel = pMainRune->GetLevel();

	bool bUseHighRune = false;
	int exp = 0;
	m_vecSelSlots.clear();
	abase::vector<RuneItem>::iterator it = m_vecItems.begin();
	abase::vector<RuneItem>::iterator ite = m_vecItems.end();
	for (; it != ite; ++ it)
	{
		RuneItem& runeItem = *it;
		if (runeItem.m_bSel)
		{
			exp += it->m_nExp;
			if (runeItem.m_bRune)
				bUseHighRune = runeItem.m_nQuality >= nMainQuality || runeItem.m_nLevel > nMainLevel;
			m_vecSelSlots.push_back(runeItem.m_nSlot);
		}
	}

	int maxExp = pMainRune->GetMaxLevelExp();
	int currentExp = pMainRune->GetEssence().exp;
	int needExp = maxExp - currentExp;
	if (exp > needExp)
	{
		if (CDlgRoleRuneEnchantingExp* pDlg = (CDlgRoleRuneEnchantingExp*)GetGameUIMan()->GetDialog("Win_Equip_RuneEnchanting_Exp"))
		{
			pDlg->ShowExpTip(exp, needExp);
			return;
		}
	}

	if (bUseHighRune)
	{
		GetGameUIMan()->MessageBox("Game_Rune2013_Enchanting", GetStringFromTable(18300), MB_YESNO);
	}
	else
	{
		Enchanting();
	}
}

// 取消
void CDlgRoleRuneEnchanting::OnCommand_Cancel(const char * szCommand)
{
	Show(false);
}

// 选择
void CDlgRoleRuneEnchanting::OnCommand_FreeSel(const char * szCommand)
{
	if (m_pChk_01->IsChecked())
	{
		m_pChk_02->Check(false);
		m_pChk_03->Check(false);
		m_pChk_04->Check(false);
	}
}

void CDlgRoleRuneEnchanting::OnCommand_SelLv1Rune(const char * szCommand)
{
	m_pChk_01->Check(false);
	bool bSel = m_pChk_02->IsChecked();
	abase::vector<RuneItem>::iterator it = m_vecItems.begin();
	abase::vector<RuneItem>::iterator ite = m_vecItems.end();
	for (; it != ite; ++ it)
	{
		if (it->m_bRune && it->m_nQuality == 0)
			it->m_bSel = bSel;
	}
}

void CDlgRoleRuneEnchanting::OnCommand_SelLv2Rune(const char * szCommand)
{
	m_pChk_01->Check(false);
	bool bSel = m_pChk_03->IsChecked();
	abase::vector<RuneItem>::iterator it = m_vecItems.begin();
	abase::vector<RuneItem>::iterator ite = m_vecItems.end();
	for (; it != ite; ++ it)
	{
		if (it->m_bRune && it->m_nQuality == 1)
			it->m_bSel = bSel;
	}
}

void CDlgRoleRuneEnchanting::OnCommand_SelFragment(const char * szCommand)
{
	m_pChk_01->Check(false);
	bool bSel = m_pChk_04->IsChecked();
	abase::vector<RuneItem>::iterator it = m_vecItems.begin();
	abase::vector<RuneItem>::iterator ite = m_vecItems.end();
	for (; it != ite; ++ it)
	{
		if (!it->m_bRune)
			it->m_bSel = bSel;
	}
}

void CDlgRoleRuneEnchanting::OnCommand_Help(const char * szCommand)
{
	//PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Equip_Rune_Enchanting_Help");
	//pDlg->Show(!pDlg->IsShow());
	//pDlg->AlignTo(this, AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
	AString strDlg = "Win_Equip_Rune_Enchanting_Help";
	GetGameUIMan()->CommonMessage("Win_HelpInfo","open",(WPARAM)(&strDlg),0);


	CECConfigs* pConfig = g_pGame->GetConfigs();
	EC_LOCAL_SETTING ls = pConfig->GetLocalSettings();
	ls.bFuWenPop4Showed = true;
	pConfig->SetLocalSettings(ls);
	pConfig->SaveLocalSetting();
	ShowRuneTipPop();
}


// 打开界面对指定符文进行升级
void CDlgRoleRuneEnchanting::EnchantingRune(int nIvtrIndex, int nWhere)
{
	m_nMainRuneIndex = nIvtrIndex;
	m_nMainRuneWhere = nWhere;
	if (IsShow())
	{
		UpdateRuneItem();
		UpdateView();
	}
	else
		Show(true);
}

// 更新包裹物品
void CDlgRoleRuneEnchanting::UpdateRuneItem()
{
	if (m_nMainRuneIndex < 0)
		return;

	ClearRuneItem();

	CECInventory* pPack = m_nMainRuneWhere == 0 ? GetHostPlayer()->GetPack() : GetHostPlayer()->GetRune2013Pack();
	const SPECIAL_ID_CONFIG& cfg = GetGame()->GetGameRun()->GetSpecialIDConfig();

	CECIvtrItem* pItem = pPack->GetItem(m_nMainRuneIndex);
	if (!pItem)
		return;
	pItem->Freeze(true);

	pPack = GetHostPlayer()->GetPack();
	
	int size = pPack->GetSize();

	for (int i = 0; i < size; ++ i)
	{
		if (m_nMainRuneWhere == 0 && i == m_nMainRuneIndex)
			continue;

		pItem = pPack->GetItem(i);
		if (!pItem)
			continue;
		if (pItem->IsLocked())
			continue;
		
		if (pItem->GetTemplateID() == cfg.id_rune2013_fragment[0] || pItem->GetTemplateID() == cfg.id_rune2013_fragment[1])
		{
			RuneItem runeItem;
			runeItem.m_nSlot = i;
			runeItem.m_nExp = pItem->GetCount() * FRAGEMENT_EXP;
			runeItem.m_bRune = false;
			runeItem.m_bSel = false;
			m_vecItems.push_back(runeItem);
			pItem->Freeze(true);
		}
		else if (pItem->GetClassID() == CECIvtrItem::ICID_RUNE2013_ITEM)
		{
			CECIvtrRune2013* pRune = (CECIvtrRune2013*)pItem;
			RuneItem runeItem;
			runeItem.m_nSlot = i;
			runeItem.m_nExp = pRune->GetEssence().exp + pRune->GetDBEssence()->self_exp;
			runeItem.m_nQuality = pRune->GetDBEssence()->quality;
			runeItem.m_nLevel = pRune->GetLevel();
			runeItem.m_bRune = true;
			runeItem.m_bSel = false;
			m_vecItems.push_back(runeItem);
			pItem->Freeze(true);
		}
	}

	int count = (int)m_vecItems.size();
	m_pScl_Item->SetScrollRange(0, (count - 1) / PACK_COL - PACK_SIZE / PACK_COL + 1);
	m_pScl_Item->SetBarLevel(0);
	m_pScl_Item->Show(count > PACK_SIZE);
	m_pScl_Item->SetBarLength(-1);
	m_pScl_Item->SetScrollStep(1);
	m_nFirstLine = 0;

	m_bNeedUpdateItem = false;
}

// 清除物品
void CDlgRoleRuneEnchanting::ClearRuneItem()
{
	CECInventory* pPack = GetHostPlayer()->GetPack();
	CECIvtrItem* pItem;
	abase::vector<RuneItem>::iterator it = m_vecItems.begin();
	abase::vector<RuneItem>::iterator ite = m_vecItems.end();
	for (; it != ite; ++ it)
	{
		if (pItem = pPack->GetItem(it->m_nSlot))
			pItem->Freeze(false);
	}
	m_vecItems.clear();

	if (pItem = pPack->GetItem(m_nMainRuneIndex))
		pItem->Freeze(false);
}

// 更新显示
void CDlgRoleRuneEnchanting::UpdateView()
{
	CECInventory* pPack = m_nMainRuneWhere == 0 ? GetHostPlayer()->GetPack() : GetHostPlayer()->GetRune2013Pack();
	CECIvtrItem* pMainRuneItem = pPack->GetItem(m_nMainRuneIndex);
	if (!pMainRuneItem)
	{
		Show(false);
		return;
	}
	GetGameUIMan()->SetIvtrInfo(m_pImgMain, pMainRuneItem);

	pPack = GetHostPlayer()->GetPack();

	if( m_nFirstLine != m_pScl_Item->GetBarLevel() )
		m_nFirstLine = m_pScl_Item->GetBarLevel();
	
	int nStart = m_nFirstLine * PACK_COL;
	unsigned int iIvtr;
	unsigned int iImg = 0;
	unsigned int len = m_vecItems.size();
	AUIImagePicture* pImg = NULL;
	CECIvtrItem* pItem;
	CECInventory* pInventory = GetHostPlayer()->GetPack();

	for (iIvtr = nStart; iIvtr < len && iImg < PACK_SIZE; ++ iIvtr, ++ iImg)
	{
		const RuneItem& runeItem = m_vecItems[iIvtr];
		pItem = pPack->GetItem(runeItem.m_nSlot);
		if (!pItem)
		{
			m_bNeedUpdateItem = true;
			return;
		}
		pImg = m_pImgItems[iImg];
		GetGameUIMan()->SetIvtrInfo(pImg, pItem, pItem->GetCount());
		pImg->SetData(iIvtr);
		m_pImgHovers[iImg]->Show(runeItem.m_bSel);
	}

	for (; iImg < PACK_SIZE; ++ iImg)
	{
		pImg = m_pImgItems[iImg];
		GetGameUIMan()->SetIvtrInfo(pImg, NULL);
		pImg->SetData(-1);
		m_pImgHovers[iImg]->Show(false);
	}

	UpdateExp();
}

// 更新经验显示
void CDlgRoleRuneEnchanting::UpdateExp()
{
	GetDlgItem("Btn_Enchanting")->Enable(false);

	CECInventory* pPack = m_nMainRuneWhere == 0 ? GetHostPlayer()->GetPack() : GetHostPlayer()->GetRune2013Pack();
	CECIvtrItem* pItem = pPack->GetItem(m_nMainRuneIndex);
	if (!pItem || pItem->GetClassID() != CECIvtrItem::ICID_RUNE2013_ITEM)
		return;
	CECIvtrRune2013* pMainRuneItem = (CECIvtrRune2013*)pItem;

	int exp = 0;
	abase::vector<RuneItem>::iterator it = m_vecItems.begin();
	abase::vector<RuneItem>::iterator ite = m_vecItems.end();

	for (; it != ite; ++ it)
	{
		if (it->m_bSel)
			exp += it->m_nExp;
	}

	wchar_t buffer[16];
	swprintf(buffer, L"%d", exp);
	m_pTxt_Exp->SetText(buffer);

	int nNextExp = pMainRuneItem->GetNextLevelExp();
	GetDlgItem("Img_Arrow")->Show(nNextExp > 0 && nNextExp <= exp + pMainRuneItem->GetEssence().exp);

	GetDlgItem("Btn_Enchanting")->Enable(exp > 0);
}

int CDlgRoleRuneEnchanting::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg == "set_rune")
	{
		int index = (int)wParam;
		if (index < 0 || index >= (int)m_vecItems.size())
			return 1;

		RuneItem& runeItem = m_vecItems[index];
		if (!runeItem.m_bRune)
			return 1;

		EnchantingRune(runeItem.m_nSlot);
		return 0;
	}
	else if (strMsg == "enchanting")
	{
		Enchanting();
		return 0;
	}
	return 1;
}

// 融合
void CDlgRoleRuneEnchanting::Enchanting()
{
	if (m_vecSelSlots.size() == 0)
		return;

	GetGameSession()->c2s_CmdFuWenUpgrade(m_nMainRuneIndex, m_nMainRuneWhere, (int)m_vecSelSlots.size(), m_vecSelSlots.begin());
	GetDlgItem("Gfx_01")->Show(false);
	GetDlgItem("Gfx_01")->Show(true);
	
	if (CDlgRoleRune* pDlg = (CDlgRoleRune*)GetGameUIMan()->GetDialog("Win_Equip_Rune"))
		pDlg->ShowEnchantingGfx();
}

// 飞升150后，第一次打开时需要显示符文泡泡提示
void CDlgRoleRuneEnchanting::ShowRuneTipPop()
{
	bool needShow = !g_pGame->GetConfigs()->GetLocalSettings().bFuWenPop4Showed;
	GetDlgItem("Img_Arrow01")->Show(needShow);
	GetDlgItem("Txt_PopHint01")->Show(needShow);
	GetDlgItem("Shape_PopHint01")->Show(needShow);
}


//////////////////////////////////////////////////////////////////////////
// 符文融合界面

AUI_BEGIN_COMMAND_MAP(CDlgRoleRuneEnchantingExp, CDlgBase)
AUI_ON_COMMAND("Btn_Yes",	OnCommandYes)
AUI_ON_COMMAND("Btn_No",	OnCommandNo)
AUI_END_COMMAND_MAP()

CDlgRoleRuneEnchantingExp::CDlgRoleRuneEnchantingExp()
{

}

CDlgRoleRuneEnchantingExp::~CDlgRoleRuneEnchantingExp()
{
	
}

bool CDlgRoleRuneEnchantingExp::OnInitDialog()
{
	m_pTxt_Exp01 = (AUILabel*)GetDlgItem("Txt_Exp01");
	m_pTxt_Exp02 = (AUILabel*)GetDlgItem("Txt_Exp02");
	m_pTxt_Exp03 = (AUILabel*)GetDlgItem("Txt_Exp03");

	return CDlgBase::OnInitDialog();
}

void CDlgRoleRuneEnchantingExp::ShowExpTip(int totalExp, int needExp)
{
	ACString szText;
	szText.Format(_AL("%d"), totalExp);
	m_pTxt_Exp01->SetText(szText);

	szText.Format(_AL("%d"), needExp);
	m_pTxt_Exp02->SetText(szText);

	szText.Format(_AL("%d"), totalExp - needExp);
	m_pTxt_Exp03->SetText(szText);

	Show(true);
}

void CDlgRoleRuneEnchantingExp::OnCommandYes(const char* szCommand)
{
	CDlgRoleRuneEnchanting* pDlg = (CDlgRoleRuneEnchanting*)GetGameUIMan()->GetDialog("Win_Equip_Rune_Enchanting");
	if (pDlg->IsShow())
		pDlg->Enchanting();
	Show(false);
}

void CDlgRoleRuneEnchantingExp::OnCommandNo(const char * szCommand)
{
	Show(false);
}