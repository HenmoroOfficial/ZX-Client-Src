// File		: DlgTalisman.cpp
// Creator	: Xiao Zhou
// Date		: 2007/4/25

#include "AFI.h"
#include "AUI\\AUICheckBox.h"
#include "AUI\\AUICTranslate.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUIRadioButton.h"
#include "AUI\\AUIComboBox.h"
#include "AUI\\AUISlider.h"
#include "DlgSkill.h"
#include "DlgTalisman.h"
#include "DlgQuickbar.h"
#include "EC_Algorithm.h"
#include "EC_FixedMsg.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "EC_Helper.h"
#include "EC_AutoHelper.h"
#include "EC_HostPlayer.h"
#include "EC_Shortcut.h"
#include "EC_Skill.h"
#include "EC_Inventory.h"
#include "EC_IvtrTalismanMain.h"
#include "elementdataman.h"
#include "EC_GameSession.h"
#include "EC_Configs.h"
#include "LuaEvent.h"
#include "EC_NewHelpGuide.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgTalisman, CDlgBase)

AUI_ON_COMMAND("Btn_Other",			OnCommandOther)
AUI_ON_COMMAND("Btn_BodyChange",	OnCommandBodyChange)
AUI_ON_COMMAND("Btn_Setting",		OnCommandSetting)
AUI_ON_COMMAND("zoarium",			OnCommandZoarium)
AUI_ON_COMMAND("fly",				OnCommandFly)
AUI_ON_COMMAND("Btn_ShieldAi_Add",			OnCommandAddMonster)
AUI_ON_COMMAND("Btn_ShieldAi_DeleteOne",	OnCommandDeleteMonster)
AUI_ON_COMMAND("Btn_ShieldAi_DeleteAll",	OnCommandDeleteAllMonster)
AUI_ON_COMMAND("Btn_Add",			OnCommandAddMatter)
AUI_ON_COMMAND("Btn_Delete",		OnCommandDeleteMatter)
AUI_ON_COMMAND("Btn_DeleteAll",		OnCommandDeleteAllMatter)
AUI_ON_COMMAND("Btn_QuickPickSet",	OnCommandQuickPick)
AUI_ON_COMMAND("changetype",		OnCommandChangeType)
AUI_ON_COMMAND("shield",			OnCommandShield)
AUI_ON_COMMAND("confirm",			OnCommandConfirm)
AUI_ON_COMMAND("Btn_Confirm",		OnCommandConfirmPick)
AUI_ON_COMMAND("Btn_Cancel",		OnCommandCancel)
AUI_ON_COMMAND("IDCANCEL",			OnCommandCancel)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgTalisman, CDlgBase)

AUI_ON_EVENT("Img_Skill*",				WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Item_*",					WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Img_Item*",				WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Img_Skill02",				WM_RBUTTONDOWN,		OnEventRButtonDown)

AUI_END_EVENT_MAP()


CDlgTalisman::CDlgTalisman()
{
}

CDlgTalisman::~CDlgTalisman()
{
}

bool CDlgTalisman::Release()
{
	int i;
	char szName[20];
	if( stricmp(m_szName, "Win_Talisman") == 0 )
	{
		for(i = 0; i < CDLGTALISMAN_SKILL_MAX; i++)
			SetTalismanItem(m_pImg_Skill[i], NULL);
		for(i = 0; i < 4; i++ )
		{
			sprintf(szName, "Item_%d", i + 1);
			SetTalismanItem(GetDlgItem(szName), NULL);
		}
	}
	if( stricmp(m_szName, "Win_TalismanSmall") == 0 )
	{
		for (i = 3; i < 10; i++)
		{
			sprintf(szName, "Img_Skill%02d", i);
			SetTalismanItem(GetDlgItem(szName), NULL);
		}
		for (i = 1; i <=3 ; i++)
		{
			sprintf(szName, "Img_Item%02d", i);
			SetTalismanItem(GetDlgItem(szName), NULL);
		}
	}

	return CDlgBase::Release();
}

void CDlgTalisman::OnShowDialog()
{
	if (stricmp(m_szName, "Win_Talisman") == 0)
	{
		GetGameUIMan()->GetDialog("Win_TalismanSmall")->Show(true);
	}
	else if (stricmp(m_szName, "Win_TalismanSettingAll") == 0)
	{
		PAUILISTBOX pListBox = (PAUILISTBOX)GetDlgItem("Lst_ShieldList");
		pListBox->ResetContent();
		CMatterSelector::ExcludeList *pList = GetGameRun()->GetHelper()->GetMonsterSelector()->GetExcludeList();
		for(int i = 0; i < (int)pList->size(); i++)
			pListBox->AddString((*pList)[i]);
		bool bEnableDeity = GetHostPlayer()->GetBasicProps().iDeityLevel>0;
		GetDlgItem("Txt_Dp")->Enable(bEnableDeity);
		GetDlgItem("Txt_DpTime")->Enable(bEnableDeity);
		if (bEnableDeity)
		{
			GetDlgItem("Txt_Dp")->SetColor(A3DCOLORRGB(255,255,255));
			GetDlgItem("Txt_DpTime")->SetColor(A3DCOLORRGB(255,255,255));
		}
		else
		{
			GetDlgItem("Txt_Dp")->SetColor(A3DCOLORRGB(128,128,128));
			GetDlgItem("Txt_DpTime")->SetColor(A3DCOLORRGB(128,128,128));
		}
	}
	else if (stricmp(m_szName, "Win_Talisman_AutoPick") == 0 )
	{
		int mask, lev1, lev2;
		GetGameRun()->GetHelper()->GetMatterSelector()->GetAutoPick(mask, lev1, lev2);
		((PAUICHECKBOX)GetDlgItem("Chk_1"))->Check(mask&1);
		((PAUICHECKBOX)GetDlgItem("Chk_2"))->Check((mask&(1<<1))>0);
		((PAUICHECKBOX)GetDlgItem("Chk_3"))->Check((mask&(1<<2))>0);
		((PAUICHECKBOX)GetDlgItem("Chk_4"))->Check((mask&(1<<3))>0);
		((PAUICOMBOBOX)GetDlgItem("Combo_1"))->SetCurSel(lev1-1);
		((PAUICOMBOBOX)GetDlgItem("Combo_2"))->SetCurSel(lev2-1);
	}
	else if (stricmp(m_szName, "Win_Talisman_AutoShield") == 0)
	{
		int mask, lev1, lev2;
		GetGameRun()->GetHelper()->GetMatterSelector()->GetAutoUnPick(mask, lev1, lev2);
		((PAUICHECKBOX)GetDlgItem("Chk_1"))->Check(mask&1);
		((PAUICHECKBOX)GetDlgItem("Chk_2"))->Check((mask&(1<<1))>0);
		((PAUICHECKBOX)GetDlgItem("Chk_3"))->Check((mask&(1<<2))>0);
		((PAUICHECKBOX)GetDlgItem("Chk_4"))->Check((mask&(1<<3))>0);
		((PAUICOMBOBOX)GetDlgItem("Combo_1"))->SetCurSel(lev1-1);
		((PAUICOMBOBOX)GetDlgItem("Combo_2"))->SetCurSel(lev2-1);
	}
}

bool CDlgTalisman::OnInitDialog()
{
	m_pPrgs_Power = (PAUIPROGRESS)GetDlgItem("Prgs_Power");
	m_pPrgs_Exp = (PAUIPROGRESS)GetDlgItem("Prgs_Exp");
	m_pPrgs_FlierExp = (PAUIPROGRESS)GetDlgItem("Prgs_FlierExp");
	
	int i;
	for(i = 0; i < CDLGTALISMAN_SKILL_ADDON_MAX; i++)
	{
		char szName[20];
		sprintf(szName, "Img_Skill%02d", i + 1);
		m_pImg_Skill[i] = (PAUIIMAGEPICTURE)GetDlgItem(szName);
	}
	m_dwChangeCoolTime = 0;
	m_bCanChange = false;

	if (stricmp(m_szName, "Win_Talisman_AutoPick") == 0 ||
		stricmp(m_szName, "Win_Talisman_AutoShield") == 0)
	{
		PAUICOMBOBOX pCombo = (PAUICOMBOBOX)GetDlgItem("Combo_1");
		for (i=0; i<10; i++)
		{
			pCombo->AddString(GetStringFromTable(9981+i));
		}
		pCombo = (PAUICOMBOBOX)GetDlgItem("Combo_2");
		for (i=0; i<10; i++)
		{
			pCombo->AddString(GetStringFromTable(9981+i));
		}
	}
	
	if(GetDlgItem("Shape_1")) GetDlgItem("Shape_1")->Show(false);
	if(GetDlgItem("Img_1")) GetDlgItem("Img_1")->Show(false);
	if(GetDlgItem("Txt_1")) GetDlgItem("Txt_1")->Show(false);

	m_iSkillIndex = 0;

	return true;
}

class CompareCECSkillShowOrder
{
public:
	bool operator ()(CECSkill * p1, CECSkill * p2)
	{
		return p1->GetShowOrder() < p2->GetShowOrder();
	}
};

void CDlgTalisman::OnTick()
{
	if( stricmp(m_szName, "Win_TalismanOption") == 0 )
		return;

	if( stricmp(m_szName, "Win_Talisman") == 0)
	{
		CECInventory *pInventory = GetHostPlayer()->GetEquipment();
		if (pInventory->GetItem(8) == NULL)
		{
			m_bCanChange = false;
		}
		PAUISTILLIMAGEBUTTON pBtn = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_BodyChange");
		if (GetHostPlayer()->IsTransfigured())
		{
			pBtn->SetPushed(true);
			GetDlgItem("Img_003")->Show(false);
		}
		else if ( GetHostPlayer()->CanDo(CECHostPlayer::CANDO_BODYCHANGE) && m_bCanChange && 
			(m_dwChangeCoolTime==0 || (m_dwChangeCoolTime>0 && !GetHostPlayer()->GetCoolTime(GP_CT_TRANSFIGURE)))
			&& !GetGameRun()->GetHelper()->IsRunning())
		{
			m_dwChangeCoolTime = 0;
			pBtn->Enable(true);
			pBtn->SetPushed(false);
			GetDlgItem("Img_003")->Show(true);
		}
		else
		{
			pBtn->Enable(false);
			GetDlgItem("Img_003")->Show(false);
		}
		m_bCanChange = false;
		
		//灵力增加冷却时间
		AUIClockIcon *pClock;
		PAUIIMAGEPICTURE pImgItem4 = (PAUIIMAGEPICTURE)GetDlgItem("Item_4");
		pClock = pImgItem4->GetClockIcon();
		pClock->SetProgressRange(0, 1);
		pClock->SetProgressPos(1);
		CECSCItem *pScItem = (CECSCItem*)pImgItem4->GetDataPtr("ptr_CECShortcut");
		if( pScItem )
		{
			CECIvtrItem *pItem = GetHostPlayer()->GetPack(pScItem->GetInventory())->GetItem(pScItem->GetIvtrSlot());
			int nMax;
			if (pItem && pItem->GetCoolTime(&nMax) > 0)
			{
				pClock->SetProgressRange(0, nMax);
				pClock->SetProgressPos(nMax - pItem->GetCoolTime());
				pClock->SetColor(A3DCOLORRGBA(0, 0, 0, 128));
			}
		}
	}
	int i;
	SkillVector vecSkill;
	CECShortcut *pSC;
	
	CECInventory *pPack = GetHostPlayer()->GetPack(IVTRTYPE_EQUIPPACK);
	CECIvtrTalismanMain *pItem = (CECIvtrTalismanMain*)pPack->GetItem(EQUIPIVTR_MAIN_TALISMAN);
	CECSkill *pSkill;
	if (pItem->GetRebornCount()>0)
	{
		pSkill = GetHostPlayer()->GetPositiveSkillByID(pItem->GetFixSkill());
		if(pSkill)
			vecSkill.push_back(pSkill);
		int idSkill2 = GetSkillByAddon(pItem->GetInfo(CECIvtrTalismanMain::TYPE_NEWADDON_ID_1+m_iSkillIndex));
		if (idSkill2>0)
		{
			if(pSkill=GetHostPlayer()->GetPositiveSkillByID(idSkill2))
				vecSkill.push_back(pSkill);
		}
	}
	else
	{
		for(i = 0; i < GetHostPlayer()->GetPositiveSkillNum(); i++ )
		{
			pSkill = GetHostPlayer()->GetPositiveSkillByIndex(i);
			if( pSkill->GetSkillLevel() > 0 && pSkill->IsCategory(GNET::CATEGORY_TALISMAN) )
				vecSkill.push_back(pSkill);
		}
	}
	
	BubbleSort(vecSkill.begin(), vecSkill.end(), CompareCECSkillShowOrder());
	if( stricmp(m_szName, "Win_Talisman") == 0 )
	{
		PAUISTILLIMAGEBUTTON pButton1 = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Zoarium");
		pButton1->SetPushed(GetGameRun()->GetHelper()->IsRunning());
		PAUISTILLIMAGEBUTTON pButton2 = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Fly");
		pButton2->SetPushed(GetHostPlayer()->IsFlying());

		for(i = 0; i < CDLGTALISMAN_SKILL_MAX; i++)
		{
			m_pImg_Skill[i]->SetHint(_AL(""));
			if( i < (int)vecSkill.size() )
			{
				CECSkill *pSkill = vecSkill[i];
				pSkill->SetItem(pItem);
				pSC = new CECSCSkill;
				if( pSC )
				{
					if( ((CECSCSkill*)pSC)->Init(pSkill) )
					{
						SetTalismanItem(m_pImg_Skill[i], pSC);
						const wchar_t *pszHint = pSC->GetDesc();
						if( pszHint )
						{
							AUICTranslate trans;
							AWString strHint = trans.Translate(pszHint);
							m_pImg_Skill[i]->SetHint(strHint);
						}
					}
				}
			}
			else
				SetTalismanItem(m_pImg_Skill[i], NULL);
		}

		CECInventory *pPack = GetHostPlayer()->GetPack(IVTRTYPE_EQUIPPACK);
		if( pPack->GetItem(EQUIPIVTR_MAIN_TALISMAN) && pPack->GetItem(EQUIPIVTR_MAIN_TALISMAN)->GetClassID() == CECIvtrItem::ICID_TALISMAN_MAINPART )
		{
			CECIvtrTalismanMain *pItem = (CECIvtrTalismanMain*)pPack->GetItem(EQUIPIVTR_MAIN_TALISMAN);
			CECIvtrTalismanMain *pItem1 = NULL;
			if( pPack->GetItem(EQUIPIVTR_WING) && pPack->GetItem(EQUIPIVTR_WING)->GetClassID() == CECIvtrItem::ICID_TALISMAN_MAINPART )
				pItem1 = (CECIvtrTalismanMain*)pPack->GetItem(EQUIPIVTR_WING);
			ACHAR szText[100];
			int nNow, nMax;
			nNow = GetHostPlayer()->GetBasicProps().iTalismanStamina;
			nMax = pItem->GetEssence().ess.stamina;
			if (nMax == nNow)
				LuaEvent::FirstRunHelper();
			a_sprintf(szText, GetStringFromTable(9160), nNow, nMax);
			m_bCanChange = nNow*2 > nMax;
			m_pPrgs_Power->SetProgress(nMax ? nNow * AUIPROGRESS_MAX / nMax : AUIPROGRESS_MAX);
			m_pPrgs_Power->SetHint(szText);
			pButton1->Enable(!GetHostPlayer()->IsFlying() && 
							 (nNow * 2 >= nMax || GetGameRun()->GetHelper()->IsRunning() || (GetHostPlayer()->GetRebornCount() == 0 && GetHostPlayer()->GetBasicProps().iLevel <= 90)) );
			GetDlgItem("Img_001")->Show(!GetGameRun()->GetHelper()->IsRunning() && 
										!GetHostPlayer()->IsFlying() && 
										(nNow * 2 >= nMax || ( GetHostPlayer()->GetRebornCount() == 0 && GetHostPlayer()->GetBasicProps().iLevel <= 90)) );
			pButton2->Enable(!GetGameRun()->GetHelper()->IsRunning() && pItem1 && (GetHostPlayer()->IsFlying() || nNow >= nMax));
			GetDlgItem("Img_002")->Show(!GetHostPlayer()->IsFlying() && !GetGameRun()->GetHelper()->IsRunning() && pItem1 && nNow >= nMax);
			nNow = pItem->GetEssence().ess.exp;
			nMax = GetHostPlayer()->GetTalismanLevelUpExp(pItem->GetEssence().ess.level + 1);
			if (nNow == nMax)
				LuaEvent::FirstTalismanUpgrade();
			a_sprintf(szText, GetStringFromTable(9161), nNow, nMax);
			m_pPrgs_Exp->SetProgress(nMax ? nNow * AUIPROGRESS_MAX / nMax : AUIPROGRESS_MAX);
			m_pPrgs_Exp->SetHint(szText);
			if( pItem1 )
			{
				nNow = pItem1->GetEssence().ess.exp;
				nMax = GetHostPlayer()->GetTalismanLevelUpExp(pItem1->GetEssence().ess.level + 1);
				if (nNow == nMax)
					LuaEvent::FirstFlySwordGrowthRateUp();
				a_sprintf(szText, GetStringFromTable(9162), nNow, nMax);
				m_pPrgs_FlierExp->SetProgress(nMax ? nNow * AUIPROGRESS_MAX / nMax : AUIPROGRESS_MAX);
				m_pPrgs_FlierExp->SetHint(szText);
			}
			else
			{
				m_pPrgs_FlierExp->SetProgress(0);
				m_pPrgs_FlierExp->SetHint(_AL(""));
			}
		}

		for(i = 0; i < CECGAMEUIMAN_MAX_TALISMANSLOT; i++ )
		{
			char szName[20];
			PAUIIMAGEPICTURE pImg;
			if( i < 3 )
			{
				sprintf(szName, "Item_%d", i + 1);
				pImg = (PAUIIMAGEPICTURE)GetDlgItem(szName);
			}
			else if (i==13)
			{
				sprintf(szName, "Item_4");
				pImg = (PAUIIMAGEPICTURE)GetDlgItem(szName);
			}
			else if( i < 10)
			{
				sprintf(szName, "Img_Skill%02d", i);
				pImg = (PAUIIMAGEPICTURE)GetGameUIMan()->GetDialog("Win_TalismanSmall")->GetDlgItem(szName);
			}
			else
			{
				sprintf(szName, "Img_Item%02d", i-9);
				pImg = (PAUIIMAGEPICTURE)GetGameUIMan()->GetDialog("Win_TalismanSmall")->GetDlgItem(szName);
			}
			if( !pImg )
				continue;

			CECShortcut *pSC;
			pSC = (CECShortcut*)pImg->GetDataPtr("ptr_CECShortcut");
			CDlgQuickBar::UpdateShortcut(pSC, pImg);
			if( !pImg->IsEnabled() )
				pImg->SetColor(A3DCOLORRGB(128, 128, 128));
			if( pSC )
			{
				const wchar_t *pszHint = pSC->GetDesc();
				if( pszHint)
				{
					AUICTranslate trans;
					AWString strHint = trans.Translate(pszHint);
					pImg->SetHint(strHint);
				}
				else
					pImg->SetHint(_AL(""));
			}
			else
			{
				if( i < 3 )
					pImg->SetText(_AL(""));
				pImg->SetHint(_AL(""));
			}
		}
	}
	else if( stricmp(m_szName, "Win_TalismanSmall") == 0 )
	{
		AlignTo(GetGameUIMan()->m_pDlgTalisman, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_CENTER, 
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_TOP);
		SetPosEx(GetPos().x, GetPos().y+(int)(10*GetGameUIMan()->GetWindowScale()));
	}
	else if( stricmp(m_szName, "Win_TalismanSettingAll") == 0 )
	{
		for(i = 0; i < 12; i++ )
		{
			char szName[20];
			if (i<9)
				sprintf(szName, "Img_Skill%02d", i + 1);
			else
				sprintf(szName, "Img_Item%02d", i - 8 );
			PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem(szName);
			PAUIIMAGEPICTURE pImg1;
			pImg1 = (PAUIIMAGEPICTURE)GetGameUIMan()->m_pDlgTalisman->GetDlgItem(szName);
			if (i>=2)
				pImg1 = (PAUIIMAGEPICTURE)GetGameUIMan()->GetDialog("Win_TalismanSmall")->GetDlgItem(szName);
			CECShortcut *pSC = (CECShortcut*)pImg1->GetDataPtr("ptr_CECShortcut");
			CDlgQuickBar::UpdateShortcut(pSC, pImg);
			if( !pImg->IsEnabled() )
				pImg->SetColor(A3DCOLORRGB(128, 128, 128));
			if( pSC )
			{
				const wchar_t *pszHint = pSC->GetDesc();
				if( pszHint)
				{
					AUICTranslate trans;
					AWString strHint = trans.Translate(pszHint);
					pImg->SetHint(strHint);
				}
				else
					pImg->SetHint(_AL(""));
			}
			else
				pImg->SetHint(_AL(""));
		}
	}
}

void CDlgTalisman::OnCommandOther(const char* szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_TalismanSmall");
	pDlg->Show(!pDlg->IsShow());
	pDlg->AlignTo(this, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT, 
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_TOP);
}

void CDlgTalisman::OnCommandBodyChange(const char* szCommand)
{
	CECIvtrItem* pItem = GetHostPlayer()->GetEquipment()->GetItem(EQUIPIVTR_FASHION_NOSE);
	if(!pItem)
		return;
	if (((CECIvtrChangeShapeCard*)pItem)->GetEssence().ess.stamina == 0)
	{
		GetGameUIMan()->AddChatMessage(GetStringFromTable(9122), GP_CHAT_MISC);
		return;
	}
	DATA_TYPE DataType;
	const void* pDataPtr = GetGame()->GetElementDataMan()->get_data_ptr(pItem->GetTemplateID(), ID_SPACE_ESSENCE, DataType);
	if (DataType != DT_CHANGE_SHAPE_CARD_ESSENCE)
		return;
	const CHANGE_SHAPE_CARD_ESSENCE* pEss = (const CHANGE_SHAPE_CARD_ESSENCE*)pDataPtr;

	if (GetHostPlayer()->IsTransfigured())
	{
		GetGameSession()->c2s_CmdStopTransfigure();
		if (pEss->epilogue[0])
			GetGameUIMan()->AddChatMessage(pEss->epilogue, GP_CHAT_SYSTEM);
	}	
	else
	{
		if (GetHostPlayer()->CanDo(CECHostPlayer::CANDO_BODYCHANGE))
		{
			GetGameSession()->c2s_CmdStartTransfigure();
			if (pEss->preface[0])
				GetGameUIMan()->AddChatMessage(pEss->preface, GP_CHAT_SYSTEM);
			m_dwChangeCoolTime = GetTickCount();
			GetDlgItem("Btn_BodyChange")->Enable(false);
		}
	}
}

void CDlgTalisman::OnCommandAddMatter(const char * szCommand)
{
	PAUILISTBOX pListBox = (PAUILISTBOX)GetDlgItem("Lst_Info");
	PAUIOBJECT pObj = GetDlgItem("Txt_ItemName");
	if( a_strlen(pObj->GetText()) > 0 )
	{
		int i;
		for(i = 0; i < pListBox->GetCount(); i++ )
		{
			int r = a_stricmp(pListBox->GetText(i), pObj->GetText());
			if( r == 0 )
			{
				GetGameUIMan()->MessageBox("", GetStringFromTable(925), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
				return;
			}
			else if( r > 0 )
				break;
		}
		if( ((PAUIRADIOBUTTON)GetDlgItem("Rdo_Exclude"))->IsChecked() )
		{
			pListBox->InsertString(i, pObj->GetText());
			CMatterSelector::ExcludeList *pList = GetGameRun()->GetHelper()->GetMatterSelector()->GetExcludeList();
			pList->insert(pList->begin() + i, pObj->GetText());
		}
		else if( ((PAUIRADIOBUTTON)GetDlgItem("Rdo_Include"))->IsChecked() )
		{
			pListBox->InsertString(i, pObj->GetText());
			CMatterSelector::IncludeList *pList = GetGameRun()->GetHelper()->GetMatterSelector()->GetIncludeList();
			pList->insert(pList->begin() + i, pObj->GetText());
		}
		
		pObj->SetText(_AL(""));
	}
}	
	
void CDlgTalisman::OnCommandAddMonster(const char * szCommand)
{
	PAUILISTBOX pListBox = (PAUILISTBOX)GetDlgItem("Lst_ShieldList");
	PAUIOBJECT	pObj = GetDlgItem("Edt_ShieldAi_Name");
	if( a_strlen(pObj->GetText()) > 0 )
	{
		int i;
		for(i = 0; i < pListBox->GetCount(); i++ )
		{
			int r = a_stricmp(pListBox->GetText(i), pObj->GetText());
			if( r == 0 )
			{
				GetGameUIMan()->MessageBox("", GetStringFromTable(925), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
				return;
			}
			else if( r > 0 )
				break;
		}
		pListBox->InsertString(i, pObj->GetText());
		CMonsterSelector::ExcludeList *pList = GetGameRun()->GetHelper()->GetMonsterSelector()->GetExcludeList();
		pList->insert(pList->begin() + i, pObj->GetText());
		
		pObj->SetText(_AL(""));
	}
}

void CDlgTalisman::OnCommandDeleteMatter(const char * szCommand)
{
	PAUILISTBOX pListBox = (PAUILISTBOX)GetDlgItem("Lst_Info");
	int nSel = pListBox->GetCurSel();
	if( nSel >= 0 && nSel < pListBox->GetCount() )
	{
		if( ((PAUIRADIOBUTTON)GetDlgItem("Rdo_Exclude"))->IsChecked() )
		{
			pListBox->DeleteString(nSel);
			CMatterSelector::ExcludeList *pList = GetGameRun()->GetHelper()->GetMatterSelector()->GetExcludeList();
			pList->erase(pList->begin() + nSel);
		}
		else if( ((PAUIRADIOBUTTON)GetDlgItem("Rdo_Include"))->IsChecked() )
		{
			pListBox->DeleteString(nSel);
			CMatterSelector::IncludeList *pList = GetGameRun()->GetHelper()->GetMatterSelector()->GetIncludeList();
			pList->erase(pList->begin() + nSel);
		}
	}
}	
	
void CDlgTalisman::OnCommandDeleteMonster(const char * szCommand)
{
	PAUILISTBOX pListBox = (PAUILISTBOX)GetDlgItem("Lst_ShieldList");
	int nSel = pListBox->GetCurSel();
	if( nSel >= 0 && nSel < pListBox->GetCount() )
	{
		pListBox->DeleteString(nSel);
		CMonsterSelector::ExcludeList *pList = GetGameRun()->GetHelper()->GetMonsterSelector()->GetExcludeList();
		pList->erase(pList->begin() + nSel);
	}
}

void CDlgTalisman::OnCommandDeleteAllMonster(const char * szCommand)
{
	GetGameUIMan()->MessageBox("Game_MonsterDeleteAll", GetStringFromTable(926), MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160));
}

void CDlgTalisman::OnCommandDeleteAllMatter(const char * szCommand)
{
	GetGameUIMan()->MessageBox("Game_MatterDeleteAll", GetStringFromTable(923), MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160));
}	

void CDlgTalisman::OnCommandQuickPick(const char * szCommand)
{
	PAUIDIALOG pDlg = NULL;
	if (GetCheckedRadioButton(1) == 3)
	{
		pDlg = GetGameUIMan()->GetDialog("Win_Talisman_AutoPick");
	}
	else if (GetCheckedRadioButton(1) == 4)
	{
		pDlg = GetGameUIMan()->GetDialog("Win_Talisman_AutoShield");
	}
	pDlg->Show(!pDlg->IsShow());
}

void CDlgTalisman::OnCommandConfirmPick(const char* szCommand)
{
	int mask, lev1, lev2;
	mask = (int)(((PAUICHECKBOX)GetDlgItem("Chk_1"))->IsChecked())+
	(((PAUICHECKBOX)GetDlgItem("Chk_2"))->IsChecked()<<1) +
	(((PAUICHECKBOX)GetDlgItem("Chk_3"))->IsChecked()<<2) +
	(((PAUICHECKBOX)GetDlgItem("Chk_4"))->IsChecked()<<3);
	lev1 = ((PAUICOMBOBOX)GetDlgItem("Combo_1"))->GetCurSel() + 1;
	lev2 = ((PAUICOMBOBOX)GetDlgItem("Combo_2"))->GetCurSel() + 1;
	if (stricmp(m_szName, "Win_Talisman_AutoPick") == 0)
		GetGameRun()->GetHelper()->GetMatterSelector()->SetAutoPick(mask, lev1, lev2);
	else
		GetGameRun()->GetHelper()->GetMatterSelector()->SetAutoUnPick(mask, lev1, lev2);
	Show(false);
}
void CDlgTalisman::OnCommandConfirm(const char* szCommand)
{
	GetGameRun()->GetHelper()->SetSkillDisabled(((PAUICHECKBOX)GetDlgItem("Chk_Skill"))->IsChecked());
	GetGameRun()->GetHelper()->SetHpRate(a_atoi(GetDlgItem("Txt_Hp")->GetText()));
	GetGameRun()->GetHelper()->SetHpDelay(int(atof(AC2AS(GetDlgItem("Txt_HpTime")->GetText())) * 1000));
	GetGameRun()->GetHelper()->SetMpRate(a_atoi(GetDlgItem("Txt_Mp")->GetText()));
	GetGameRun()->GetHelper()->SetMpDelay(int(atof(AC2AS(GetDlgItem("Txt_MpTime")->GetText())) * 1000));
	GetGameRun()->GetHelper()->SetDpRate(a_atoi(GetDlgItem("Txt_Dp")->GetText()));
	GetGameRun()->GetHelper()->SetDpDelay(int(atof(AC2AS(GetDlgItem("Txt_DpTime")->GetText())) * 1000));
	GetGameRun()->GetHelper()->SetSkillDelay(1, int(atof(AC2AS(GetDlgItem("Txt_Skill01Time")->GetText())) * 1000));
	GetGameRun()->GetHelper()->SetSkillDelay(2, int(atof(AC2AS(GetDlgItem("Txt_Skill02Time")->GetText())) * 1000));
	GetGameRun()->GetHelper()->SetSkillDelay(3, int(atof(AC2AS(GetDlgItem("Txt_Skill03Time")->GetText())) * 1000));
	GetGameRun()->GetHelper()->SetSkillDelay(4, int(atof(AC2AS(GetDlgItem("Txt_Skill04Time")->GetText())) * 1000));
	GetGameRun()->GetHelper()->SetSkillDelay(5, int(atof(AC2AS(GetDlgItem("Txt_Skill05Time")->GetText())) * 1000));
	GetGameRun()->GetHelper()->SetSkillDelay(6, int(atof(AC2AS(GetDlgItem("Txt_Skill06Time")->GetText())) * 1000));
	GetGameRun()->GetHelper()->SetSkillDelay(7, int(atof(AC2AS(GetDlgItem("Txt_Skill07Time")->GetText())) * 1000));
	GetGameRun()->GetHelper()->SetSkillDelay(8, int(atof(AC2AS(GetDlgItem("Txt_Skill08Time")->GetText())) * 1000));
	GetGameRun()->GetHelper()->SetSkillDelay(9, int(atof(AC2AS(GetDlgItem("Txt_Skill09Time")->GetText())) * 1000));
	GetGameRun()->GetHelper()->SetUseItemDelay(1, int(atof(AC2AS(GetDlgItem("Txt_ItemTime01")->GetText())) * 1000));
	GetGameRun()->GetHelper()->SetUseItemDelay(2, int(atof(AC2AS(GetDlgItem("Txt_ItemTime02")->GetText())) * 1000));
	GetGameRun()->GetHelper()->SetUseItemDelay(3, int(atof(AC2AS(GetDlgItem("Txt_ItemTime03")->GetText())) * 1000));
	int i;
	PAUILISTBOX pListBox = (PAUILISTBOX)GetDlgItem("Lst_Info");
	if( ((PAUIRADIOBUTTON)GetDlgItem("Rdo_All"))->IsChecked() )
		GetGameRun()->GetHelper()->GetMatterSelector()->SetSelectType(CMatterSelector::MATTER_ALL);
	else if( ((PAUIRADIOBUTTON)GetDlgItem("Rdo_None"))->IsChecked() )
		GetGameRun()->GetHelper()->GetMatterSelector()->SetSelectType(CMatterSelector::MATTER_NONE);
	else if( ((PAUIRADIOBUTTON)GetDlgItem("Rdo_Exclude"))->IsChecked() )
	{
		GetGameRun()->GetHelper()->GetMatterSelector()->SetSelectType(CMatterSelector::MATTER_EXCLUDE);
		CMatterSelector::ExcludeList *pList = GetGameRun()->GetHelper()->GetMatterSelector()->GetExcludeList();
		pList->clear();
		for(i = 0; i < pListBox->GetCount(); i++)
			pList->push_back(pListBox->GetText(i));
	}
	else
	{
		GetGameRun()->GetHelper()->GetMatterSelector()->SetSelectType(CMatterSelector::MATTER_INCLUDE);
		CMatterSelector::IncludeList *pList = GetGameRun()->GetHelper()->GetMatterSelector()->GetIncludeList();
		pList->clear();
		for(i = 0; i < pListBox->GetCount(); i++)
			pList->push_back(pListBox->GetText(i));
	}
	//save helper settings
	GetGameRun()->GetHelper()->Save(GetHostPlayer()->GetCharacterID());
	
	//save helper monster settings
	CMonsterSelector * pMonsterSelector = GetGameRun()->GetHelper()->GetMonsterSelector();
	if (pMonsterSelector)
	{
		pMonsterSelector->Save(GetHostPlayer()->GetCharacterID());
		pMonsterSelector->SaveMonsterList(GetHostPlayer()->GetCharacterID());
	}
	//save helper matter selector include and exclude list
	CMatterSelector * pMatterSelector = GetGameRun()->GetHelper()->GetMatterSelector();
	if (pMatterSelector)
	{
		pMatterSelector->Save(GetHostPlayer()->GetCharacterID(), CMatterSelector::MATTER_INCLUDE);
		pMatterSelector->Save(GetHostPlayer()->GetCharacterID(), CMatterSelector::MATTER_EXCLUDE);
	}
	
	int nType = 0;
	PAUICHECKBOX pCheck;
	pCheck = (PAUICHECKBOX)GetDlgItem("Chk_001");
	if( pCheck->IsChecked() )
		nType |= CMonsterSelector::MONSTER_NORMAL;
	pCheck = (PAUICHECKBOX)GetDlgItem("Chk_002");
	if( pCheck->IsChecked() )
		nType |= CMonsterSelector::MONSTER_ELITE;
	pCheck = (PAUICHECKBOX)GetDlgItem("Chk_003");
	if( pCheck->IsChecked() )
		nType |= CMonsterSelector::MONSTER_BOSS;
	GetGameRun()->GetHelper()->GetMonsterSelector()->SetTargetType(nType);
	//		pCheck = (PAUICHECKBOX)GetDlgItem("Chk_004");
	//		GetGameRun()->GetHelper()->GetMonsterSelector()->SetFullHp(pCheck->IsChecked());
	pCheck = (PAUICHECKBOX)GetDlgItem("Chk_005");
	GetGameRun()->GetHelper()->GetMonsterSelector()->SetTaskOnly(pCheck->IsChecked());
	pCheck = (PAUICHECKBOX)GetDlgItem("Chk_006");
//	GetGameRun()->GetHelper()->SetAnswerQuestion(pCheck->IsChecked());
	PAUISLIDER pSldRange = (PAUISLIDER)GetDlgItem("Prg_RangeLimit");
	GetGameRun()->GetHelper()->SetAnchorRange(pSldRange->GetLevel());
	PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("Edt_RangeInput");
	int nRange = a_atoi(pEdt->GetText());
	if (nRange>=1&&nRange<=20)
	{
		GetGameRun()->GetHelper()->GetMonsterSelector()->SetAttackRange(nRange);
	}
	pListBox = (PAUILISTBOX)GetDlgItem("Lst_ShieldList");
	CMonsterSelector::ExcludeList *pList = GetGameRun()->GetHelper()->GetMonsterSelector()->GetExcludeList();
	pList->clear();
	for(i = 0; i < pListBox->GetCount(); i++)
		pList->push_back(pListBox->GetText(i));
	Show(false);
}

void CDlgTalisman::OnCommandCancel(const char* szCommand)
{
	if( stricmp(m_szName, "Win_Talisman") != 0 )
		Show(false);
}

void CDlgTalisman::OnCommandSetting(const char* szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_TalismanSettingAll");
	pDlg->Show(!pDlg->IsShow());
	int nType = GetGameRun()->GetHelper()->GetMonsterSelector()->GetTargetType();
	PAUICHECKBOX pCheck;
	pCheck = (PAUICHECKBOX)pDlg->GetDlgItem("Chk_001");
	pCheck->Check((nType & CMonsterSelector::MONSTER_NORMAL) != 0);
	pCheck = (PAUICHECKBOX)pDlg->GetDlgItem("Chk_002");
	pCheck->Check((nType & CMonsterSelector::MONSTER_ELITE) != 0);
	pCheck = (PAUICHECKBOX)pDlg->GetDlgItem("Chk_003");
	pCheck->Check((nType & CMonsterSelector::MONSTER_BOSS) != 0);
//	pCheck = (PAUICHECKBOX)pDlg->GetDlgItem("Chk_004");
//	pCheck->Check(GetGameRun()->GetHelper()->GetMonsterSelector()->IsFullHp());
	pCheck = (PAUICHECKBOX)pDlg->GetDlgItem("Chk_005");
	pCheck->Check(GetGameRun()->GetHelper()->GetMonsterSelector()->IsTaskOnly());
 	pCheck = (PAUICHECKBOX)pDlg->GetDlgItem("Chk_006");
 	// todo pCheck->Check(GetGameRun()->GetHelper()->IsAnswerQuestion());
	
	PAUISLIDER pSldRange = (PAUISLIDER)pDlg->GetDlgItem("Prg_RangeLimit");
	pSldRange->SetLevel(GetGameRun()->GetHelper()->GetAnchorRange());
	PAUIEDITBOX pEdit = (PAUIEDITBOX)pDlg->GetDlgItem("Edt_RangeInput");
	ACHAR szText[20];
	a_sprintf(szText, _AL("%d"), GetGameRun()->GetHelper()->GetMonsterSelector()->GetAttackRange());
	pEdit->SetText(szText);

	((PAUICHECKBOX)pDlg->GetDlgItem("Chk_Skill"))->Check(GetGameRun()->GetHelper()->IsSkillDisabled());
	a_sprintf(szText, _AL("%d"), GetGameRun()->GetHelper()->GetHpRate());
	pDlg->GetDlgItem("Txt_Hp")->SetText(szText);
	a_sprintf(szText, _AL("%g"), float(GetGameRun()->GetHelper()->GetHpDelay()) / 1000.0f);
	pDlg->GetDlgItem("Txt_HpTime")->SetText(szText);
	a_sprintf(szText, _AL("%d"), GetGameRun()->GetHelper()->GetMpRate());
	pDlg->GetDlgItem("Txt_Mp")->SetText(szText);
	a_sprintf(szText, _AL("%g"), float(GetGameRun()->GetHelper()->GetMpDelay()) / 1000.0f);
	pDlg->GetDlgItem("Txt_MpTime")->SetText(szText);
	a_sprintf(szText, _AL("%d"), GetGameRun()->GetHelper()->GetDpRate());
	pDlg->GetDlgItem("Txt_Dp")->SetText(szText);
	a_sprintf(szText, _AL("%g"), float(GetGameRun()->GetHelper()->GetDpDelay()) / 1000.0f);
	pDlg->GetDlgItem("Txt_DpTime")->SetText(szText);
	a_sprintf(szText, _AL("%g"), float(GetGameRun()->GetHelper()->GetSkillDelay(1)) / 1000.0f);
	pDlg->GetDlgItem("Txt_Skill01Time")->SetText(szText);
	a_sprintf(szText, _AL("%g"), float(GetGameRun()->GetHelper()->GetSkillDelay(2)) / 1000.0f);
	pDlg->GetDlgItem("Txt_Skill02Time")->SetText(szText);
	a_sprintf(szText, _AL("%g"), float(GetGameRun()->GetHelper()->GetSkillDelay(3)) / 1000.0f);
	pDlg->GetDlgItem("Txt_Skill03Time")->SetText(szText);
	a_sprintf(szText, _AL("%g"), float(GetGameRun()->GetHelper()->GetSkillDelay(4)) / 1000.0f);
	pDlg->GetDlgItem("Txt_Skill04Time")->SetText(szText);
	a_sprintf(szText, _AL("%g"), float(GetGameRun()->GetHelper()->GetSkillDelay(5)) / 1000.0f);
	pDlg->GetDlgItem("Txt_Skill05Time")->SetText(szText);
	a_sprintf(szText, _AL("%g"), float(GetGameRun()->GetHelper()->GetSkillDelay(6)) / 1000.0f);
	pDlg->GetDlgItem("Txt_Skill06Time")->SetText(szText);
	a_sprintf(szText, _AL("%g"), float(GetGameRun()->GetHelper()->GetSkillDelay(7)) / 1000.0f);
	pDlg->GetDlgItem("Txt_Skill07Time")->SetText(szText);
	a_sprintf(szText, _AL("%g"), float(GetGameRun()->GetHelper()->GetSkillDelay(8)) / 1000.0f);
	pDlg->GetDlgItem("Txt_Skill08Time")->SetText(szText);
	a_sprintf(szText, _AL("%g"), float(GetGameRun()->GetHelper()->GetSkillDelay(9)) / 1000.0f);
	pDlg->GetDlgItem("Txt_Skill09Time")->SetText(szText);
	a_sprintf(szText, _AL("%g"), float(GetGameRun()->GetHelper()->GetUseItemDelay(1)) / 1000.0f);
	pDlg->GetDlgItem("Txt_ItemTime01")->SetText(szText);
	a_sprintf(szText, _AL("%g"), float(GetGameRun()->GetHelper()->GetUseItemDelay(2)) / 1000.0f);
	pDlg->GetDlgItem("Txt_ItemTime02")->SetText(szText);
	a_sprintf(szText, _AL("%g"), float(GetGameRun()->GetHelper()->GetUseItemDelay(3)) / 1000.0f);
	pDlg->GetDlgItem("Txt_ItemTime03")->SetText(szText);
	pDlg->CheckRadioButton(1, GetGameRun()->GetHelper()->GetMatterSelector()->GetSelectType() + 1);
	pDlg->OnCommand("changetype");
}

void CDlgTalisman::OnCommandShield(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_TalismanSettingAll");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgTalisman::OnCommandChangeType(const char * szCommand)
{
	int i;
	PAUILISTBOX pListBox = (PAUILISTBOX)GetDlgItem("Lst_Info");
	PAUIEDITBOX pEdit = (PAUIEDITBOX)GetDlgItem("Txt_ItemName");
	PAUISTILLIMAGEBUTTON pAdd = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Add");
	PAUISTILLIMAGEBUTTON pDelete = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Delete");
	PAUISTILLIMAGEBUTTON pDeleteAll = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_DeleteAll");
	PAUISTILLIMAGEBUTTON pQuickPickSet = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_QuickPickSet");
	pListBox->ResetContent();
	if( ((PAUIRADIOBUTTON)GetDlgItem("Rdo_All"))->IsChecked() || ((PAUIRADIOBUTTON)GetDlgItem("Rdo_None"))->IsChecked() )
	{
		pListBox->ResetContent();
		pListBox->Enable(false);
		pEdit->Enable(false);
		pEdit->SetText(_AL(""));
		pAdd->Enable(false);
		pDelete->Enable(false);
		pDeleteAll->Enable(false);
		pQuickPickSet->Show(false);
	}
	else if( ((PAUIRADIOBUTTON)GetDlgItem("Rdo_Exclude"))->IsChecked() )
	{
		pListBox->ResetContent();
		pListBox->Enable(true);
		pEdit->Enable(true);
		pAdd->Enable(true);
		pDelete->Enable(true);
		pDeleteAll->Enable(true);
		CMatterSelector::ExcludeList *pList = GetGameRun()->GetHelper()->GetMatterSelector()->GetExcludeList();
		for(i = 0; i < (int)pList->size(); i++)
			pListBox->AddString((*pList)[i]);
		pQuickPickSet->Show(true);
		PAUIDIALOG pDlgShow = GetGameUIMan()->GetDialog("Win_Talisman_AutoPick");
		if (pDlgShow->IsShow())
		{
			pDlgShow->Show(false);
			GetGameUIMan()->GetDialog("Win_Talisman_AutoShield")->Show(true);
			GetGameUIMan()->GetDialog("Win_Talisman_AutoShield")->SetPosEx(pDlgShow->GetPos().x, pDlgShow->GetPos().y);
		}
	}
	else
	{
		pListBox->ResetContent();
		pListBox->Enable(true);
		pEdit->Enable(true);
		pAdd->Enable(true);
		pDelete->Enable(true);
		pDeleteAll->Enable(true);
		CMatterSelector::IncludeList *pList = GetGameRun()->GetHelper()->GetMatterSelector()->GetIncludeList();
		for(i = 0; i < (int)pList->size(); i++)
			pListBox->AddString((*pList)[i]);
		pQuickPickSet->Show(true);
		PAUIDIALOG pDlgShow = GetGameUIMan()->GetDialog("Win_Talisman_AutoShield");
		if (pDlgShow->IsShow())
		{
			pDlgShow->Show(false);
			GetGameUIMan()->GetDialog("Win_Talisman_AutoPick")->Show(true);
			GetGameUIMan()->GetDialog("Win_Talisman_AutoPick")->SetPosEx(pDlgShow->GetPos().x, pDlgShow->GetPos().y);
		}
	}
}

void CDlgTalisman::OnCommandZoarium(const char* szCommand)
{
	if (!GetGame()->GetConfigs()->IsAutoHelperEnable())
		return;
	
	if (GetHostPlayer()->IsFlying())
		return;

	CECInventory *pPack = GetHostPlayer()->GetPack(IVTRTYPE_EQUIPPACK);
	CECIvtrTalismanMain *pItem = (CECIvtrTalismanMain*)pPack->GetItem(EQUIPIVTR_MAIN_TALISMAN);
	if (!pItem)
		return;
	CECIvtrTalismanMain *pItem1 = NULL;
	if( pPack->GetItem(EQUIPIVTR_WING) && pPack->GetItem(EQUIPIVTR_WING)->GetClassID() == CECIvtrItem::ICID_TALISMAN_MAINPART )
		pItem1 = (CECIvtrTalismanMain*)pPack->GetItem(EQUIPIVTR_WING);
	int nNow, nMax;
	nNow = GetHostPlayer()->GetBasicProps().iTalismanStamina;
	nMax = pItem->GetEssence().ess.stamina;

	if ( !GetGameRun()->GetHelper()->IsRunning() && nNow * 2 < nMax &&	(GetHostPlayer()->GetRebornCount() > 0 || GetHostPlayer()->GetBasicProps().iLevel > 90) )
		return;

	if( GetGameRun()->GetHelper()->IsRunning() )
		GetGameSession()->bot_begin(0, 0);
	else
	{
		if (GetHostPlayer()->GetRebornInfo().size()>0 || GetHostPlayer()->GetBasicProps().iLevel>90)
		{
			int index = 0;
			if (!GetDlgItem("Item_1")->GetDataPtr("ptr_CECShortcut"))
			{
				index |= 0x1;
			}
			if (!GetDlgItem("Item_2")->GetDataPtr("ptr_CECShortcut"))
			{
				index |= 0x2;
			}
			if (!GetDlgItem("Item_3")->GetDataPtr("ptr_CECShortcut"))
			{
				index |= 0x4;
			}
			if (index)
			{
				GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(3630 + index), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			}
		}
		if (GetGame()->GetConfigs()->GetGameSettings().bAtk_NoWhite)
			GetGameSession()->bot_begin(1, 0);//GetGameRun()->GetHelper()->IsAnswerQuestion());
		else
			GetGameUIMan()->MessageBox("Game_Help_WhiteDeal", GetGameUIMan()->GetStringFromTable(9322), MB_YESNO, A3DCOLORRGBA(160, 255, 255, 255));
	}

	NewHelpGuide::FinishGuide(GUIDE_HELP_USEGAMEAUTOMATIC_2);
}

void CDlgTalisman::OnCommandFly(const char* szCommand)
{
	if( !GetHostPlayer()->IsFlying() )
		GetGameSession()->c2s_CmdStartFly();
	else
		GetGameSession()->c2s_CmdStopFly();
}

void CDlgTalisman::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( !pObj->GetDataPtr("ptr_CECShortcut") )
		return;
	
	if( stricmp(m_szName, "Win_Talisman") != 0 && stricmp(m_szName, "Win_TalismanSmall") != 0 )
		return;
	
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	POINT pt = 
	{
		GET_X_LPARAM(lParam) - p->X,
		GET_Y_LPARAM(lParam) - p->Y,
	};
	
	GetGameUIMan()->InvokeDragDrop(this, pObj, pt);
}

void CDlgTalisman::OnEventRButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	CECInventory *pPack = GetHostPlayer()->GetPack(IVTRTYPE_EQUIPPACK);
	CECIvtrTalismanMain *pItem = (CECIvtrTalismanMain*)pPack->GetItem(EQUIPIVTR_MAIN_TALISMAN);
	if (pItem->GetInfo(CECIvtrTalismanMain::TYPE_NEWADDON_ID_2)==0)
		return;

	CDlgSkill *pDlgSkill = (CDlgSkill*)GetGameUIMan()->GetDialog("Win_Skill");
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_TalismanQuickBar");
	if (pDlg->IsShow())
	{
		pDlg->Show(false);
		return;
	}
	int i;
	for (i=0; i<5; ++i)
	{
		int iSkillId = GetSkillByAddon(pItem->GetInfo(CECIvtrTalismanMain::TYPE_NEWADDON_ID_1+i));
		if (iSkillId==0)
		{
			break;
		}
		PAUIIMAGEPICTURE pImg;
		AString strName;
		strName.Format("Item_%d", i+1);
		pImg = (PAUIIMAGEPICTURE)pDlg->GetDlgItem(strName);

		CECSkill *pSkill = GetHostPlayer()->GetPositiveSkillByID(iSkillId);
		if (pSkill)
		{
			pDlgSkill->SetImage(pImg, pSkill);
		}
		else
		{
			ACString strText;
			strText.Format(_AL("error skill id: %d"), iSkillId);
			GetGameUIMan()->MessageBox("", strText);
			break;
		}
	}
	if (i>0)
	{
		pDlg->Show(true);
		pDlg->SetPosEx(pObj->GetPos().x, pObj->GetPos().y-pDlg->GetSize().cy);
	}
	for (; i<5; ++i)
	{
		PAUIIMAGEPICTURE pImg;
		AString strName;
		strName.Format("Item_%d", i+1);
		pImg = (PAUIIMAGEPICTURE)pDlg->GetDlgItem(strName);
		pDlgSkill->SetImage(pImg, NULL);
	}
}

int CDlgTalisman::UseTalismanSkill(int nSlot)
{
	char szName[20];
	sprintf(szName, "Img_Skill%02d", nSlot);
	PAUIIMAGEPICTURE pImg;
	if (nSlot<=2)
		pImg = (PAUIIMAGEPICTURE)GetGameUIMan()->GetDialog("Win_Talisman")->GetDlgItem(szName);
	else
		pImg = (PAUIIMAGEPICTURE)GetGameUIMan()->GetDialog("Win_TalismanSmall")->GetDlgItem(szName);
	if( !pImg || !pImg->GetDataPtr("ptr_CECShortcut") )
		return 0;
	CECShortcut *pSC = (CECShortcut*)pImg->GetDataPtr("ptr_CECShortcut");
	if( pSC )
	{
		pSC->Execute();
		if( pSC->GetType() == CECShortcut::SCT_COMMAND )
			return 0;
		else if( pSC->GetType() == CECShortcut::SCT_SKILL )
			return ((CECSCSkill*)pSC)->GetSkill()->GetSkillID();
	}
	return 0;
}

int CDlgTalisman::CheckTalismanSkill(int nSlot)
{
	char szName[20];
	sprintf(szName, "Img_Skill%02d", nSlot);
	PAUIIMAGEPICTURE pImg;
	if (nSlot<=2)
		pImg = (PAUIIMAGEPICTURE)GetGameUIMan()->GetDialog("Win_Talisman")->GetDlgItem(szName);
	else
		pImg = (PAUIIMAGEPICTURE)GetGameUIMan()->GetDialog("Win_TalismanSmall")->GetDlgItem(szName);

	if( !pImg || !pImg->GetDataPtr("ptr_CECShortcut") )
		return 0;
	CECShortcut *pSC = (CECShortcut*)pImg->GetDataPtr("ptr_CECShortcut");
	if( pSC )
	{
		if( pSC->GetType() == CECShortcut::SCT_COMMAND )
			return 0;
		else if( pSC->GetType() == CECShortcut::SCT_SKILL )
			return ((CECSCSkill*)pSC)->GetSkill()->GetSkillID();
	}
	return 0;
}

void CDlgTalisman::UseTalismanItem(int nSlot)
{
	char szName[20];
	if ( stricmp(m_szName, "Win_Talisman") == 0 )
	{
		if (nSlot>9)
		{
			CDlgTalisman *pDlg = (CDlgTalisman*)GetGameUIMan()->GetDialog("Win_TalismanSmall");
			pDlg->UseTalismanItem(nSlot);
			return;
		}
		sprintf(szName, "Item_%d", nSlot);
	}
	else
		sprintf(szName, "Img_Item%02d", nSlot-9);
	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem(szName);
	if( !pImg || !pImg->GetDataPtr("ptr_CECShortcut") )
		return;
	CECSCItem *pItemSC = (CECSCItem*)pImg->GetDataPtr("ptr_CECShortcut");
	if( pItemSC )
	{
		if( !pItemSC->Execute() )
		{
			delete pItemSC;
			pImg->SetDataPtr(NULL);
		}
	}
}

void CDlgTalisman::SetTalismanItem(AUIObject* pObj, CECShortcut *pSC)
{
	CECShortcut *pOldSC = (CECShortcut*)pObj->GetDataPtr("ptr_CECShortcut");
	if( pOldSC )
		delete pOldSC;
	PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)pObj;
	CDlgQuickBar::UpdateShortcut(pSC, pImage);
}

void CDlgTalisman::SetTalismanItem(char* szName, CECShortcut *pSC)
{
	AUIObject* pObj = GetDlgItem(szName);
	if (pObj==NULL)
	{
		pObj=GetGameUIMan()->GetDialog("Win_TalismanSmall")->GetDlgItem(szName);
	}
	SetTalismanItem(pObj, pSC);
}

void CDlgTalisman::UpdateMovedItemSC(int tidItem, int iSrcIvtr, int iSrcSlot, int iDstIvtr, int iDstSlot)
{
	for(int i = 0; i < 4; i++ )
	{
		char szName[20];
		if( stricmp(m_szName, "Win_Talisman") == 0 )
			sprintf(szName, "Item_%d", i + 1);
		else if( stricmp(m_szName, "Win_TalismanSmall") == 0 )
			sprintf(szName, "Img_Item%02d", i + 1);
		else
			return;
		PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem(szName);
		if (!pImg)
		{
			continue;
		}

		CECSCItem *pSC = (CECSCItem*)pImg->GetDataPtr("ptr_CECShortcut");
		if( pSC && pSC->GetInventory() == iSrcIvtr && pSC->GetItemTID() == tidItem && pSC->GetIvtrSlot() == iSrcSlot )
			pSC->MoveItem(iDstIvtr, iDstSlot);
	}
}

void CDlgTalisman::UpdateRemovedItemSC(int tidItem, int iIvtr, int iSlot, int iSameItem)
{
	for(int i = 0; i < 4; i++ )
	{
		char szName[20];
		
		if( stricmp(m_szName, "Win_Talisman") == 0 )
			sprintf(szName, "Item_%d", i + 1);
		else if( stricmp(m_szName, "Win_TalismanSmall") == 0 )
			sprintf(szName, "Img_Item%02d", i + 1);
		else
			return;
		PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem(szName);
		if (!pImg)
		{
			continue;
		}

		CECSCItem *pSC = (CECSCItem*)pImg->GetDataPtr("ptr_CECShortcut");
		if( pSC && pSC->GetInventory() == iIvtr && pSC->GetItemTID() == tidItem && pSC->GetIvtrSlot() == iSlot )
		{
			if( iSameItem >= 0 )
				pSC->MoveItem(iIvtr, iSameItem);
			else
				SetTalismanItem(pImg, NULL);
		}
	}
}

void CDlgTalisman::UpdateExchangedItemSC(int tidItem1, int iIvtr1, int iSlot1, int tidItem2, int iIvtr2, int iSlot2)
{
	for(int i = 0; i < 4; i++ )
	{
		char szName[20];
		if( stricmp(m_szName, "Win_Talisman") == 0 )
			sprintf(szName, "Item_%d", i + 1);
		else if( stricmp(m_szName, "Win_TalismanSmall") == 0 )
			sprintf(szName, "Img_Item%02d", i + 1);
		else
			return;
		
		PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem(szName);
		if (!pImg)
		{
			continue;
		}
		
		CECSCItem *pSC = (CECSCItem*)pImg->GetDataPtr("ptr_CECShortcut");
		if( pSC && pSC->GetInventory() == iIvtr1 && pSC->GetItemTID() == tidItem1 && pSC->GetIvtrSlot() == iSlot1 )
			pSC->MoveItem(iIvtr2, iSlot2);
		else if( pSC && pSC->GetInventory() == iIvtr2 && pSC->GetItemTID() == tidItem2 && pSC->GetIvtrSlot() == iSlot2 )
			pSC->MoveItem(iIvtr1, iSlot1);
	}
}

void CDlgTalisman::UpdateRemovedSkill(int id)
{
	if (stricmp(m_szName, "Win_TalismanSmall")!=0)
		return;

	for(int i = 3; i <= 9; i++ )
	{
		AString strName;
		strName.Format("Img_Skill%02d", i);
		
		PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		if (!pImg)
		{
			continue;
		}
		
		CECShortcut *pSC = (CECShortcut*)pImg->GetDataPtr("ptr_CECShortcut");
		if (pSC && pSC->GetType() == CECShortcut::SCT_SKILL)
		{
			CECSCSkill *pSCSkill = (CECSCSkill*)pSC;
			if( pSCSkill->GetSkill() && pSCSkill->GetSkill()->GetSkillID() == id)
			{
				delete pSCSkill;
				pImg->SetDataPtr(NULL, "ptr_CECShortcut");
				pImg->SetData(id, "toadd_id");
			}
		}
	}
}

void CDlgTalisman::UpdateExchangeSkill(int id, CECSkill *pSkill)
{
	if (stricmp(m_szName, "Win_TalismanSmall")!=0)
		return;
	
	for(int i = 3; i <= 9; i++ )
	{
		AString strName;
		strName.Format("Img_Skill%02d", i);
		
		PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		if (!pImg)
		{
			continue;
		}

		if ((int)pImg->GetData("toadd_id")==id)
		{
			pImg->SetData(0, "toadd_id");
			CECSCSkill *pNewSC = new CECSCSkill;
			if( pNewSC )
			{
				if( pNewSC->Init(pSkill) )
					SetTalismanItem(pImg, pNewSC);
				else
					delete pNewSC;
			}
		}
	}
}

int CDlgTalisman::GetSkillByAddon(int idAddOn)
{
	int iRet = 0;
	if (idAddOn==0)
		return iRet;

	DATA_TYPE dt;
	EQUIPMENT_ADDON * pAddOn = (EQUIPMENT_ADDON *) GetGame()->GetElementDataMan()->get_data_ptr(idAddOn, ID_SPACE_ADDON, dt);
	if( dt != DT_EQUIPMENT_ADDON )
		return iRet;
	
	if (pAddOn->type != 38)
		return iRet;

	return pAddOn->param1;
}


AUI_BEGIN_EVENT_MAP(CDlgTalismanSkillSel, CDlgBase)

AUI_ON_EVENT("Item_*",				WM_LBUTTONDOWN,		OnEventLButtonDown)

AUI_END_EVENT_MAP()

void CDlgTalismanSkillSel::OnTick()
{
	PAUIDIALOG pDlgMouseOn;
	PAUIOBJECT pObjMouseOn;
	GetGameUIMan()->GetMouseOn(&pDlgMouseOn, &pObjMouseOn);
	if (!pDlgMouseOn || (pDlgMouseOn!=this && pDlgMouseOn!=GetGameUIMan()->GetDialog("Win_Talisman")))
	{
		Show(false);
	}
}

void CDlgTalismanSkillSel::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( !pObj->GetDataPtr("ptr_CECSkill") )
		return;

	int index = atoi(pObj->GetName()+strlen("Item_"))-1;

	CDlgTalisman *pDlg = (CDlgTalisman*)GetGameUIMan()->GetDialog("Win_Talisman");
	pDlg->SetSkillSel(index);
	Show(false);
}