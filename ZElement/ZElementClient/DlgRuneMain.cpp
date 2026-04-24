// Filename	: DlgRuneMain.cpp
// Creator	: Fu Chonggang
// Date		: 2012.4.21

#include "AUI\\AUILabel.h"
#include "AUI\\AUIProgress.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIProgress.h"
#include "AUI\\AUICTranslate.h"
#include "AUI\\AUIComboBox.h"
#include "AUI\\AUIListBox.h"
#include "AFI.h"
#include "AUI\\CSplit.h"
#include "DlgRuneMain.h"
#include "DlgRuneOperate.h"
#include "EC_Game.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_IvtrRune.h"
#include "EC_FixedMsg.h"
#include "elementdataman.h"
#include "ExpTypes.h"
#include "EC_GameRun.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgRuneMain, CDlgBase)

AUI_ON_COMMAND("Btn_Default",			OnCommandDefault)
AUI_ON_COMMAND("Btn_Upgrade",			OnCommandUpgrade)
AUI_ON_COMMAND("Btn_Spilt",				OnCommandDecompose)
AUI_ON_COMMAND("Btn_Refresh",			OnCommandRefine)
AUI_ON_COMMAND("Btn_RuneOn",			OnCommandInsert)
AUI_ON_COMMAND("Btn_HoleErase",			OnCommandChange)
AUI_ON_COMMAND("Btn_RuneErase",			OnCommandErase)
AUI_ON_COMMAND("Btn_LvUp",				OnCommandLevelUp)
AUI_ON_COMMAND("Btn_Help",				OnCommandHelp)
AUI_ON_COMMAND("Btn_CheckAll",			OnCommandHelp1)
AUI_ON_COMMAND("Btn_Custom01",			OnCommandCustomizeRune1)
AUI_ON_COMMAND("Btn_Custom02",			OnCommandCustomizeRune2)
AUI_ON_COMMAND("Btn_Custom03",			OnCommandCustomizeRune3)
AUI_ON_COMMAND("Btn_Custom04",			OnCommandCustomizeRune4)

AUI_END_COMMAND_MAP()

const int	CDlgRuneMain::ms_idTmpConfig = 536;
const int	CDlgRuneMain::ms_iDefaultPropValue[50] = {
//	0		1		2		3		4		5		6		7		8		9
	0,		58800,	67200,	6930,	3990,	840,	630,	840,	630,	16800,
	294,	1890,	336,	2310,	630,	630,	630,	630,	630,	630,
	2730,	336,	630,	630,	630,	630,	630,	630,	420,	630,
	630,	840,	630,	840,
};

const int	CDlgRuneMain::ms_iRuneValueLevel[6] = {0, 2000, 10000, 60000, 200000, 2000000000};


CDlgRuneMain::CDlgRuneMain()
{
	m_pRune = NULL;
}

CDlgRuneMain::~CDlgRuneMain()
{
}

bool CDlgRuneMain::OnInitDialog()
{
	m_pLvUp = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_LvUp");
	m_pImgIcon = (PAUIIMAGEPICTURE)GetDlgItem("Img_Pic");
	m_pPrgRuneValue = (PAUIPROGRESS)GetDlgItem("Prgs_Bless");
	m_pLabRuneValue = (PAUILABEL)GetDlgItem("Txt_PrgsBless");
	m_pDlgInService = NULL;

	AUIOBJECT_SETPROPERTY p;
	strcpy(p.fn,"icon\\Equip_Yuanying_CustomImg.tga");
	m_pCustomImage1 = (PAUIIMAGEPICTURE)GetDlgItem("Img_CustomCover01");
	m_pCustomImage1->SetProperty("Image File",&p);
	m_pCustomImage1->SetFrameNumber(2);
	m_pCustomImage2 = (PAUIIMAGEPICTURE)GetDlgItem("Img_CustomCover02");
	m_pCustomImage2->SetProperty("Image File",&p);
	m_pCustomImage2->SetFrameNumber(2);
	m_pCustomImage3 = (PAUIIMAGEPICTURE)GetDlgItem("Img_CustomCover03");
	m_pCustomImage3->SetProperty("Image File",&p);
	m_pCustomImage3->SetFrameNumber(2);
	m_pCustomImage4 = (PAUIIMAGEPICTURE)GetDlgItem("Img_CustomCover04");
	m_pCustomImage4->SetProperty("Image File",&p);
	m_pCustomImage4->SetFrameNumber(2);

	
	return true;
}

void CDlgRuneMain::OnShowDialog()
{
	m_iIndex = GetData("rune_index");
	m_pRune = (CECIvtrRune*)GetDataPtr("CECIvtrRune_Ptr");
}

void CDlgRuneMain::OnTick()
{
	m_pRune = (CECIvtrRune*)GetHostPlayer()->GetEquipment()->GetItem(EQUIPIVTR_RUNE);
	if (!m_pRune)
	{
		Show(false);
		return;
	}
	UpdateDisplay();
}

void CDlgRuneMain::OnCommandDefault(const char* szCommand)
{
	CDlgRuneReset *pDlg = (CDlgRuneReset*)GetGameUIMan()->GetDialog("Win_Equip_Yuanying_Default");
	if(m_pDlgInService)
		m_pDlgInService->Show(false);
	m_pDlgInService = pDlg;
	pDlg->Show(true);
	pDlg->SetRune();
	pDlg->AlignTo(this, 
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
}

void CDlgRuneMain::OnCommandUpgrade(const char* szCommand)
{
	CDlgRuneLevelUp *pDlg = (CDlgRuneLevelUp*)GetGameUIMan()->GetDialog("Win_Equip_Yuanying_Upgrade");
	if(m_pDlgInService)
		m_pDlgInService->Show(false);
	m_pDlgInService = pDlg;
	pDlg->Show(true);
	GetGameUIMan()->GetDialog("Win_Inventory")->Show(true);
	pDlg->SetRune();
	pDlg->AlignTo(this, 
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
	GetGameUIMan()->GetDialog("Win_Inventory")->AlignTo(pDlg, 
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
			AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_BOTTOM);
}

void CDlgRuneMain::OnCommandLevelUp(const char* szCommand)
{
	GetGameSession()->c2s_CmdRuneLevelUp(EQUIPIVTR_RUNE);
}

void CDlgRuneMain::OnCommandCustomizeRune1(const char* szCommand)
{
	CDlgRuneCustom* pDlgRuneCustom = (CDlgRuneCustom*)GetGameUIMan()->GetDialog("Win_Equip_Yuanying_Custom01");
	pDlgRuneCustom->SetCustomType(CDlgRuneCustom::CUSTOMIZE_TYPE1);
	pDlgRuneCustom->Show(true);
}

void CDlgRuneMain::OnCommandCustomizeRune2(const char* szCommand)
{
	CDlgRuneCustom* pDlgRuneCustom = (CDlgRuneCustom*)GetGameUIMan()->GetDialog("Win_Equip_Yuanying_Custom02");
	pDlgRuneCustom->SetCustomType(CDlgRuneCustom::CUSTOMIZE_TYPE2);
	pDlgRuneCustom->Show(true);
}

void CDlgRuneMain::OnCommandCustomizeRune3(const char* szCommand)
{
	CDlgRuneCustom* pDlgRuneCustom = (CDlgRuneCustom*)GetGameUIMan()->GetDialog("Win_Equip_Yuanying_Custom02");
	pDlgRuneCustom->SetCustomType(CDlgRuneCustom::CUSTOMIZE_TYPE3);
	pDlgRuneCustom->Show(true);
}

void CDlgRuneMain::OnCommandCustomizeRune4(const char* szCommand)
{
	CDlgRuneCustom* pDlgRuneCustom = (CDlgRuneCustom*)GetGameUIMan()->GetDialog("Win_Equip_Yuanying_Custom02");
	pDlgRuneCustom->SetCustomType(CDlgRuneCustom::CUSTOMIZE_TYPE4);
	pDlgRuneCustom->Show(true);
}

void CDlgRuneMain::OnCommandHelp(const char* szCommand)
{
	//PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Equip_Yuanying_help");
	//pDlg->Show(!pDlg->IsShow());
	AString strDlg = "Win_Equip_Yuanying_help";
	GetGameUIMan()->CommonMessage("Win_HelpInfo","open",(WPARAM)(&strDlg),0);
}

void CDlgRuneMain::OnCommandHelp1(const char* szCommand)
{
	//PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Equip_Yuanying_CheckAllhelp");
	//pDlg->Show(!pDlg->IsShow());
	AString strDlg = "Win_Equip_Yuanying_CheckAllhelp";
	GetGameUIMan()->CommonMessage("Win_HelpInfo","open",(WPARAM)(&strDlg),0);
}

void CDlgRuneMain::OnCommandDecompose(const char* szCommand)
{
	if (m_pRune->GetEssence().level<10)
	{
		GetGameUIMan()->MessageBox(13252);
		return;
	}

	PAUIDIALOG pDlg;
	GetGameUIMan()->MessageBox("Game_RuneDecompose", GetGameUIMan()->GetStringFromTable(13253), MB_YESNO, 
		A3DCOLORRGBA(255,255,255,166), &pDlg);
	pDlg->SetData(m_iIndex);
}

void CDlgRuneMain::OnCommandRefine(const char* szCommand)
{
	CDlgRuneRefine *pDlg = (CDlgRuneRefine*)GetGameUIMan()->GetDialog("Win_Equip_Yuanying_Refresh");
	if(m_pDlgInService)
		m_pDlgInService->Show(false);
	m_pDlgInService = pDlg;
	pDlg->Show(true);
	pDlg->SetRune();
	pDlg->AlignTo(this, 
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
}

void CDlgRuneMain::OnCommandInsert(const char* szCommand)
{
	CDlgRuneFillSlot *pDlg = (CDlgRuneFillSlot*)GetGameUIMan()->GetDialog("Win_Equip_Yuanying_RuneOn");
	if(m_pDlgInService)
		m_pDlgInService->Show(false);
	m_pDlgInService = pDlg;
	pDlg->Show(true);
	GetGameUIMan()->GetDialog("Win_Inventory")->Show(true);
	pDlg->SetRune();
	pDlg->AlignTo(this, 
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
	GetGameUIMan()->GetDialog("Win_Inventory")->AlignTo(pDlg, 
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
			AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_BOTTOM);
}

void CDlgRuneMain::OnCommandChange(const char* szCommand)
{
	int i;
	for (i=0; i<5; ++i)
	{
		// 有镶嵌符文不能改变插槽
		if(m_pRune->GetEssence().rune_stone[i]>0)
		{
			GetGameUIMan()->MessageBox(13250);
			return;
		}
	}
	CDlgSlotErase *pDlg = (CDlgSlotErase*)GetGameUIMan()->GetDialog("Win_Equip_Yuanying_HoleErase");
	if(m_pDlgInService)
		m_pDlgInService->Show(false);
	m_pDlgInService = pDlg;
	pDlg->Show(true);
	pDlg->SetRune();
	pDlg->AlignTo(this, 
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
}

void CDlgRuneMain::OnCommandErase(const char* szCommand)
{
	CDlgRuneErase *pDlg = (CDlgRuneErase*)GetGameUIMan()->GetDialog("Win_Equip_Yuanying_RuneErase");
	if(m_pDlgInService)
		m_pDlgInService->Show(false);
	m_pDlgInService = pDlg;
	pDlg->Show(true);
	pDlg->SetRune();
	pDlg->AlignTo(this, 
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
}

void CDlgRuneMain::DisplayOneProp(int id, int value, AString strSuffix, AUIDialog* pDlg)
{
	if (!pDlg)
	{
		pDlg = this;
	}
	PAUILABEL pLabValue		= (PAUILABEL)pDlg->GetDlgItem("Lab_"+strSuffix);
	PAUILABEL pTxtValue		= (PAUILABEL)pDlg->GetDlgItem("Txt_"+strSuffix);
	PAUILABEL pLabQuality	= (PAUILABEL)pDlg->GetDlgItem("Txt_Prgs"+strSuffix);
	PAUIPROGRESS pPrgQuality= (PAUIPROGRESS)pDlg->GetDlgItem("Prgs_"+strSuffix);
	if(pLabValue)
		pLabValue->Show(id>0);
	if(pTxtValue)
		pTxtValue->Show(id>0);
	pLabQuality->Show(id>0);
	pPrgQuality->Show(id>0);
	if (id<=0)
		return;

	if(pLabValue)
		pLabValue->SetText(GetGame()->GetItemDesc()->GetWideString(ITEMDESC_EQUIPADDON_61+id));
	ACString strText = m_pRune->GetPropString(id, value);
	ACString strTextTotal = m_pRune->GetPropString(id, value*m_pRune->GetEssence().level);
	ACString strPrgText = m_pRune->GetPropString(id, ms_iDefaultPropValue[id]);
	if(pTxtValue)
		pTxtValue->SetText(strTextTotal);
	pLabQuality->SetText(strText + _AL("/") + strPrgText);
	pPrgQuality->SetProgress(AUIPROGRESS_MAX * value / ms_iDefaultPropValue[id]);
	ACString strHint;
	if (pDlg==this)
	{
		strPrgText = m_pRune->GetPropString(id, ms_iDefaultPropValue[id]*20);
		strHint.Format(GetGameUIMan()->GetStringFromTable(13254), 
			GetGame()->GetItemDesc()->GetWideString(ITEMDESC_EQUIPADDON_61+id), strTextTotal, strPrgText);
	}
	else
		strHint = _AL("");
	pPrgQuality->SetHint(strHint);
}

bool CDlgRuneMain::IsInService()
{
	return IsShow() ||
		GetGameUIMan()->GetDialog("Win_Equip_Yuanying_Upgrade")->IsShow() ||
		GetGameUIMan()->GetDialog("Win_Equip_Yuanying_Refresh")->IsShow() ||
		GetGameUIMan()->GetDialog("Win_Equip_Yuanying_RuneOn")->IsShow() ||
		GetGameUIMan()->GetDialog("Win_Equip_Yuanying_HoleErase")->IsShow() ||
		GetGameUIMan()->GetDialog("Win_Equip_Yuanying_RuneErase")->IsShow() ||
		GetGameUIMan()->GetDialog("Win_Equip_Yuanying_Default")->IsShow() ||
		GetGameUIMan()->GetDialog("Win_Equip_Yuanying_HoleErase")->IsShow() ||
		GetGameUIMan()->GetDialog("Win_Equip_Yuanying_RuneOn")->IsShow();
}

void CDlgRuneMain::UpdateDisplay()
{
	if (!m_pRune)
	{
		return;
	}

	ACString strText;
	PAUILABEL pLab;
	pLab = (PAUILABEL)GetDlgItem("Txt_Name");	//名称
	pLab->SetText(m_pRune->GetName());
	pLab = (PAUILABEL)GetDlgItem("Txt_Lv");	//等级
	strText.Format(_AL("%d"), m_pRune->GetEssence().level);
	pLab->SetText(strText);
	pLab = (PAUILABEL)GetDlgItem("Txt_Quality");	//综合评价
	pLab->SetText(GetGame()->GetItemDesc()->GetWideString(ITEMDESC_RUNEQUALITY1+m_pRune->GetEssence().quality));

	int nReputation = GetHostPlayer()->GetRuneValue();
	int level = 0;
	while( nReputation >= ms_iRuneValueLevel[level + 1] )
		level++;
	int nNowReputation = nReputation - ms_iRuneValueLevel[level];
	int nMaxReputation = ms_iRuneValueLevel[level + 1] - ms_iRuneValueLevel[level];

	m_pPrgRuneValue->SetProgress(int(double(nNowReputation) / nMaxReputation * AUIPROGRESS_MAX));
	m_pLabRuneValue->SetText(GetGameUIMan()->GetStringFromTable(13280+level));
	strText.Format(GetGameUIMan()->GetStringFromTable(13262), level+2);
	if ((level+2)>=5)
	{
		strText = GetGameUIMan()->GetStringFromTable(13268+level+2-5) + ACString(_AL("\r")) + strText;
	}
	ACString strRune;
	strRune.Format(GetGameUIMan()->GetStringFromTable(19933), nReputation, ms_iRuneValueLevel[level + 1]);
	strText = strText + ACString(_AL("\r")) + strRune;
	m_pPrgRuneValue->SetHint(strText);

	
	AString strFile;
	af_GetFileTitle(m_pRune->GetIconFile(), strFile);
	strFile.MakeLower();
	m_pImgIcon->SetCover(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_INVENTORY),
		GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_INVENTORY, strFile));
	
	elementdataman* pDataman = GetGame()->GetElementDataMan();
	DATA_TYPE dtType = DT_INVALID;
	const void* p = pDataman->get_data_ptr(CDlgRuneMain::ms_idTmpConfig, ID_SPACE_CONFIG, dtType);
	assert(dtType==DT_RUNE_DATA_CONFIG);
	const RUNE_DATA_CONFIG* pConfig = (const RUNE_DATA_CONFIG*)p;
	PAUIDIALOG pDlgMouseOn;
	PAUIOBJECT pObjMouseOn;
	GetGameUIMan()->GetMouseOn(&pDlgMouseOn, &pObjMouseOn);
	if (pObjMouseOn==m_pImgIcon)
	{
		AUICTranslate trans;
		m_pImgIcon->SetHint(trans.Translate(m_pRune->GetDesc()));
	}
	else if (pObjMouseOn==GetDlgItem("Btn_LvUp"))
	{
		if (m_pRune->GetEssence().level==20)
		{
			strText = GetGameUIMan()->GetStringFromTable(13258);
		}
		else
			strText.Format(GetGameUIMan()->GetStringFromTable(13257), 
				pConfig->equip_req[m_pRune->GetEssence().level-1].rebount_count_req==0 ? _AL("") : GetGameUIMan()->GetStringFromTable(7012),
				pConfig->equip_req[m_pRune->GetEssence().level-1].lv_req);
		GetDlgItem("Btn_LvUp")->SetHint(strText);
	}

	// 当前属性
	DisplayOneProp(m_pRune->GetEssence().prop[0].id, m_pRune->GetEssence().prop[0].value, "A01");
	DisplayOneProp(m_pRune->GetEssence().prop[1].id, m_pRune->GetEssence().prop[1].value, "A02");
	DisplayOneProp(m_pRune->GetEssence().prop[2].id, m_pRune->GetEssence().prop[2].value, "A03");
	DisplayOneProp(m_pRune->GetEssence().prop[3].id, m_pRune->GetEssence().prop[3].value, "A04");
	DisplayOneProp(m_pRune->GetEssence().prop[4].id, m_pRune->GetEssence().prop[4].value, "A05");
	DisplayOneProp(m_pRune->GetEssence().prop[5].id, m_pRune->GetEssence().prop[5].value, "A06");
	DisplayOneProp(m_pRune->GetEssence().prop[6].id, m_pRune->GetEssence().prop[6].value, "A07");
	DisplayOneProp(m_pRune->GetEssence().prop[7].id, m_pRune->GetEssence().prop[7].value, "A08");

	// 符语
	GetDlgItem("Shape_Cover")->Show(m_pRune->GetEssence().hole==0);
	GetDlgItem("Lab_Rune")->Show(m_pRune->GetEssence().hole>0);
	GetDlgItem("Lab_Effect")->Show(m_pRune->GetEssence().hole>0);
	GetDlgItem("Btn_Spilt")->Enable(m_pRune->GetEssence().level>=10);
	GetDlgItem("Btn_RuneOn")->Enable(m_pRune->GetEssence().level>=10);
	GetDlgItem("Btn_HoleErase")->Enable(m_pRune->GetEssence().level>=10);
	GetDlgItem("Btn_RuneErase")->Enable(m_pRune->GetEssence().level>=10);
	int i;
	PAUIIMAGEPICTURE pImg;
	AString strName;
	for (i=0; i<5; ++i)
	{
		strName.Format("Img_Rune%d", i+1);
		pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		GetGameUIMan()->SetIvtrInfo(pImg, m_pRune->GetEssence().rune_stone[i]);
		if (m_pRune->GetEssence().rune_stone[i]==0)
		{
			pImg->SetHint(GetGameUIMan()->GetStringFromTable(13266));
		}
		strName.Format("Img_RuneShape%d", i+1);
		pImg->Show(i<m_pRune->GetEssence().hole);
		pImg->SetRenderTop(true);
		pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		pImg->Show(i<m_pRune->GetEssence().hole);
	}
	PAUILABEL pLabPropName = (PAUILABEL)GetDlgItem("Txt_Rune");
	PAUILABEL pLabPropEffect = (PAUILABEL)GetDlgItem("Txt_Effect");
	pLabPropName->SetText(_AL(""));
	pLabPropEffect->SetText(_AL(""));
	if (m_pRune->GetRuneSuiteId()>0)
	{
		const void* pData = pDataman->get_data_ptr(m_pRune->GetRuneSuiteId(),ID_SPACE_CONFIG, dtType);
		if(dtType == DT_RUNE_COMB_PROPERTY)
		{
			const RUNE_COMB_PROPERTY* pRuneSoul = (const RUNE_COMB_PROPERTY*)pData;
			pLabPropName->SetText(pRuneSoul->name);
			strText = _AL("");
			for (i=0; i<9; i++)
			{
				if (pRuneSoul->addons[i]==0)
				{
					break;
				}
				const void *pDataProp = pDataman->get_data_ptr(pRuneSoul->addons[i], ID_SPACE_ADDON, dtType);
				if (dtType != DT_EQUIPMENT_ADDON)
				{
					ASSERT(dtType == DT_EQUIPMENT_ADDON);
					continue;
				}
				
				const EQUIPMENT_ADDON* pAddOn = (const EQUIPMENT_ADDON*)pDataProp;
				strText += m_pRune->GetEquipAddonDesc(pAddOn->id, pAddOn->type, pAddOn->param1, pAddOn->param2, pAddOn->param3, 1);
				strText += _AL("\r");
			}
			pLabPropEffect->SetText(strText);
		}
	}

	//经验
	PAUIPROGRESS pPrgExp = (PAUIPROGRESS)GetDlgItem("Prgs_Lv");
	PAUILABEL	pLabExp	= (PAUILABEL)GetDlgItem("Txt_PrgsLv");
	if (m_pRune->GetEssence().level>=20)
	{
		pPrgExp->SetProgress(AUIPROGRESS_MAX);
		strText.Format(_AL("%d"), m_pRune->GetEssence().exp);
		pLabExp->SetText(strText);
	}
	else
	{
		pPrgExp->SetProgress(AUIPROGRESS_MAX * m_pRune->GetEssence().exp / pConfig->exp[m_pRune->GetEssence().level-1]);
		strText.Format(_AL("%d/%d"), m_pRune->GetEssence().exp, pConfig->exp[m_pRune->GetEssence().level-1]);
		pLabExp->SetText(strText);
	}
	int iLevel = GetHostPlayer()->GetBasicProps().iLevel + 150*GetHostPlayer()->GetRebornCount() ;
	int iLvReq = pConfig->equip_req[m_pRune->GetEssence().level-1].lv_req+pConfig->equip_req[m_pRune->GetEssence().level-1].rebount_count_req*150;
	m_pLvUp->Enable(m_pRune->GetEssence().exp>=(int)pConfig->exp[m_pRune->GetEssence().level-1] && 
		iLevel>=iLvReq && m_pRune->GetEssence().level<20);

	//能否定制元婴
	int iCustomizeRune = GetHostPlayer()->GetRuneCustomize();
	GetDlgItem("Btn_Custom01")->Enable(nReputation >= 25000 && (!(iCustomizeRune & 0x01)));
	GetDlgItem("Btn_Custom02")->Enable(nReputation >= 60000 && (!(iCustomizeRune & 0x02)));
	GetDlgItem("Btn_Custom03")->Enable(nReputation >= 200000 && (!(iCustomizeRune & 0x04)));
	GetDlgItem("Btn_Custom04")->Enable(nReputation >= 400000 && (!(iCustomizeRune & 0x08)));
	
	ACString strHint;
	ACString strCustom;
	strCustom = GetStringFromTable(19934);
	
	strHint.Format(GetStringFromTable(19935), 3, 5, 25000);
	if(nReputation < 25000)
	{
		GetDlgItem("Btn_Custom01")->Show(false);
		m_pCustomImage1->Show(true);
		m_pCustomImage1->SetHint(strHint);
		m_pCustomImage1->FixFrame(0);
		m_pCustomImage1;
	}
	else
	{
		if(!(iCustomizeRune & 0x01))
		{
			GetDlgItem("Btn_Custom01")->Show(true);
			m_pCustomImage1->Show(false);
		}
		else
		{
			GetDlgItem("Btn_Custom01")->Show(false);
			m_pCustomImage1->Show(true);
			m_pCustomImage1->FixFrame(1);
			strHint = strHint + strCustom;
			m_pCustomImage1->SetHint(strHint);
		}
	}
	
	strHint.Format(GetStringFromTable(19935), 4, 6, 60000);
	if(nReputation < 60000)
	{
		GetDlgItem("Btn_Custom02")->Show(false);
		m_pCustomImage2->Show(true);
		m_pCustomImage2->SetHint(strHint);
		m_pCustomImage2->FixFrame(0);
	}
	else
	{
		if(!(iCustomizeRune & 0x02))
		{
			GetDlgItem("Btn_Custom02")->Show(true);
			m_pCustomImage2->Show(false);
		}
		else
		{
			GetDlgItem("Btn_Custom02")->Show(false);
			m_pCustomImage2->Show(true);
			m_pCustomImage2->FixFrame(1);
			strHint = strHint + strCustom;
			m_pCustomImage2->SetHint(strHint);
		}
	}
	
	strHint.Format(GetStringFromTable(19935), 4, 8, 200000);
	if(nReputation < 200000)
	{
		GetDlgItem("Btn_Custom03")->Show(false);
		m_pCustomImage3->Show(true);
		m_pCustomImage3->SetHint(strHint);
		m_pCustomImage3->FixFrame(0);
	}
	else
	{
		if(!(iCustomizeRune & 0x04))
		{
			GetDlgItem("Btn_Custom03")->Show(true);
			m_pCustomImage3->Show(false);
		}
		else
		{
			GetDlgItem("Btn_Custom03")->Show(false);
			m_pCustomImage3->Show(true);
			m_pCustomImage3->FixFrame(1);
			strHint = strHint + strCustom;
			m_pCustomImage3->SetHint(strHint);
		}
	}

	strHint.Format(GetStringFromTable(19935), 5, 8, 400000);
	if(nReputation < 400000)
	{
		GetDlgItem("Btn_Custom04")->Show(false);
		m_pCustomImage4->Show(true);
		m_pCustomImage4->SetHint(strHint);
		m_pCustomImage4->FixFrame(0);
	}
	else
	{
		if(!(iCustomizeRune & 0x08))
		{
			GetDlgItem("Btn_Custom04")->Show(true);
			m_pCustomImage4->Show(false);
		}
		else
		{
			GetDlgItem("Btn_Custom04")->Show(false);
			m_pCustomImage4->Show(true);
			m_pCustomImage4->FixFrame(1);
			strHint = strHint + strCustom;
			m_pCustomImage4->SetHint(strHint);
		}
	}
}



AUI_BEGIN_COMMAND_MAP(CDlgRuneCustom, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)
AUI_ON_COMMAND("Btn_Lock",		OnCommandConfirm)
AUI_ON_COMMAND("Btn_Custom",	OnCommandProduce)

AUI_END_COMMAND_MAP()

CDlgRuneCustom::CDlgRuneCustom()
{

}

CDlgRuneCustom::~CDlgRuneCustom()
{

}

void CDlgRuneCustom::InitStrings()
{	
	m_pCombo_Property1->ResetContent();
	m_pCombo_Property2->ResetContent();
	m_pCombo_Property3->ResetContent();
	if(m_szName == "Win_Equip_Yuanying_Custom02")
	{
		m_pCombo_Property4->ResetContent();
		m_pCombo_ProPerty5->ResetContent();
	}

	CSplit s = GetStringFromTable(19930);
	CSPLIT_STRING_VECTOR vec = s.Split(_AL(","));	
	for(int i = 0; i < vec.size(); i++)
	{
		m_pCombo_Property1->AddString(vec[i]);
		m_pCombo_Property2->AddString(vec[i]);
		m_pCombo_Property3->AddString(vec[i]);
		if(m_szName == "Win_Equip_Yuanying_Custom02")
		{
			m_pCombo_Property4->AddString(vec[i]);
			m_pCombo_ProPerty5->AddString(vec[i]);
		}
	}
	m_pCombo_Property1->SetCurSel(0);
	m_pCombo_Property2->SetCurSel(1);
	m_pCombo_Property3->SetCurSel(2);
	if(m_szName == "Win_Equip_Yuanying_Custom02")
	{
		m_pCombo_Property4->SetCurSel(3);
		m_pCombo_ProPerty5->SetCurSel(4);
	}
		
}

bool CDlgRuneCustom::OnInitDialog()
{
	m_pCombo_Property1 = (PAUICOMBOBOX)GetDlgItem("Combo_01");
	m_pCombo_Property2 = (PAUICOMBOBOX)GetDlgItem("Combo_02");
	m_pCombo_Property3 = (PAUICOMBOBOX)GetDlgItem("Combo_03");;	
	
	if(m_szName == "Win_Equip_Yuanying_Custom02")
	{
		m_pCombo_Property4 = (PAUICOMBOBOX)GetDlgItem("Combo_04");
		m_pCombo_ProPerty5 = (PAUICOMBOBOX)GetDlgItem("Combo_05");
	}
	
	m_iType = 0;
	memset(m_iProperty, 0, sizeof(m_iProperty));

	return true;
}

void CDlgRuneCustom::OnShowDialog()
{
	InitStrings();

	m_pCombo_Property1->Enable(true);
	m_pCombo_Property2->Enable(true);
	m_pCombo_Property3->Enable(true);
	GetDlgItem("Btn_Lock")->Enable(true);
	GetDlgItem("Btn_Custom")->Enable(false);
	if(m_szName == "Win_Equip_Yuanying_Custom02")
	{
		m_pCombo_Property4->Enable(true);
		m_pCombo_ProPerty5->Enable(true);
	}

	if(m_szName == "Win_Equip_Yuanying_Custom02")
	{
		if(m_iType == CUSTOMIZE_TYPE2 || m_iType == CUSTOMIZE_TYPE3)
		{
			m_pCombo_ProPerty5->Show(false);
			GetDlgItem("Lab_05")->Show(false);
		}
		else if(m_iType == CUSTOMIZE_TYPE4)
		{
			m_pCombo_ProPerty5->Show(true);
			GetDlgItem("Lab_05")->Show(true);
		}
	}

	ACString strText;
	int iNumberCustom = 0, iNumberItems = 0;
	if(m_iType == CUSTOMIZE_TYPE1)
	{
		iNumberCustom = 3;
		iNumberItems = 5;
	}
	else if(m_iType == CUSTOMIZE_TYPE2)
	{
		iNumberCustom = 4;
		iNumberItems = 6;
	}	
	else if(m_iType == CUSTOMIZE_TYPE3)
	{
		iNumberCustom = 4;
		iNumberItems = 8;
	}
	else if(m_iType == CUSTOMIZE_TYPE4)
	{
		iNumberCustom = 5;
		iNumberItems = 8;
	}
	
		
	strText.Format(GetStringFromTable(19931), iNumberCustom, iNumberCustom, iNumberItems);
	((AUILabel*)GetDlgItem("Lab_Hint"))->SetText(strText);
}

void CDlgRuneCustom::OnCommandCancel(const char * szCommand)
{
	Show(false);
}

void CDlgRuneCustom::OnCommandConfirm(const char * szCommand)
{
	if(m_iType < CUSTOMIZE_TYPE1 || m_iType > CUSTOMIZE_TYPE4)
		return;

	int Property[34];
	int RuneItemNum[5] = {0, 5, 6, 8, 8};
	memset(Property, 0, sizeof(Property));
	memset(m_iProperty, 0, sizeof(m_iProperty));

	if(m_pCombo_Property1->GetCurSel() > 25)
		m_iProperty[0] = m_pCombo_Property1->GetCurSel() + 2;
	else 
		m_iProperty[0] = m_pCombo_Property1->GetCurSel() + 1;
	if(m_pCombo_Property2->GetCurSel() > 25)
		m_iProperty[1] = m_pCombo_Property2->GetCurSel() + 2;
	else
		m_iProperty[1] = m_pCombo_Property2->GetCurSel() + 1;
	if(m_pCombo_Property3->GetCurSel() > 25)
		m_iProperty[2] = m_pCombo_Property3->GetCurSel() + 2;
	else
		m_iProperty[2] = m_pCombo_Property3->GetCurSel() + 1;
	Property[m_iProperty[0]]++;
	Property[m_iProperty[1]]++;
	Property[m_iProperty[2]]++;

	if(m_szName == "Win_Equip_Yuanying_Custom02")
	{
		if(m_pCombo_Property4->GetCurSel() > 25)
			m_iProperty[3] = m_pCombo_Property4->GetCurSel() + 2;
		else
			m_iProperty[3] = m_pCombo_Property4->GetCurSel() + 1;
		Property[m_iProperty[3]]++;
		if(m_iType == CUSTOMIZE_TYPE4)
		{
			if(m_pCombo_ProPerty5->GetCurSel() > 25)
				m_iProperty[4] = m_pCombo_ProPerty5->GetCurSel() + 2;
			else
				m_iProperty[4] = m_pCombo_ProPerty5->GetCurSel() + 1;
			Property[m_iProperty[4]]++;
		}
		
	}
	
	int iNumber0 = 0, iNumber1 = 0, iNumber2 = 0;
	for(int i = 1; i < 34; i++)
	{
		if(Property[i] <= 0)
			continue;
		if(i < 28)
		{
			if(Property[i] > 2)
			{
				GetGameRun()->AddChatMessage(GetGameUIMan()->GetStringFromTable(19932), GP_CHAT_MISC);
				return;
			}
			else if(Property[i] == 2)
			{
				iNumber1++;
				iNumber2++;
			}
			else if(Property[i] == 1)
				iNumber0++;
		}
		else
		{
			if(Property[i] > 1)
			{
				GetGameRun()->AddChatMessage(GetGameUIMan()->GetStringFromTable(19932), GP_CHAT_MISC);
				return;
			}
			else if(Property[i] == 1)
			{
				iNumber1++;
			}
		}
	}

	if(iNumber2  > RuneItemNum[m_iType] - 4)
	{
		GetGameRun()->AddChatMessage(GetGameUIMan()->GetStringFromTable(19932), GP_CHAT_MISC);
		return;
	}
	else if(iNumber1 > 4)
	{
		GetGameRun()->AddChatMessage(GetGameUIMan()->GetStringFromTable(19932), GP_CHAT_MISC);
		return;
	}
	else if(iNumber0 + iNumber1 + iNumber2 > RuneItemNum[m_iType]) 
	{
		GetGameRun()->AddChatMessage(GetGameUIMan()->GetStringFromTable(19932), GP_CHAT_MISC);
		return;
	}

	m_pCombo_Property1->Enable(false);
	m_pCombo_Property2->Enable(false);
	m_pCombo_Property3->Enable(false);
	GetDlgItem("Btn_Lock")->Enable(false);
	GetDlgItem("Btn_Custom")->Enable(true);
	if(m_szName == "Win_Equip_Yuanying_Custom02")
	{
		m_pCombo_Property4->Enable(false);
		m_pCombo_ProPerty5->Enable(false);
	}
	
}

void CDlgRuneCustom::OnCommandProduce(const char * szCommand)
{
	if(GetHostPlayer()->GetPack()->GetEmptySlotNum() < 1)
	{
		GetGameRun()->AddChatMessage(GetGameUIMan()->GetStringFromTable(7076), GP_CHAT_MISC);
		return;
	}
	int iNumber = 0;
	if(m_iType == CUSTOMIZE_TYPE1)
		iNumber = 3;
	else if(m_iType == CUSTOMIZE_TYPE2 || m_iType == CUSTOMIZE_TYPE3)
		iNumber = 4;
	else if(m_iType == CUSTOMIZE_TYPE4)
		iNumber = 5;
	GetGameSession()->c2s_CmdCustomizeRune(m_iType, iNumber, m_iProperty);
	Show(false);
}

void CDlgRuneCustom::SetCustomType(int iType)
{
	m_iType = iType;	
}

