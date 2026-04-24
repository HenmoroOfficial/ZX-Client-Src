// File		: DlgPetDetail.cpp
// Creator	: Xiao Zhou
// Date		: 2008/4/3

#include "AFI.h"
#include "A2DSprite.h"
#include "DlgPetDetail.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIProgress.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUICTranslate.h"
#include "EC_IvtrConsume.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_FixedMsg.h"
#include "EC_Model.h"
#include "EC_NPC.h"
#include "EC_Skill.h"
#include "EC_Utility.h"
#include "EC_ModelRender.h"
#include "A3DSkinMan.h"
#include "DlgPetOperate.h"
#include "DlgPetCombine.h"
#include "LuaScript.h"
#include "luastate.h"

#define new A_DEBUG_NEW

#define  PET_LEVEL_NAME    "pet_level"
const char szPetLevelFile[] = "Interfaces\\script\\petlevel.lua";
const static int g_nMaxValue = 9999;


CDlgPetDetail::CDlgPetDetail()
{
	for(int i = 0; i < IVTRSIZE_PETPACK; i++)
		memset(&(m_PetPropadded[i]), 0, sizeof(S2C::player_pet_prop_added));
}

CDlgPetDetail::~CDlgPetDetail()
{
}

bool CDlgPetDetail::OnInitDialog()
{
	UINT i, j;
	m_pPrgLevel[0][0] = (PAUIPROGRESS)GetDlgItem("Prgs_Hp");
	m_pPrgLevel[1][0] = (PAUIPROGRESS)GetDlgItem("Prgs_Mp");
	m_pPrgLevel[2][0] = (PAUIPROGRESS)GetDlgItem("Prgs_MinAttack");
	m_pPrgLevel[3][0] = (PAUIPROGRESS)GetDlgItem("Prgs_MaxAttack");
	m_pPrgLevel[4][0] = (PAUIPROGRESS)GetDlgItem("Prgs_Defence");
	m_pPrgLevel[5][0] = (PAUIPROGRESS)GetDlgItem("Prgs_Definition");
	m_pPrgLevel[6][0] = (PAUIPROGRESS)GetDlgItem("Prgs_Evade");
	m_pPrgLevel[7][0] = (PAUIPROGRESS)GetDlgItem("Prgs_StunRes");
	m_pPrgLevel[8][0] = (PAUIPROGRESS)GetDlgItem("Prgs_WeaknessRes");
	m_pPrgLevel[9][0] = (PAUIPROGRESS)GetDlgItem("Prgs_EnlaceRes");
	m_pPrgLevel[10][0] = (PAUIPROGRESS)GetDlgItem("Prgs_SilenceRes");
	m_pPrgLevel[11][0] = (PAUIPROGRESS)GetDlgItem("Prgs_SleepRes");
	
	m_pPrgLevel[0][1] = (PAUIPROGRESS)GetDlgItem("Prgs_Extend_Hp");
	m_pPrgLevel[1][1] = (PAUIPROGRESS)GetDlgItem("Prgs_Extend_Mp");
	m_pPrgLevel[2][1] = (PAUIPROGRESS)GetDlgItem("Prgs_Extend_MinAttack");
	m_pPrgLevel[3][1] = (PAUIPROGRESS)GetDlgItem("Prgs_Extend_MaxAttack");
	m_pPrgLevel[4][1] = (PAUIPROGRESS)GetDlgItem("Prgs_Extend_Defence");
	m_pPrgLevel[5][1] = (PAUIPROGRESS)GetDlgItem("Prgs_Extend_Definition");
	m_pPrgLevel[6][1] = (PAUIPROGRESS)GetDlgItem("Prgs_Extend_Evade");
	m_pPrgLevel[7][1] = (PAUIPROGRESS)GetDlgItem("Prgs_Extend_StunRes");
	m_pPrgLevel[8][1] = (PAUIPROGRESS)GetDlgItem("Prgs_Extend_WeaknessRes");
	m_pPrgLevel[9][1] = (PAUIPROGRESS)GetDlgItem("Prgs_Extend_EnlaceRes");
	m_pPrgLevel[10][1] = (PAUIPROGRESS)GetDlgItem("Prgs_Extend_SilenceRes");
	m_pPrgLevel[11][1] = (PAUIPROGRESS)GetDlgItem("Prgs_Extend_SleepRes");
	for (i=0; i<2; i++)
	{
		for (j=0; j<12; j++)
		{
			m_pPrgLevel[j][i]->Show(false);
		}
	}
	m_nLength = m_pPrgLevel[0][0]->GetSize().cx;
	
	m_pImageStar = new A2DSprite;
	if( !m_pImageStar ) return AUI_ReportError(__LINE__, __FILE__);
	
	bool bval = m_pImageStar->Init(m_pA3DDevice, "pet\\星等.tga", AUI_COLORKEY);
	if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
	A3DRECT rc[7];
	int W = m_pImageStar->GetWidth();
	int H = m_pImageStar->GetHeight() / 4;
	for( i = 0; i < 7; i++ )
		rc[i].SetRect(0, H * i, W, H * (i + 1));
	m_pImageStar->ResetItems(4, rc);
	int index;
	GetGameUIMan()->AddImageListItem(m_pImageStar, index);

	ACString strHintStar;
	strHintStar = GetGameUIMan()->GetStringFromTable(9302);
	strHintStar += _AL("\r");
	EditBoxItemBase item(enumEIImage);
	item.SetImageIndex(index);
	item.SetImageFrame(0);
	strHintStar += (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
	strHintStar += GetGameUIMan()->GetStringFromTable(9303);
	strHintStar += _AL("\r");
	item.SetImageIndex(index);
	item.SetImageFrame(1);
	strHintStar += (ACHAR)(AUICOMMON_ITEM_CODE_START+1) + item.Serialize();
	strHintStar += GetGameUIMan()->GetStringFromTable(9304);
	strHintStar += _AL("\r");
	item.SetImageIndex(index);
	item.SetImageFrame(2);
	strHintStar += (ACHAR)(AUICOMMON_ITEM_CODE_START+2) + item.Serialize();
	strHintStar += GetGameUIMan()->GetStringFromTable(9305);
	strHintStar += _AL("\r");
	item.SetImageIndex(index);
	item.SetImageFrame(3);
	strHintStar += (ACHAR)(AUICOMMON_ITEM_CODE_START+3) + item.Serialize();
	strHintStar += GetGameUIMan()->GetStringFromTable(9306);
	AString strName;
	for (i=0; i<4; i++)
	{
		strName.Format("Img_Star%d", i+1);
		((PAUIIMAGEPICTURE)GetDlgItem(strName))->SetHint(strHintStar);
	}
	for(i = 13; i <= 16; i++ )
	{
		strName.Format("Img_Skill%02d", i);
		PAUIOBJECT pObj = GetDlgItem(strName);
		if(pObj)
			pObj->Show(false);
	}

	m_pState = g_LuaStateMan.GetAIState();
	if (!m_pState)
	{
		a_LogOutput(1, "CDlgPetDetail::InitDialog, null lua state\n");
		return false;
	}
	CLuaScript * pScript = m_pState->RegisterFile(szPetLevelFile);
	if (!pScript)
	{
		a_LogOutput(1, "CDlgPetDetail::InitDialog, cannt open Interfaces\\script\\petlevel.lua\n");
		return false;
	}

 	abase::vector<CScriptValue> args, results;
	m_pState->Call(PET_LEVEL_NAME, "GetPetLevelDef", args, results);

	CScriptValue& ret0 = results[0];
	abase::vector<CScriptValue>& ret0ik = ret0.m_ArrKey;
	abase::vector<CScriptValue>& ret0iv = ret0.m_ArrVal;
	for (i=0; i<ret0ik.size(); i++)
	{
		CScriptValue& ret0ijk = ret0ik[i];
		CScriptValue& ret0ijv = ret0iv[i];

		if (ret0ijk.m_Type == CScriptValue::SVT_STRING)
		{
			AString key; 
			ret0ijk.RetrieveAString(key);
			if (strcmp(key, "def") == 0)
			{
				for (j=0; j<ret0ijv.m_ArrVal.size(); j++)
				{
					m_nLevelValue[j] = ret0ijv.m_ArrVal[j].GetInt();
				}
			}
		}
		else if (ret0ijk.m_Type == CScriptValue::SVT_NUMBER)
		{
			int index = ret0ijk.GetInt();
			for (j=0; j<ret0ijv.m_ArrVal.size(); j++)
			{
				m_nLevelsIndex[index][j] = ret0ijv.m_ArrVal[j].GetInt();
			}
		}
	}

	for (i=0; i<DLGPETDETAIL_MAX_HOROSCOPE; i++)
	{
		for(j=0; j<DLGPETDETAIL_MAX_TYPE; j++)
		{
			m_nLevels[i][j] = m_nLevelValue[m_nLevelsIndex[i][j]];
		}
	}

	SetData(0xffffffff);

	return true;
}

bool CDlgPetDetail::Release()
{
	if (GetDataPtr())
	{
		CECPetBedge *pPet = (CECPetBedge*)GetDataPtr();
		delete pPet;
		SetDataPtr(NULL);
	}
	A3DRELEASE(m_pImageStar)
	return CDlgBase::Release();
}

void CDlgPetDetail::OnTick()
{
	CECPetBedge *pPet = NULL;
	if (GetData() != 0xffffffff)
	{
		pPet = (CECPetBedge *)GetHostPlayer()->GetPetPack()->GetItem(GetData());
	}
	else if (GetDataPtr())
	{
		pPet = (CECPetBedge*)GetDataPtr();
	}
	if( !pPet || !pPet->GetEssence().identify )
	{
		Show(false);
		return;
	}

	UpdateDisplay(pPet);
}

void CDlgPetDetail::UpdateDisplay(CECPetBedge *pPet)
{
	ACHAR szText[100];
	CECStringTab* pDescTab = GetGame()->GetItemDesc();
	AString strFile;
	af_GetFileTitle(GetGame()->GetDataPath(pPet->GetDBEssence()->file_head_icon[pPet->GetEssence().face]), strFile);
	strFile.MakeLower();
	((PAUIIMAGEPICTURE)GetDlgItem("Img_Head"))->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_PET],
		GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_PET][strFile]);
	GetDlgItem("Txt_Name")->SetText(pPet->GetName());
	a_sprintf(szText, _AL("%d"), pPet->GetEssence().level);
	GetDlgItem("Txt_Lv")->SetText(szText);
	if( CECPlayer::GetPetLevelUpExp(pPet->GetEssence().level + 1) > 0 )
		a_sprintf(szText, _AL("%.1f%%"), pPet->GetEssence().exp * 100.0 / CECPlayer::GetPetLevelUpExp(pPet->GetEssence().level + 1));
	else
		a_strcpy(szText, _AL("---"));
	GetDlgItem("Txt_Exp")->SetText(szText);

	int i = 1;
	int star = pPet->GetEssence().star;
	a_sprintf(szText, _AL("%d"), star);
	GetDlgItem("Txt_StarLv")->SetText(szText);
	if (pPet->GetEssence().reborn_star>0)
	{
		a_sprintf(szText, _AL("+%d"), pPet->GetEssence().reborn_star);
	}
	else
		szText[0] = 0;
	GetDlgItem("Txt_StarLv2")->SetText(szText);

	char szName[20];
	while( star >= 15 && i <= 4 )
	{
		sprintf(szName, "Img_Star%d", i);
		((PAUIIMAGEPICTURE)GetDlgItem(szName))->FixFrame(3);
		((PAUIIMAGEPICTURE)GetDlgItem(szName))->Show(true);
		i++;
		star -= 15;
	}
	while( star >= 9 && i <= 4 )
	{
		sprintf(szName, "Img_Star%d", i);
		((PAUIIMAGEPICTURE)GetDlgItem(szName))->FixFrame(2);
		((PAUIIMAGEPICTURE)GetDlgItem(szName))->Show(true);
		i++;
		star -= 9;
	}
	while( star >= 4 && i <= 4 )
	{
		sprintf(szName, "Img_Star%d", i);
		((PAUIIMAGEPICTURE)GetDlgItem(szName))->FixFrame(1);
		((PAUIIMAGEPICTURE)GetDlgItem(szName))->Show(true);
		i++;
		star -= 4;
	}
	while( star >= 1 && i <= 4 )
	{
		sprintf(szName, "Img_Star%d", i);
		((PAUIIMAGEPICTURE)GetDlgItem(szName))->FixFrame(0);
		((PAUIIMAGEPICTURE)GetDlgItem(szName))->Show(true);
		i++;
		star -= 1;
	}
	while( i <= 4 )
	{
		sprintf(szName, "Img_Star%d", i);
		((PAUIIMAGEPICTURE)GetDlgItem(szName))->Show(false);
		i++;
	}

	if ( pPet->GetEssence().reborn_cnt>0 )
		a_sprintf(szText, _AL("%s"), GetGameUIMan()->GetStringFromTable(3627));
	else
		a_sprintf(szText, _AL("%d/%d"), pPet->GetEssence().age, pPet->GetEssence().life);
	GetDlgItem("Txt_Life")->SetText(szText);

	if ( pPet->GetEssence().reborn_cnt>0 )
		a_sprintf(szText, _AL("%s"), GetGameUIMan()->GetStringFromTable(3628));
	else
		a_sprintf(szText, _AL("%d/%d"), pPet->GetEssence().cur_hunger_point, pPet->GetEssence().max_hunger_point);
	GetDlgItem("Txt_Full")->SetText(szText);
	if ( pPet->GetEssence().reborn_cnt>0 )
		a_sprintf(szText, _AL("%s"), GetGameUIMan()->GetStringFromTable(3629));
	else
		a_sprintf(szText, _AL("%d/%d"), pPet->GetEssence().cur_honor_point, pPet->GetEssence().max_honor_point);
	GetDlgItem("Txt_Love")->SetText(szText);
	GetDlgItem("Txt_Horoscope")->SetText(pDescTab->GetWideString(ITEMDESC_PET_HOROSCOPE0 + pPet->GetEssence().horoscope));
	int nHoroType = pPet->GetEssence().horoscope;
	for (i=0; i<DLGPETDETAIL_MAX_TYPE-1; i++)
	{
		int cx1 = m_nLength * m_nLevels[nHoroType][i] / g_nMaxValue;
		int cy1 = m_pPrgLevel[i][0]->GetSize().cy;
		m_pPrgLevel[i][0]->SetSize(cx1,cy1);

		int cx2 = m_nLength * (m_nLevelValue[m_nLevelsIndex[nHoroType][i]+5] - m_nLevels[nHoroType][i] )/ g_nMaxValue;
		int cy2 = m_pPrgLevel[i][1]->GetSize().cy;
		int x2 = m_pPrgLevel[i][0]->GetPos(true).x + cx1;
		int y2 = m_pPrgLevel[i][1]->GetPos(true).y;
		m_pPrgLevel[i][1]->SetPos(x2, y2);
		m_pPrgLevel[i][1]->SetSize(cx2,cy2);
	}
	GetDlgItem("Txt_Element")->SetText(pDescTab->GetWideString(ITEMDESC_PET_ELEMENT0 + pPet->GetEssence().element));
	GetDlgItem("Txt_Race")->SetText(pDescTab->GetWideString(ITEMDESC_PET_RACE0 + pPet->GetEssence().race));
	a_sprintf(szText, _AL("%d/%d"), pPet->GetEssence().cur_hp, pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_HP].cur_value);
	GetDlgItem("Txt_Hp")->SetText(szText);
	a_sprintf(szText, _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_HP].init);
	GetDlgItem("Txt_IniHp")->SetText(szText);
	int nPotential = pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_HP].potential;
	a_sprintf(szText, _AL("%d"), nPotential);
	GetDlgItem("Txt_TalentHp")->SetText(szText);
	m_pPrgLevel[0][0]->SetProgress(int(double(nPotential) / m_nLevels[nHoroType][0] * AUIPROGRESS_MAX));
	m_pPrgLevel[0][1]->SetProgress(int(double(nPotential-m_nLevels[nHoroType][0]) / (m_nLevelValue[m_nLevelsIndex[nHoroType][0]+5]-m_nLevels[nHoroType][0]) * AUIPROGRESS_MAX));
	if( pPet->GetEssence().main_type == itemdataman::_pet_bedge_essence::MAX_HP )
	{
		GetDlgItem("Txt_Hp")->SetColor(A3DCOLORRGB(255, 0, 0));
		GetDlgItem("Txt_IniHp")->SetColor(A3DCOLORRGB(255, 0, 0));
		GetDlgItem("Txt_TalentHp")->SetColor(A3DCOLORRGB(255, 0, 0));
	}
	else
	{
		GetDlgItem("Txt_Hp")->SetColor(A3DCOLORRGB(255, 255, 255));
		GetDlgItem("Txt_IniHp")->SetColor(A3DCOLORRGB(255, 255, 255));
		GetDlgItem("Txt_TalentHp")->SetColor(A3DCOLORRGB(255, 255, 255));
	}

	a_sprintf(szText, _AL("%d/%d"), pPet->GetEssence().cur_vigor, pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_VIGOR].cur_value);
	GetDlgItem("Txt_Mp")->SetText(szText);
	a_sprintf(szText, _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_VIGOR].init);
	GetDlgItem("Txt_IniMp")->SetText(szText);
	nPotential = pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_VIGOR].potential;
	a_sprintf(szText, _AL("%d"), nPotential);
	GetDlgItem("Txt_TalentMp")->SetText(szText);
	m_pPrgLevel[1][0]->SetProgress(int(double(nPotential) / m_nLevels[nHoroType][1] * AUIPROGRESS_MAX));
	m_pPrgLevel[1][1]->SetProgress(int(double(nPotential-m_nLevels[nHoroType][1]) / (m_nLevelValue[m_nLevelsIndex[nHoroType][1]+5]-m_nLevels[nHoroType][1]) * AUIPROGRESS_MAX));
	if( pPet->GetEssence().main_type == itemdataman::_pet_bedge_essence::MAX_VIGOR )
	{
		GetDlgItem("Txt_Mp")->SetColor(A3DCOLORRGB(255, 0, 0));
		GetDlgItem("Txt_IniMp")->SetColor(A3DCOLORRGB(255, 0, 0));
		GetDlgItem("Txt_TalentMp")->SetColor(A3DCOLORRGB(255, 0, 0));
	}
	else
	{
		GetDlgItem("Txt_Mp")->SetColor(A3DCOLORRGB(255, 255, 255));
		GetDlgItem("Txt_IniMp")->SetColor(A3DCOLORRGB(255, 255, 255));
		GetDlgItem("Txt_TalentMp")->SetColor(A3DCOLORRGB(255, 255, 255));
	}

	a_sprintf(szText, _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MIN_ATTACK].cur_value);
	GetDlgItem("Txt_MinAttack")->SetText(szText);
	a_sprintf(szText, _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MIN_ATTACK].init);
	GetDlgItem("Txt_IniMinAttack")->SetText(szText);
	nPotential = pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MIN_ATTACK].potential;
	a_sprintf(szText, _AL("%d"), nPotential);
	GetDlgItem("Txt_TalentMinAttack")->SetText(szText);
	m_pPrgLevel[2][0]->SetProgress(int(double(nPotential) / m_nLevels[nHoroType][2] * AUIPROGRESS_MAX));
	m_pPrgLevel[2][1]->SetProgress(int(double(nPotential-m_nLevels[nHoroType][2]) / (m_nLevelValue[m_nLevelsIndex[nHoroType][2]+5]-m_nLevels[nHoroType][2]) * AUIPROGRESS_MAX));
	if( pPet->GetEssence().main_type == itemdataman::_pet_bedge_essence::MIN_ATTACK )
	{
		GetDlgItem("Txt_MinAttack")->SetColor(A3DCOLORRGB(255, 0, 0));
		GetDlgItem("Txt_IniMinAttack")->SetColor(A3DCOLORRGB(255, 0, 0));
		GetDlgItem("Txt_TalentMinAttack")->SetColor(A3DCOLORRGB(255, 0, 0));
	}
	else
	{
		GetDlgItem("Txt_MinAttack")->SetColor(A3DCOLORRGB(255, 255, 255));
		GetDlgItem("Txt_IniMinAttack")->SetColor(A3DCOLORRGB(255, 255, 255));
		GetDlgItem("Txt_TalentMinAttack")->SetColor(A3DCOLORRGB(255, 255, 255));
	}

	a_sprintf(szText, _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_ATTACK].cur_value);
	GetDlgItem("Txt_MaxAttack")->SetText(szText);
	a_sprintf(szText, _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_ATTACK].init);
	GetDlgItem("Txt_IniMaxAttack")->SetText(szText);
	nPotential = pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_ATTACK].potential;
	a_sprintf(szText, _AL("%d"), nPotential);
	GetDlgItem("Txt_TalentMaxAttack")->SetText(szText);
	m_pPrgLevel[3][0]->SetProgress(int(double(nPotential) / m_nLevels[nHoroType][3] * AUIPROGRESS_MAX));
	m_pPrgLevel[3][1]->SetProgress(int(double(nPotential-m_nLevels[nHoroType][1]) / (m_nLevelValue[m_nLevelsIndex[nHoroType][3]+5]-m_nLevels[nHoroType][3]) * AUIPROGRESS_MAX));
	if( pPet->GetEssence().main_type == itemdataman::_pet_bedge_essence::MAX_ATTACK )
	{
		GetDlgItem("Txt_MaxAttack")->SetColor(A3DCOLORRGB(255, 0, 0));
		GetDlgItem("Txt_IniMaxAttack")->SetColor(A3DCOLORRGB(255, 0, 0));
		GetDlgItem("Txt_TalentMaxAttack")->SetColor(A3DCOLORRGB(255, 0, 0));
	}
	else
	{
		GetDlgItem("Txt_MaxAttack")->SetColor(A3DCOLORRGB(255, 255, 255));
		GetDlgItem("Txt_IniMaxAttack")->SetColor(A3DCOLORRGB(255, 255, 255));
		GetDlgItem("Txt_TalentMaxAttack")->SetColor(A3DCOLORRGB(255, 255, 255));
	}

	a_sprintf(szText, _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::DEFENCE].cur_value);
	GetDlgItem("Txt_Defence")->SetText(szText);
	a_sprintf(szText, _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::DEFENCE].init);
	GetDlgItem("Txt_IniDefence")->SetText(szText);
	nPotential = pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::DEFENCE].potential;
	a_sprintf(szText, _AL("%d"), nPotential);
	GetDlgItem("Txt_TalentDefence")->SetText(szText);
	m_pPrgLevel[4][0]->SetProgress(int(double(nPotential) / m_nLevels[nHoroType][4] * AUIPROGRESS_MAX));
	m_pPrgLevel[4][1]->SetProgress(int(double(nPotential-m_nLevels[nHoroType][1]) / (m_nLevelValue[m_nLevelsIndex[nHoroType][4]+5]-m_nLevels[nHoroType][4]) * AUIPROGRESS_MAX));
	if( pPet->GetEssence().main_type == itemdataman::_pet_bedge_essence::DEFENCE )
	{
		GetDlgItem("Txt_Defence")->SetColor(A3DCOLORRGB(255, 0, 0));
		GetDlgItem("Txt_IniDefence")->SetColor(A3DCOLORRGB(255, 0, 0));
		GetDlgItem("Txt_TalentDefence")->SetColor(A3DCOLORRGB(255, 0, 0));
	}
	else
	{
		GetDlgItem("Txt_Defence")->SetColor(A3DCOLORRGB(255, 255, 255));
		GetDlgItem("Txt_IniDefence")->SetColor(A3DCOLORRGB(255, 255, 255));
		GetDlgItem("Txt_TalentDefence")->SetColor(A3DCOLORRGB(255, 255, 255));
	}

	a_sprintf(szText, _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::HIT].cur_value);
	GetDlgItem("Txt_Definition")->SetText(szText);
	a_sprintf(szText, _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::HIT].init);
	GetDlgItem("Txt_IniDefinition")->SetText(szText);
	nPotential = pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::HIT].potential;
	a_sprintf(szText, _AL("%d"), nPotential);
	GetDlgItem("Txt_TalentDefinition")->SetText(szText);
	m_pPrgLevel[5][0]->SetProgress(int(double(nPotential) / m_nLevels[nHoroType][5] * AUIPROGRESS_MAX));
	m_pPrgLevel[5][1]->SetProgress(int(double(nPotential-m_nLevels[nHoroType][1]) / (m_nLevelValue[m_nLevelsIndex[nHoroType][5]+5]-m_nLevels[nHoroType][5]) * AUIPROGRESS_MAX));
	if( pPet->GetEssence().main_type == itemdataman::_pet_bedge_essence::HIT )
	{
		GetDlgItem("Txt_Definition")->SetColor(A3DCOLORRGB(255, 0, 0));
		GetDlgItem("Txt_IniDefinition")->SetColor(A3DCOLORRGB(255, 0, 0));
		GetDlgItem("Txt_TalentDefinition")->SetColor(A3DCOLORRGB(255, 0, 0));
	}
	else
	{
		GetDlgItem("Txt_Definition")->SetColor(A3DCOLORRGB(255, 255, 255));
		GetDlgItem("Txt_IniDefinition")->SetColor(A3DCOLORRGB(255, 255, 255));
		GetDlgItem("Txt_TalentDefinition")->SetColor(A3DCOLORRGB(255, 255, 255));
	}

	a_sprintf(szText, _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::JOUK].cur_value);
	GetDlgItem("Txt_Evade")->SetText(szText);
	a_sprintf(szText, _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::JOUK].init);
	GetDlgItem("Txt_IniEvade")->SetText(szText);
	nPotential = pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::JOUK].potential;
	a_sprintf(szText, _AL("%d"), nPotential);
	GetDlgItem("Txt_TalentEvade")->SetText(szText);
	m_pPrgLevel[6][0]->SetProgress(int(double(nPotential) / m_nLevels[nHoroType][6] * AUIPROGRESS_MAX));
	m_pPrgLevel[6][1]->SetProgress(int(double(nPotential-m_nLevels[nHoroType][1]) / (m_nLevelValue[m_nLevelsIndex[nHoroType][6]+5]-m_nLevels[nHoroType][6]) * AUIPROGRESS_MAX));
	if( pPet->GetEssence().main_type == itemdataman::_pet_bedge_essence::JOUK )
	{
		GetDlgItem("Txt_Evade")->SetColor(A3DCOLORRGB(255, 0, 0));
		GetDlgItem("Txt_IniEvade")->SetColor(A3DCOLORRGB(255, 0, 0));
		GetDlgItem("Txt_TalentEvade")->SetColor(A3DCOLORRGB(255, 0, 0));
	}
	else
	{
		GetDlgItem("Txt_Evade")->SetColor(A3DCOLORRGB(255, 255, 255));
		GetDlgItem("Txt_IniEvade")->SetColor(A3DCOLORRGB(255, 255, 255));
		GetDlgItem("Txt_TalentEvade")->SetColor(A3DCOLORRGB(255, 255, 255));
	}

	a_sprintf(szText, _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE1].cur_value);
	GetDlgItem("Txt_StunRes")->SetText(szText);
	a_sprintf(szText, _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE1].init);
	GetDlgItem("Txt_IniStunRes")->SetText(szText);
	nPotential = pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE1].potential;
	a_sprintf(szText, _AL("%d"), nPotential);
	GetDlgItem("Txt_TalentStunRes")->SetText(szText);
	m_pPrgLevel[7][0]->SetProgress(int(double(nPotential) / m_nLevels[nHoroType][7] * AUIPROGRESS_MAX));
	m_pPrgLevel[7][1]->SetProgress(int(double(nPotential-m_nLevels[nHoroType][1]) / (m_nLevelValue[m_nLevelsIndex[nHoroType][7]+5]-m_nLevels[nHoroType][7]) * AUIPROGRESS_MAX));
	if( pPet->GetEssence().main_type == itemdataman::_pet_bedge_essence::RESISTANCE1 )
	{
		GetDlgItem("Txt_StunRes")->SetColor(A3DCOLORRGB(255, 0, 0));
		GetDlgItem("Txt_IniStunRes")->SetColor(A3DCOLORRGB(255, 0, 0));
		GetDlgItem("Txt_TalentStunRes")->SetColor(A3DCOLORRGB(255, 0, 0));
	}
	else
	{
		GetDlgItem("Txt_StunRes")->SetColor(A3DCOLORRGB(255, 255, 255));
		GetDlgItem("Txt_IniStunRes")->SetColor(A3DCOLORRGB(255, 255, 255));
		GetDlgItem("Txt_TalentStunRes")->SetColor(A3DCOLORRGB(255, 255, 255));
	}

	a_sprintf(szText, _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE2].cur_value);
	GetDlgItem("Txt_WeaknessRes")->SetText(szText);
	a_sprintf(szText, _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE2].init);
	GetDlgItem("Txt_IniWeaknessRes")->SetText(szText);
	nPotential = pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE2].potential;
	a_sprintf(szText, _AL("%d"), nPotential);
	GetDlgItem("Txt_TalentWeaknessRes")->SetText(szText);
	m_pPrgLevel[8][0]->SetProgress(int(double(nPotential) / m_nLevels[nHoroType][8] * AUIPROGRESS_MAX));
	m_pPrgLevel[8][1]->SetProgress(int(double(nPotential-m_nLevels[nHoroType][1]) / (m_nLevelValue[m_nLevelsIndex[nHoroType][8]+5]-m_nLevels[nHoroType][8]) * AUIPROGRESS_MAX));
	if( pPet->GetEssence().main_type == itemdataman::_pet_bedge_essence::RESISTANCE2 )
	{
		GetDlgItem("Txt_WeaknessRes")->SetColor(A3DCOLORRGB(255, 0, 0));
		GetDlgItem("Txt_IniWeaknessRes")->SetColor(A3DCOLORRGB(255, 0, 0));
		GetDlgItem("Txt_TalentWeaknessRes")->SetColor(A3DCOLORRGB(255, 0, 0));
	}
	else
	{
		GetDlgItem("Txt_WeaknessRes")->SetColor(A3DCOLORRGB(255, 255, 255));
		GetDlgItem("Txt_IniWeaknessRes")->SetColor(A3DCOLORRGB(255, 255, 255));
		GetDlgItem("Txt_TalentWeaknessRes")->SetColor(A3DCOLORRGB(255, 255, 255));
	}

	a_sprintf(szText, _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE3].cur_value);
	GetDlgItem("Txt_EnlaceRes")->SetText(szText);
	a_sprintf(szText, _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE3].init);
	GetDlgItem("Txt_IniEnlaceRes")->SetText(szText);
	nPotential = pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE3].potential;
	a_sprintf(szText, _AL("%d"), nPotential);
	GetDlgItem("Txt_TalentEnlaceRes")->SetText(szText);
	m_pPrgLevel[9][0]->SetProgress(int(double(nPotential) / m_nLevels[nHoroType][9] * AUIPROGRESS_MAX));
	m_pPrgLevel[9][1]->SetProgress(int(double(nPotential-m_nLevels[nHoroType][1]) / (m_nLevelValue[m_nLevelsIndex[nHoroType][9]+5]-m_nLevels[nHoroType][9]) * AUIPROGRESS_MAX));
	if( pPet->GetEssence().main_type == itemdataman::_pet_bedge_essence::RESISTANCE3 )
	{
		GetDlgItem("Txt_EnlaceRes")->SetColor(A3DCOLORRGB(255, 0, 0));
		GetDlgItem("Txt_IniEnlaceRes")->SetColor(A3DCOLORRGB(255, 0, 0));
		GetDlgItem("Txt_TalentEnlaceRes")->SetColor(A3DCOLORRGB(255, 0, 0));
	}
	else
	{
		GetDlgItem("Txt_EnlaceRes")->SetColor(A3DCOLORRGB(255, 255, 255));
		GetDlgItem("Txt_IniEnlaceRes")->SetColor(A3DCOLORRGB(255, 255, 255));
		GetDlgItem("Txt_TalentEnlaceRes")->SetColor(A3DCOLORRGB(255, 255, 255));
	}
	if (m_szName=="Win_PetDetail")
	{
		if( GetData() != 0xffffffff && m_PetPropadded[GetData()].maxhp != 0 )
		{
			ACString strText;
			strText.Format(_AL("^00ff00%+d"), m_PetPropadded[GetData()].maxhp);
			GetDlgItem("Txt_Hp")->SetText(GetDlgItem("Txt_Hp")->GetText() + strText);
		}
		if( GetData() != 0xffffffff && m_PetPropadded[GetData()].maxmp != 0 )
		{
			ACString strText;
			strText.Format(_AL("^00ff00%+d"), m_PetPropadded[GetData()].maxmp);
			GetDlgItem("Txt_Mp")->SetText(GetDlgItem("Txt_Mp")->GetText() + strText);
		}
		if( GetData() != 0xffffffff && m_PetPropadded[GetData()].attack != 0 )
		{
			ACString strText;
			strText.Format(_AL("^00ff00%+d"), m_PetPropadded[GetData()].attack);
			GetDlgItem("Txt_MinAttack")->SetText(GetDlgItem("Txt_MinAttack")->GetText() + strText);
		}
		if( GetData() != 0xffffffff && m_PetPropadded[GetData()].attack != 0 )
		{
			ACString strText;
			strText.Format(_AL("^00ff00%+d"), m_PetPropadded[GetData()].attack);
			GetDlgItem("Txt_MaxAttack")->SetText(GetDlgItem("Txt_MaxAttack")->GetText() + strText);
		}
		if( GetData() != 0xffffffff && m_PetPropadded[GetData()].defence != 0 )
		{
			ACString strText;
			strText.Format(_AL("^00ff00%+d"), m_PetPropadded[GetData()].defence);
			GetDlgItem("Txt_Defence")->SetText(GetDlgItem("Txt_Defence")->GetText() + strText);
		}
		if( GetData() != 0xffffffff && m_PetPropadded[GetData()].hit != 0 )
		{
			ACString strText;
			strText.Format(_AL("^00ff00%+d"), m_PetPropadded[GetData()].hit);
			GetDlgItem("Txt_Definition")->SetText(GetDlgItem("Txt_Definition")->GetText() + strText);
		}
		if( GetData() != 0xffffffff && m_PetPropadded[GetData()].jouk != 0 )
		{
			ACString strText;
			strText.Format(_AL("^00ff00%+d"), m_PetPropadded[GetData()].jouk);
			GetDlgItem("Txt_Evade")->SetText(GetDlgItem("Txt_Evade")->GetText() + strText);
		}
		if( GetData() != 0xffffffff && m_PetPropadded[GetData()].resistance[0] != 0 )
		{
			ACString strText;
			strText.Format(_AL("^00ff00%+d"), m_PetPropadded[GetData()].resistance[0]);
			GetDlgItem("Txt_StunRes")->SetText(GetDlgItem("Txt_StunRes")->GetText() + strText);
		}
		if( GetData() != 0xffffffff && m_PetPropadded[GetData()].resistance[1] != 0 )
		{
			ACString strText;
			strText.Format(_AL("^00ff00%+d"), m_PetPropadded[GetData()].resistance[1]);
			GetDlgItem("Txt_WeaknessRes")->SetText(GetDlgItem("Txt_WeaknessRes")->GetText() + strText);
		}
		if( GetData() != 0xffffffff && m_PetPropadded[GetData()].resistance[2] != 0 )
		{
			ACString strText;
			strText.Format(_AL("^00ff00%+d"), m_PetPropadded[GetData()].resistance[2]);
			GetDlgItem("Txt_EnlaceRes")->SetText(GetDlgItem("Txt_EnlaceRes")->GetText() + strText);
		}
		if( GetData() != 0xffffffff && m_PetPropadded[GetData()].resistance[3] != 0 )
		{
			ACString strText;
			strText.Format(_AL("^00ff00%+d"), m_PetPropadded[GetData()].resistance[3]);
			GetDlgItem("Txt_SilenceRes")->SetText(GetDlgItem("Txt_SilenceRes")->GetText() + strText);
		}
		if( GetData() != 0xffffffff && m_PetPropadded[GetData()].resistance[4] != 0 )
		{
			ACString strText;
			strText.Format(_AL("^00ff00%+d"), m_PetPropadded[GetData()].resistance[4]);
			GetDlgItem("Txt_SleepRes")->SetText(GetDlgItem("Txt_SleepRes")->GetText() + strText);
		}
		if( GetData() != 0xffffffff && fabs(m_PetPropadded[GetData()].crit_rate) > 0.00001f  )
		{
			ACString strText;
			strText.Format(_AL("^00ff00%+.1f%%"), m_PetPropadded[GetData()].crit_rate * 100);
			GetDlgItem("Txt_CriticalRate")->SetText(GetDlgItem("Txt_CriticalRate")->GetText() + strText);
		}
		if( GetData() != 0xffffffff && fabs(m_PetPropadded[GetData()].crit_damage) > 0.00001f  )
		{
			ACString strText;
			strText.Format(_AL("^00ff00^00ff00%+d%%"), int(m_PetPropadded[GetData()].crit_damage * 100 + 0.001));
			GetDlgItem("Txt_CriticalDamage")->SetText(GetDlgItem("Txt_CriticalDamage")->GetText() + strText);
		}

		GetDlgItem("Lab_SkillDefinition")->Show(pPet->GetEssence().reborn_cnt>0);
		GetDlgItem("Txt_SkillDefinition")->Show(pPet->GetEssence().reborn_cnt>0);
		GetDlgItem("Lab_SkillEvade")->Show(pPet->GetEssence().reborn_cnt>0);
		GetDlgItem("Txt_SkillEvade")->Show(pPet->GetEssence().reborn_cnt>0);
		GetDlgItem("Lab_DeCriticalRate")->Show(pPet->GetEssence().reborn_cnt>0);
		GetDlgItem("Txt_DeCriticalRate")->Show(pPet->GetEssence().reborn_cnt>0);
		GetDlgItem("Lab_DeCriticalDamage")->Show(pPet->GetEssence().reborn_cnt>0);
		GetDlgItem("Txt_DeCriticalDamage")->Show(pPet->GetEssence().reborn_cnt>0);
		GetDlgItem("Lab_DeDamage")->Show(pPet->GetEssence().reborn_cnt>0);
		GetDlgItem("Txt_DeDamage")->Show(pPet->GetEssence().reborn_cnt>0);
		GetDlgItem("Lab_DeDamageRate")->Show(pPet->GetEssence().reborn_cnt>0);
		GetDlgItem("Txt_DeDamageRate")->Show(pPet->GetEssence().reborn_cnt>0);
	}

	if ( pPet->GetEssence().reborn_cnt>0 )
	{
		a_sprintf(szText, _AL("%.1f"), pPet->GetEssence().skill_attack_rate * 100);
		GetDlgItem("Txt_SkillDefinition")->SetText(szText);
		a_sprintf(szText, _AL("%.1f"), pPet->GetEssence().skill_armor_rate* 100);
		GetDlgItem("Txt_SkillEvade")->SetText(szText);
		a_sprintf(szText, _AL("%.1f%%"), pPet->GetEssence().anti_crit_rate* 100);
		GetDlgItem("Txt_DeCriticalRate")->SetText(szText);
		a_sprintf(szText, _AL("%.1f%%"), pPet->GetEssence().anti_crit_damage* 100);
		GetDlgItem("Txt_DeCriticalDamage")->SetText(szText);
		a_sprintf(szText, _AL("%d"), pPet->GetEssence().damage_reduce);
		GetDlgItem("Txt_DeDamage")->SetText(szText);
		a_sprintf(szText, _AL("%.1f%%"), pPet->GetEssence().damage_reduce_percent* 100);
		GetDlgItem("Txt_DeDamageRate")->SetText(szText);
	}
	
	a_sprintf(szText, _AL("%.1f%%"), pPet->GetEssence().crit_rate * 100);
	GetDlgItem("Txt_CriticalRate")->SetText(szText);
	
	a_sprintf(szText, _AL("%d%%"), int(pPet->GetEssence().crit_damage * 100 + 0.001));
	GetDlgItem("Txt_CriticalDamage")->SetText(szText);

	a_sprintf(szText, _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE4].cur_value);
	GetDlgItem("Txt_SilenceRes")->SetText(szText);
	a_sprintf(szText, _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE4].init);
	GetDlgItem("Txt_IniSilenceRes")->SetText(szText);
	nPotential = pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE4].potential;
	a_sprintf(szText, _AL("%d"), nPotential);
	GetDlgItem("Txt_TalentSilenceRes")->SetText(szText);
	m_pPrgLevel[10][0]->SetProgress(int(double(nPotential) / m_nLevels[nHoroType][10] * AUIPROGRESS_MAX));
	m_pPrgLevel[10][1]->SetProgress(int(double(nPotential-m_nLevels[nHoroType][1]) / (m_nLevelValue[m_nLevelsIndex[nHoroType][10]+5]-m_nLevels[nHoroType][10]) * AUIPROGRESS_MAX));
	if( pPet->GetEssence().main_type == itemdataman::_pet_bedge_essence::RESISTANCE4 )
	{
		GetDlgItem("Txt_SilenceRes")->SetColor(A3DCOLORRGB(255, 0, 0));
		GetDlgItem("Txt_IniSilenceRes")->SetColor(A3DCOLORRGB(255, 0, 0));
		GetDlgItem("Txt_TalentSilenceRes")->SetColor(A3DCOLORRGB(255, 0, 0));
	}
	else
	{
		GetDlgItem("Txt_SilenceRes")->SetColor(A3DCOLORRGB(255, 255, 255));
		GetDlgItem("Txt_IniSilenceRes")->SetColor(A3DCOLORRGB(255, 255, 255));
		GetDlgItem("Txt_TalentSilenceRes")->SetColor(A3DCOLORRGB(255, 255, 255));
	}

	a_sprintf(szText, _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE5].cur_value);
	GetDlgItem("Txt_SleepRes")->SetText(szText);
	a_sprintf(szText, _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE5].init);
	GetDlgItem("Txt_IniSleepRes")->SetText(szText);
	nPotential = pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE5].potential;
	a_sprintf(szText, _AL("%d"), nPotential);
	GetDlgItem("Txt_TalentSleepRes")->SetText(szText);
	m_pPrgLevel[11][0]->SetProgress(int(double(nPotential) / m_nLevels[nHoroType][11] * AUIPROGRESS_MAX));
	m_pPrgLevel[11][1]->SetProgress(int(double(nPotential-m_nLevels[nHoroType][1]) / (m_nLevelValue[m_nLevelsIndex[nHoroType][11]+5]-m_nLevels[nHoroType][11]) * AUIPROGRESS_MAX));
	if( pPet->GetEssence().main_type == itemdataman::_pet_bedge_essence::RESISTANCE5 )
	{
		GetDlgItem("Txt_SleepRes")->SetColor(A3DCOLORRGB(255, 0, 0));
		GetDlgItem("Txt_IniSleepRes")->SetColor(A3DCOLORRGB(255, 0, 0));
		GetDlgItem("Txt_TalentSleepRes")->SetColor(A3DCOLORRGB(255, 0, 0));
	}
	else
	{
		GetDlgItem("Txt_SleepRes")->SetColor(A3DCOLORRGB(255, 255, 255));
		GetDlgItem("Txt_IniSleepRes")->SetColor(A3DCOLORRGB(255, 255, 255));
		GetDlgItem("Txt_TalentSleepRes")->SetColor(A3DCOLORRGB(255, 255, 255));
	}

	//生活能力
	a_sprintf(szText, _AL("%d"), pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::ACUITY]);
	GetDlgItem("Txt_Live0")->SetText(szText);
	a_sprintf(szText, _AL("%d"), pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::STAMINA]);
	GetDlgItem("Txt_Live1")->SetText(szText);
	a_sprintf(szText, _AL("%d"), pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::SAWY]);
	GetDlgItem("Txt_Live2")->SetText(szText);
	a_sprintf(szText, _AL("%d"), pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::DOWRY]);
	GetDlgItem("Txt_Live3")->SetText(szText);
	a_sprintf(szText, _AL("%d"), pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::WISDOM]);
	GetDlgItem("Txt_Live4")->SetText(szText);
	a_sprintf(szText, _AL("%d"), pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::CNR]);
	GetDlgItem("Txt_Live5")->SetText(szText);
	for (i=0; i<8; ++i)
	{
		AString strName;
		strName.Format("Txt_Skill%02d", i+1);
		PAUILABEL pLabSkillName = (PAUILABEL)GetDlgItem(strName);
		if(pLabSkillName)
			pLabSkillName->SetText(_AL(""));
	}
	// 星象固定1356
	//static int horoscopeSkill[] = {}
	CECSkill *pSkill = new CECSkill(1359+pPet->GetEssence().horoscope, 10);
	if (pSkill)
	{
		const char* szIcon = GNET::ElementSkill::GetIcon(pSkill->GetSkillID());
		AString strFile;
		af_GetFileTitle(szIcon, strFile);
		strFile.MakeLower();
		((PAUIIMAGEPICTURE)GetDlgItem("Img_Horoscope"))->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_SKILL],
			GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_SKILL][strFile]);
				((PAUIIMAGEPICTURE)GetDlgItem("Img_Horoscope"))->SetHint(pSkill->GetDesc());
		delete pSkill;
	}
	for(i = 0; i < itemdataman::_pet_bedge_essence::MAX_PET_SKILL_COUNT; i++ )
	{
		char szName[20];
		sprintf(szName, "Img_Skill%02d", i + 1);
		PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)GetDlgItem(szName);
		int idSkill = pPet->GetEssence().skills[i].id;
		CECSkill *pSkill = 0;

		int iLevel =  pPet->GetEssence().skills[i].level;
		if( idSkill!=0 )
			pSkill = new CECSkill(idSkill, iLevel);
		AString strName;
		strName.Format("Txt_Skill%02d", i+1);
		PAUILABEL pLabSkillName = (PAUILABEL)GetDlgItem(strName);


		if( pSkill )
		{
			if(pLabSkillName)
			{
				ACString strText;
				strText.Format(_AL("%s ^ffcb4a%d/%d"), pSkill->GetName(), pSkill->GetSkillLevel(), pSkill->GetSkillMaxLevel());
				pLabSkillName->SetText(strText);
			}
			pImage->SetHint(pSkill->GetDesc());
			const char* szIcon = GNET::ElementSkill::GetIcon(idSkill);
			AString strFile;
			af_GetFileTitle(szIcon, strFile);
			strFile.MakeLower();
			pImage->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_SKILL],
				GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_SKILL][strFile]);
			if( i == 10 && GetDlgItem("Img_Bone"))
			{
				if (pSkill->GetSkillID()>=1359&&pSkill->GetSkillID()<=1367)
				{
					((PAUIIMAGEPICTURE)GetDlgItem("Img_Bone"))->ClearCover();
					((PAUIIMAGEPICTURE)GetDlgItem("Img_Bone"))->SetHint(_AL(""));
					((PAUILABEL)GetDlgItem("Txt_Bone"))->SetText(_AL(""));
				}
				else
				{
					((PAUIIMAGEPICTURE)GetDlgItem("Img_Bone"))->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_SKILL],
						GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_SKILL][strFile]);
					((PAUIIMAGEPICTURE)GetDlgItem("Img_Bone"))->SetHint(pSkill->GetDesc());
					((PAUILABEL)GetDlgItem("Txt_Bone"))->SetText(pSkill->GetName());
				}
			}
			if( i == 9 )
			{
				((PAUIIMAGEPICTURE)GetDlgItem("Img_Race"))->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_SKILL],
					GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_SKILL][strFile]);
				((PAUIIMAGEPICTURE)GetDlgItem("Img_Race"))->SetHint(pSkill->GetDesc());
			}
			if( i == 8 && GetDlgItem("Img_Prelife") )
			{
				((PAUIIMAGEPICTURE)GetDlgItem("Img_Prelife"))->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_SKILL],
					GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_SKILL][strFile]);
				((PAUIIMAGEPICTURE)GetDlgItem("Img_Prelife"))->SetHint(pSkill->GetDesc());
				((PAUILABEL)GetDlgItem("Txt_Prelife"))->SetText(pSkill->GetName());
			}
			if( i == 11 && GetDlgItem("Img_Fight") )
			{
				((PAUIIMAGEPICTURE)GetDlgItem("Img_Fight"))->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_SKILL],
					GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_SKILL][strFile]);
				((PAUIIMAGEPICTURE)GetDlgItem("Img_Fight"))->SetHint(pSkill->GetDesc());
				((PAUILABEL)GetDlgItem("Txt_Fight"))->SetText(pSkill->GetName());
			}
			AUIClockIcon *pClock;
			pClock = pImage->GetClockIcon();
			int nMax;
			int nCool = pPet->GetSkillCoolTime(i, &nMax);
			if( nMax > 0 )
			{
				pClock->SetColor(A3DCOLORRGBA(0, 0, 0, 128));
				pClock->SetProgressRange(0, nMax);
				pClock->SetProgressPos(nMax - nCool);
			}
			else
			{
				pClock->SetProgressRange(0, 1);
				pClock->SetProgressPos(1);
			}
			delete pSkill;
		}
		else
		{
			pImage->ClearCover();
			pImage->SetHint(_AL(""));
			if( i == 10 && GetDlgItem("Img_Bone") )
			{
				((PAUIIMAGEPICTURE)GetDlgItem("Img_Bone"))->ClearCover();
				((PAUIIMAGEPICTURE)GetDlgItem("Img_Bone"))->SetHint(_AL(""));
				((PAUILABEL)GetDlgItem("Txt_Bone"))->SetText(_AL(""));
			}
			if( i == 9 )
			{
				((PAUIIMAGEPICTURE)GetDlgItem("Img_Race"))->ClearCover();
				((PAUIIMAGEPICTURE)GetDlgItem("Img_Race"))->SetHint(_AL(""));
				((PAUILABEL)GetDlgItem("Txt_Bone"))->SetText(_AL(""));
			}
			if( i == 8 && GetDlgItem("Img_Prelife") )
			{
				((PAUIIMAGEPICTURE)GetDlgItem("Img_Prelife"))->ClearCover();
				((PAUIIMAGEPICTURE)GetDlgItem("Img_Prelife"))->SetHint(_AL(""));
				((PAUILABEL)GetDlgItem("Txt_Prelife"))->SetText(_AL(""));
			}
			if( i == 11 && GetDlgItem("Img_Fight") )
			{
				((PAUIIMAGEPICTURE)GetDlgItem("Img_Fight"))->ClearCover();
				((PAUIIMAGEPICTURE)GetDlgItem("Img_Fight"))->SetHint(_AL(""));
				((PAUILABEL)GetDlgItem("Txt_Fight"))->SetText(_AL(""));
			}
		}
	}
	for(i = 0; i < itemdataman::_pet_bedge_essence::MAX_PET_TANLENT_COUNT; i++ )
	{
		char szName[20];
		sprintf(szName, "Img_Talent%02d", i + 1);
		PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)GetDlgItem(szName);
		if (!pImage)
		{
			break;
		}
		pImage->FixFrame(pPet->GetEssence().talents[i] / 5);
		CECSkill* pSkill = new CECSkill(1210 + i, 1);
		if( pSkill )
		{
			pImage->SetHint(pSkill->GetDesc());
			delete pSkill;
		}
	}
}

void CDlgPetDetail::SetPropAdded(void *pData)
{
	memcpy(&(m_PetPropadded[((S2C::player_pet_prop_added*)pData)->pet_index]), pData, sizeof(S2C::player_pet_prop_added));
}

void CDlgPetDetail::SetInfo(int index, CECPetBedge *pPet)
{
	if (GetDataPtr())
	{
		CECPetBedge *pPet = (CECPetBedge*)GetDataPtr();
		delete pPet;
		SetDataPtr(NULL);
	}
	if (index == -1)
	{
		SetData(0xffffffff);
		SetDataPtr(pPet);
	}
	else
	{
		SetData(index);
	}
}


AUI_BEGIN_COMMAND_MAP(CDlgPetDetailOperate, CDlgBase)

AUI_ON_COMMAND("Btn_ChangeName",		OnCommandChangeName)
AUI_ON_COMMAND("Btn_ChangeHoroscope",	OnCommandChangeHoroscope)
AUI_ON_COMMAND("Btn_ChangeImge",		OnCommandChangeImage)
AUI_ON_COMMAND("Btn_ChangePrelife",		OnCommandChangePreLife)
AUI_ON_COMMAND("Btn_ChangeNature",		OnCommandChangeNature)
AUI_ON_COMMAND("Btn_ChangeFight",		OnCommandChangeFight)
AUI_ON_COMMAND("Btn_ChangeBone",		OnCommandChangeBone)
AUI_ON_COMMAND("Btn_ChangeBasicType",	OnCommandChangeBasicType)
AUI_ON_COMMAND("Btn_ChangeLife",		OnCommandChangeLife)
AUI_ON_COMMAND("Btn_UpNum",				OnCommandUpNum)
AUI_ON_COMMAND("Btn_LvUpAttri",			OnCommandFlyAttrOpt)
AUI_ON_COMMAND("Btn_ChangeSkill",		OnCommandChangeSkill)
AUI_ON_COMMAND("Btn_ExpandSkill",		OnCommandExpandSkill)
AUI_ON_COMMAND("Btn_Close",				OnCommandCancel)
AUI_ON_COMMAND("IDCANCEL",				OnCommandCancel)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgPetDetailOperate, CDlgBase)

AUI_ON_EVENT("Img_Head",			WM_LBUTTONDOWN,		OnEventLButtonDown)

AUI_END_EVENT_MAP()

bool CDlgPetDetailOperate::PrepareOpt()
{
	if (GetData()==-1)
	{
		return false;
	}
	CECPetBedge *pPet = (CECPetBedge*)GetHostPlayer()->GetPack()->GetItem(GetData());
	if (!pPet)
	{
		return false;
	}

	GetGameUIMan()->GetDialog("Win_Pet_ChangeName")->Show(false);
	GetGameUIMan()->GetDialog("Win_Pet_ChangeBone")->Show(false);
	GetGameUIMan()->GetDialog("Win_Pet_ChangeFight")->Show(false);
	GetGameUIMan()->GetDialog("Win_Pet_ChangeHoroscope")->Show(false);
	GetGameUIMan()->GetDialog("Win_Pet_ChangeImage")->Show(false);
	GetGameUIMan()->GetDialog("Win_Pet_ChangeLife")->Show(false);
	GetGameUIMan()->GetDialog("Win_Pet_ChangeNature")->Show(false);
	GetGameUIMan()->GetDialog("Win_Pet_ChangePrelife")->Show(false);
	GetGameUIMan()->GetDialog("Win_Pet_ChangeSkill")->Show(false);
	GetGameUIMan()->GetDialog("Win_Pet_ExpandSkill")->Show(false);
	GetGameUIMan()->GetDialog("Win_Pet_UpNum")->Show(false);
	GetGameUIMan()->GetDialog("Win_Pet_ChangeBasicType")->Show(false);

	return true;
}

void CDlgPetDetailOperate::ClearDisplay()
{
	PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_Lord");		// 主人
	pLab->SetText(_AL(""));
	pLab = (PAUILABEL)GetDlgItem("Txt_Nature");				// 特质
	pLab->SetText(_AL(""));
	pLab = (PAUILABEL)GetDlgItem("Txt_BasicType");			// 限界
	pLab->SetText(_AL(""));

	GetDlgItem("Txt_SkillDefinition")->SetText(_AL(""));
	GetDlgItem("Txt_SkillEvade")->SetText(_AL(""));
	GetDlgItem("Txt_DeCriticalRate")->SetText(_AL(""));
	GetDlgItem("Txt_DeCriticalDamage")->SetText(_AL(""));
	GetDlgItem("Txt_DeDamage")->SetText(_AL(""));
	GetDlgItem("Txt_DeDamageRate")->SetText(_AL(""));

	GetDlgItem("Txt_Name")->SetText(_AL(""));
	GetDlgItem("Txt_Lv")->SetText(_AL(""));
	GetDlgItem("Txt_Exp")->SetText(_AL(""));
	GetDlgItem("Txt_StarLv")->SetText(_AL(""));
	GetDlgItem("Txt_StarLv2")->SetText(_AL(""));
	GetDlgItem("Txt_Life")->SetText(_AL(""));
	GetDlgItem("Txt_Full")->SetText(_AL(""));
	GetDlgItem("Txt_Love")->SetText(_AL(""));
	GetDlgItem("Txt_Horoscope")->SetText(_AL(""));
	int i;
	for (i=0; i<DLGPETDETAIL_MAX_TYPE-1; i++)
	{
		int cx1 = m_nLength * m_nLevels[0][i] / g_nMaxValue;
		int cy1 = m_pPrgLevel[i][0]->GetSize().cy;
		m_pPrgLevel[i][0]->SetSize(cx1,cy1);

		int cx2 = m_nLength * (m_nLevelValue[m_nLevelsIndex[0][i]+5] - m_nLevels[0][i] )/ g_nMaxValue;
		int cy2 = m_pPrgLevel[i][1]->GetSize().cy;
		int x2 = m_pPrgLevel[i][0]->GetPos(true).x + cx1;
		int y2 = m_pPrgLevel[i][1]->GetPos(true).y;
		m_pPrgLevel[i][1]->SetPos(x2, y2);
		m_pPrgLevel[i][1]->SetSize(cx2,cy2);
	}
	GetDlgItem("Txt_Element")->SetText(_AL(""));
	GetDlgItem("Txt_Race")->SetText(_AL(""));
	GetDlgItem("Txt_Hp")->SetText(_AL(""));
	GetDlgItem("Txt_IniHp")->SetText(_AL(""));
	GetDlgItem("Txt_TalentHp")->SetText(_AL(""));
	GetDlgItem("Txt_Mp")->SetText(_AL(""));
	GetDlgItem("Txt_IniMp")->SetText(_AL(""));
	GetDlgItem("Txt_TalentMp")->SetText(_AL(""));
	GetDlgItem("Txt_MinAttack")->SetText(_AL(""));
	GetDlgItem("Txt_IniMinAttack")->SetText(_AL(""));
	GetDlgItem("Txt_TalentMinAttack")->SetText(_AL(""));
	GetDlgItem("Txt_MaxAttack")->SetText(_AL(""));
	GetDlgItem("Txt_IniMaxAttack")->SetText(_AL(""));
	GetDlgItem("Txt_TalentMaxAttack")->SetText(_AL(""));
	GetDlgItem("Txt_Defence")->SetText(_AL(""));
	GetDlgItem("Txt_IniDefence")->SetText(_AL(""));
	GetDlgItem("Txt_TalentDefence")->SetText(_AL(""));
	for (i=0; i<12; ++i)
	{
		m_pPrgLevel[i][0]->SetProgress(AUIPROGRESS_MAX);
		m_pPrgLevel[i][1]->SetProgress(AUIPROGRESS_MAX);
	}

	GetDlgItem("Txt_Definition")->SetText(_AL(""));
	GetDlgItem("Txt_IniDefinition")->SetText(_AL(""));
	GetDlgItem("Txt_TalentDefinition")->SetText(_AL(""));
	GetDlgItem("Txt_Evade")->SetText(_AL(""));
	GetDlgItem("Txt_IniEvade")->SetText(_AL(""));
	GetDlgItem("Txt_TalentEvade")->SetText(_AL(""));
	GetDlgItem("Txt_StunRes")->SetText(_AL(""));
	GetDlgItem("Txt_IniStunRes")->SetText(_AL(""));
	GetDlgItem("Txt_TalentStunRes")->SetText(_AL(""));
	GetDlgItem("Txt_WeaknessRes")->SetText(_AL(""));
	GetDlgItem("Txt_IniWeaknessRes")->SetText(_AL(""));
	GetDlgItem("Txt_TalentWeaknessRes")->SetText(_AL(""));
	GetDlgItem("Txt_EnlaceRes")->SetText(_AL(""));
	GetDlgItem("Txt_IniEnlaceRes")->SetText(_AL(""));
	GetDlgItem("Txt_TalentEnlaceRes")->SetText(_AL(""));
	GetDlgItem("Txt_CriticalRate")->SetText(_AL(""));
	GetDlgItem("Txt_CriticalDamage")->SetText(_AL(""));
	GetDlgItem("Txt_SilenceRes")->SetText(_AL(""));
	GetDlgItem("Txt_IniSilenceRes")->SetText(_AL(""));
	GetDlgItem("Txt_TalentSilenceRes")->SetText(_AL(""));
	GetDlgItem("Txt_SleepRes")->SetText(_AL(""));
	GetDlgItem("Txt_IniSleepRes")->SetText(_AL(""));
	GetDlgItem("Txt_TalentSleepRes")->SetText(_AL(""));
	GetDlgItem("Txt_Prelife")->SetText(_AL(""));
	GetDlgItem("Txt_Fight")->SetText(_AL(""));
	GetDlgItem("Txt_Bone")->SetText(_AL(""));
	m_pObjFlyAttrButton->Enable(false);
	m_pObjFlyAttrButton->SetHint(GetGameUIMan()->GetStringFromTable(3599));

	//生活能力
	for (i=0; i<6; ++i)
	{
		AString strName;
		strName.Format("Txt_Skill%02d", i+1);
		PAUILABEL pLab = (PAUILABEL)GetDlgItem(strName);
		pLab->SetText(_AL(""));
		strName.Format("Txt_Live%d", i);
		GetDlgItem(strName)->SetText(_AL(""));
		strName.Format("Txt_Skill%02d", i+1);
		pLab = (PAUILABEL)GetDlgItem(strName);
		pLab->SetText(_AL(""));
	}

	((PAUIIMAGEPICTURE)GetDlgItem("Img_Horoscope"))->ClearCover();
	((PAUIIMAGEPICTURE)GetDlgItem("Img_Horoscope"))->SetHint(_AL(""));
	((PAUIIMAGEPICTURE)GetDlgItem("Img_Race"))->ClearCover();
	((PAUIIMAGEPICTURE)GetDlgItem("Img_Race"))->SetHint(_AL(""));
	((PAUIIMAGEPICTURE)GetDlgItem("Img_Prelife"))->ClearCover();
	((PAUIIMAGEPICTURE)GetDlgItem("Img_Prelife"))->SetHint(_AL(""));
	((PAUIIMAGEPICTURE)GetDlgItem("Img_Fight"))->ClearCover();
	((PAUIIMAGEPICTURE)GetDlgItem("Img_Fight"))->SetHint(_AL(""));
	((PAUIIMAGEPICTURE)GetDlgItem("Img_Bone"))->ClearCover();
	((PAUIIMAGEPICTURE)GetDlgItem("Img_Bone"))->SetHint(_AL(""));
	AString strName;
	for(i = 0; i < itemdataman::_pet_bedge_essence::MAX_PET_SKILL_COUNT; i++ )
	{
		strName.Format("Img_Skill%02d", i + 1);
		PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)GetDlgItem(strName);

		pImage->ClearCover();
		pImage->SetHint(_AL(""));
	}
	for(i = 0; i < itemdataman::_pet_bedge_essence::MAX_PET_TANLENT_COUNT; i++ )
	{
		char szName[20];
		sprintf(szName, "Img_Talent%02d", i + 1);
		PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)GetDlgItem(szName);
		if (pImage)
		{
			pImage->FixFrame(0);
			pImage->SetHint(_AL(""));
		}
	}
	
	for (i=0; i<4; i++)
	{
		strName.Format("Img_Star%d", i+1);
		((PAUIIMAGEPICTURE)GetDlgItem(strName))->Show(false);
	}
	SwitchFlyAttr(false);
}

void CDlgPetDetailOperate::OnCommandChangeName(const char * szCommand)
{
	if (!PrepareOpt())
		return;

	CECPetBedge *pPet = (CECPetBedge*)GetHostPlayer()->GetPack()->GetItem(GetData());
	CDlgPetReName *pDlgPet = (CDlgPetReName*)GetGameUIMan()->GetDialog("Win_Pet_ChangeName");
	pDlgPet->SetItem(pPet, GetData());
	GetGameUIMan()->GetDialog("Win_Pet_ChangeName")->Show(true);
	GetGameUIMan()->GetDialog("Win_Inventory")->Show(true);
}

void CDlgPetDetailOperate::OnCommandChangeBone(const char * szCommand)
{
	if (!PrepareOpt())
		return;
	
	CECPetBedge *pPet = (CECPetBedge*)GetHostPlayer()->GetPack()->GetItem(GetData());
	CDlgPetCombineExchange *pDlgPetCombine = (CDlgPetCombineExchange*)GetGameUIMan()->GetDialog("Win_Pet_ChangeBone");
	pDlgPetCombine->SetData(GetData(), "pet_index");
	GetGameUIMan()->GetDialog("Win_Pet_ChangeBone")->Show(true);
	GetGameUIMan()->GetDialog("Win_Inventory")->Show(true);	
}

void CDlgPetDetailOperate::OnCommandChangeFight(const char * szCommand)
{
	if (!PrepareOpt())
		return;
	
	CECPetBedge *pPet = (CECPetBedge*)GetHostPlayer()->GetPack()->GetItem(GetData());
	CDlgPetCombineExchange *pDlgPetCombine = (CDlgPetCombineExchange*)GetGameUIMan()->GetDialog("Win_Pet_ChangeFight");
	pDlgPetCombine->SetData(GetData(), "pet_index");
	GetGameUIMan()->GetDialog("Win_Pet_ChangeFight")->Show(true);
	GetGameUIMan()->GetDialog("Win_Inventory")->Show(true);	
}

void CDlgPetDetailOperate::OnCommandChangeHoroscope(const char * szCommand)
{
	if (!PrepareOpt())
		return;
	
	CECPetBedge *pPet = (CECPetBedge*)GetHostPlayer()->GetPack()->GetItem(GetData());
	CDlgPetCombine *pDlgPetCombine = (CDlgPetCombine*)GetGameUIMan()->GetDialog("Win_Pet_ChangeHoroscope");
	pDlgPetCombine->SetData(GetData(), "pet_index");
	GetGameUIMan()->GetDialog("Win_Pet_ChangeHoroscope")->Show(true);
	GetGameUIMan()->GetDialog("Win_Inventory")->Show(true);	
}

void CDlgPetDetailOperate::OnCommandChangeImage(const char * szCommand)
{
	if (!PrepareOpt())
		return;
	
	CECPetBedge *pPet = (CECPetBedge*)GetHostPlayer()->GetPack()->GetItem(GetData());
	CDlgPetCombine *pDlgPetCombine = (CDlgPetCombine*)GetGameUIMan()->GetDialog("Win_Pet_ChangeImage");
	pDlgPetCombine->SetData(GetData(), "pet_index");
	GetGameUIMan()->GetDialog("Win_Pet_ChangeImage")->Show(true);
	GetGameUIMan()->GetDialog("Win_Inventory")->Show(true);	
}

void CDlgPetDetailOperate::OnCommandChangeLife(const char * szCommand)
{
	if (!PrepareOpt())
		return;
	
	CECPetBedge *pPet = (CECPetBedge*)GetHostPlayer()->GetPack()->GetItem(GetData());
	CDlgPetCombine *pDlgPetCombine = (CDlgPetCombine*)GetGameUIMan()->GetDialog("Win_Pet_ChangeLife");
	pDlgPetCombine->SetData(GetData(), "pet_index");
	GetGameUIMan()->GetDialog("Win_Pet_ChangeLife")->Show(true);
	GetGameUIMan()->GetDialog("Win_Inventory")->Show(true);	
}

void CDlgPetDetailOperate::OnCommandChangeNature(const char * szCommand)
{
	if (!PrepareOpt())
		return;
	
	CECPetBedge *pPet = (CECPetBedge*)GetHostPlayer()->GetPack()->GetItem(GetData());
	CDlgPetCombineExchange *pDlgPetCombine = (CDlgPetCombineExchange*)GetGameUIMan()->GetDialog("Win_Pet_ChangeNature");
	pDlgPetCombine->SetData(GetData(), "pet_index");
	GetGameUIMan()->GetDialog("Win_Pet_ChangeNature")->Show(true);
	GetGameUIMan()->GetDialog("Win_Inventory")->Show(true);	
}

void CDlgPetDetailOperate::OnCommandChangePreLife(const char * szCommand)
{
	if (!PrepareOpt())
		return;
	
	CECPetBedge *pPet = (CECPetBedge*)GetHostPlayer()->GetPack()->GetItem(GetData());
	CDlgPetCombineExchange *pDlgPetCombine = (CDlgPetCombineExchange*)GetGameUIMan()->GetDialog("Win_Pet_ChangePrelife");
	pDlgPetCombine->SetData(GetData(), "pet_index");
	GetGameUIMan()->GetDialog("Win_Pet_ChangePrelife")->Show(true);
	GetGameUIMan()->GetDialog("Win_Inventory")->Show(true);	
}

void CDlgPetDetailOperate::OnCommandChangeSkill(const char * szCommand)
{
	if (!PrepareOpt())
		return;
	
	CECPetBedge *pPet = (CECPetBedge*)GetHostPlayer()->GetPack()->GetItem(GetData());
	CDlgPetCombineExchange *pDlgPetCombine = (CDlgPetCombineExchange*)GetGameUIMan()->GetDialog("Win_Pet_ChangeSkill");
	pDlgPetCombine->SetData(GetData(), "pet_index");
	GetGameUIMan()->GetDialog("Win_Pet_ChangeSkill")->Show(true);
	GetGameUIMan()->GetDialog("Win_Inventory")->Show(true);	
}

void CDlgPetDetailOperate::OnCommandExpandSkill(const char * szCommand)
{
	if (!PrepareOpt())
		return;
	
	CECPetBedge *pPet = (CECPetBedge*)GetHostPlayer()->GetPack()->GetItem(GetData());
	CDlgPetCombineExchange *pDlgPetCombine = (CDlgPetCombineExchange*)GetGameUIMan()->GetDialog("Win_Pet_ExpandSkill");
	pDlgPetCombine->SetData(GetData(), "pet_index");
	GetGameUIMan()->GetDialog("Win_Pet_ExpandSkill")->Show(true);
	GetGameUIMan()->GetDialog("Win_Inventory")->Show(true);	
}

void CDlgPetDetailOperate::OnCommandUpNum(const char * szCommand)
{
	if (!PrepareOpt())
		return;
	
	CECPetBedge *pPet = (CECPetBedge*)GetHostPlayer()->GetPack()->GetItem(GetData());
	CDlgPetCombine *pDlgPetCombine = (CDlgPetCombine*)GetGameUIMan()->GetDialog("Win_Pet_UpNum");
	pDlgPetCombine->SetData(GetData(), "pet_index");
	GetGameUIMan()->GetDialog("Win_Pet_UpNum")->Show(true);
	GetGameUIMan()->GetDialog("Win_Inventory")->Show(true);	
}

void CDlgPetDetailOperate::OnCommandChangeBasicType(const char * szCommand)
{
	if (!PrepareOpt())
		return;
	
	CECPetBedge *pPet = (CECPetBedge*)GetHostPlayer()->GetPack()->GetItem(GetData());
	CDlgPetCombineExchange *pDlgPetCombine = (CDlgPetCombineExchange*)GetGameUIMan()->GetDialog("Win_Pet_ChangeBasicType");
	pDlgPetCombine->SetData(GetData(), "pet_index");
	GetGameUIMan()->GetDialog("Win_Pet_ChangeBasicType")->Show(true);
	GetGameUIMan()->GetDialog("Win_Inventory")->Show(true);	
}

void CDlgPetDetailOperate::OnCommandFlyAttrOpt(const char * szCommand)
{
	if (GetData()==0xffffffff)
	{
		return;
	}
	CECPetBedge *pPet = (CECPetBedge*)GetHostPlayer()->GetPack()->GetItem(GetData());
	if (pPet->GetEssence().reborn_cnt==0)
		return;

	PAUIOBJECT pObj = GetDlgItem("Sub_01");
	if (pObj->IsShow())
	{
		SwitchFlyAttr(false);
	}
	else
		SwitchFlyAttr(true);
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Pet_Detail_LvupAttri");
	pDlg->SetData(GetData(), "pet_index");
}

void CDlgPetDetailOperate::SwitchFlyAttr(bool bFly)
{
	POINT ptRect = GetDlgItem("Shape4")->GetPos(true);
	SIZE szRect = GetDlgItem("Shape4")->GetSize();

	PAUIOBJECTLISTELEMENT pElement = GetFirstControl();
	while( pElement )
	{
		POINT pt = pElement->pThis->GetPos(true);
		if( pt.x>ptRect.x && pt.x<(ptRect.x+szRect.cx) &&
			pt.y>ptRect.y && pt.y<(ptRect.y+szRect.cy))
		{
			pElement->pThis->Show(!bFly);
		}
		pElement = pElement->pNext;
	}
	int i, j;
	for (i=0; i<2; i++)
	{
		for (j=0; j<12; j++)
		{
			m_pPrgLevel[j][i]->Show(false);
		}
	}

	GetDlgItem("Sub_01")->Show(bFly);
}

void CDlgPetDetailOperate::OnCommandCancel(const char * szCommand)
{
	PrepareOpt();
	Show(false);
	OnEventLButtonDown(0, 0, GetDlgItem("Img_Head"));
	SetDataPtr(NULL);
	GetGameUIMan()->EndNPCService();
	GetGameUIMan()->UnfreezeAllItems();
}

void CDlgPetDetailOperate::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	PrepareOpt();
	ClearDisplay();
	if (GetData()==0xffffffff)
	{
		return;
	}
	CECPetBedge *pPet = (CECPetBedge*)GetHostPlayer()->GetPack()->GetItem(GetData());
	if (!pPet)
	{
		return;
	}
	pPet->Freeze(false);
	SetData(0xffffffff);
	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)pObj;
	GetGameUIMan()->SetIvtrInfo(pImg, 0);
	if (m_bLoaded)
	{
		m_pPet->Release();
		m_bLoaded = false;
	}
}

void CDlgPetDetailOperate::SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg)
{
	PrepareOpt();
	DoSetItem(pItem, nSlot, pImg);
}
	
void CDlgPetDetailOperate::DoSetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg)
{
	if (!pImg)
		pImg = (PAUIIMAGEPICTURE)GetDlgItem("Img_Head");
	
	if (pImg!=(PAUIIMAGEPICTURE)GetDlgItem("Img_Head"))
		return;

	CECPetBedge *pPetItem = (CECPetBedge*)pItem;
	if (0==pPetItem->GetEssence().identify || pPetItem->GetOwnerName()!=ACString(GetHostPlayer()->GetName()))
	{
		GetGameUIMan()->AddChatMessage(GetStringFromTable(15014), GP_CHAT_MISC);
		return;
	}
	
	if (nSlot!=(int)GetData() && GetData()!=0xffffffff)
	{
		CECPetBedge *pPet = (CECPetBedge*)GetHostPlayer()->GetPack()->GetItem(GetData());
		if(pPet)
			pPet->Freeze(false);
	}
	
	SetData(nSlot);
	GetGameUIMan()->SetIvtrInfo(pImg, pItem);
	pItem->Freeze(true);
	pImg->SetData(nSlot);
	pImg->SetDataPtr(pItem, "ptr_CECIvtrItem");
	m_pObjFlyAttrButton->Enable(pPetItem->GetEssence().reborn_cnt>0);
	m_pObjFlyAttrButton->SetHint(pPetItem->GetEssence().reborn_cnt>0 ? _AL("") : GetGameUIMan()->GetStringFromTable(3599));

	GetDlgItem("Lab_SkillDefinition")->Show(pPetItem->GetEssence().reborn_cnt>0);
	GetDlgItem("Txt_SkillDefinition")->Show(pPetItem->GetEssence().reborn_cnt>0);
	GetDlgItem("Lab_SkillEvade")->Show(pPetItem->GetEssence().reborn_cnt>0);
	GetDlgItem("Txt_SkillEvade")->Show(pPetItem->GetEssence().reborn_cnt>0);
	GetDlgItem("Lab_DeCriticalRate")->Show(pPetItem->GetEssence().reborn_cnt>0);
	GetDlgItem("Txt_DeCriticalRate")->Show(pPetItem->GetEssence().reborn_cnt>0);
	GetDlgItem("Lab_DeCriticalDamage")->Show(pPetItem->GetEssence().reborn_cnt>0);
	GetDlgItem("Txt_DeCriticalDamage")->Show(pPetItem->GetEssence().reborn_cnt>0);
	GetDlgItem("Lab_DeDamage")->Show(pPetItem->GetEssence().reborn_cnt>0);
	GetDlgItem("Txt_DeDamage")->Show(pPetItem->GetEssence().reborn_cnt>0);
	GetDlgItem("Lab_DeDamageRate")->Show(pPetItem->GetEssence().reborn_cnt>0);
	GetDlgItem("Txt_DeDamageRate")->Show(pPetItem->GetEssence().reborn_cnt>0);
	SwitchFlyAttr(false);
}

int CDlgPetDetailOperate::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="continue_opt")
	{
		if (GetGameUIMan()->GetDialog("Win_Pet_ChangeFight")->IsShow())
		{
			GetGameUIMan()->CommonMessage("Win_Pet_ChangeFight", "continue_opt", 0, 0);
		}
		else if (GetGameUIMan()->GetDialog("Win_Pet_UpNum")->IsShow())
		{
			GetGameUIMan()->CommonMessage("Win_Pet_UpNum", "continue_opt", 0, 0);
		}
		else if (GetGameUIMan()->GetDialog("Win_Pet_ChangeSkill")->IsShow())
		{
			GetGameUIMan()->CommonMessage("Win_Pet_ChangeSkill", "continue_opt", 0, 0);
		}
	}
	else if (strMsg=="stop_opt")
	{
		if (GetGameUIMan()->GetDialog("Win_Pet_ChangeFight")->IsShow())
		{
			GetGameUIMan()->CommonMessage("Win_Pet_ChangeFight", "stop_opt", 0, 0);
		}
		else if (GetGameUIMan()->GetDialog("Win_Pet_UpNum")->IsShow())
		{
			GetGameUIMan()->CommonMessage("Win_Pet_UpNum", "stop_opt", 0, 0);
		}
		else if (GetGameUIMan()->GetDialog("Win_Pet_ChangeSkill")->IsShow())
		{
			GetGameUIMan()->CommonMessage("Win_Pet_ChangeSkill", "stop_opt", 0, 0);
		}
	}
	return 0;
}


bool CDlgPetDetailOperate::OnInitDialog()
{
	bool bRet =CDlgPetDetail::OnInitDialog();
	m_pBtnLeftTurn = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_LeftTurn");
	m_pBtnRightTurn = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_RightTurn");
	m_pImgPet = (PAUIIMAGEPICTURE)GetDlgItem("Img_3D");
	m_pObjFlyAttrButton = GetDlgItem("Btn_LvUpAttri");
	m_pObjFlyAttrButton->Enable(false);
	m_pPet = new CECModel;
	m_bLoaded = false;
	SetData(0xffffffff);
	ClearDisplay();
	return bRet;
}

bool CDlgPetDetailOperate::Release()
{
	A3DRELEASE(m_pPet);
	return CDlgPetDetail::Release();
}

void CDlgPetDetailOperate::OnTick()
{
	m_pImgPet->SetRenderCallback(NULL,	(DWORD)m_pPet, MRT_PET, m_iAngle);
	if (GetGameUIMan()->GetActiveDialog()==this && GetGameUIMan()->GetDialog("MagicProgress1")->IsShow())
	{
		GetGameUIMan()->BringWindowToTop(GetGameUIMan()->GetDialog("MagicProgress1"));
	}

	// 是否放入宠物
	if (GetData()==0xffffffff)
	{
		return;
	}
	CECPetBedge *pPet = (CECPetBedge*)GetHostPlayer()->GetPack()->GetItem(GetData());
	if (!pPet)
	{
		return;
	}

	// 修炼外观可能改变宠物id跟指针
	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem("Img_Head");
	if (pImg->GetDataPtr("ptr_CECIvtrItem")!=pPet)
	{
		DoSetItem(pPet, GetData());
		// 渲染3D造型
		if (m_bLoaded)
		{
			if( m_pBtnLeftTurn->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
				m_iAngle = (m_iAngle + GetGame()->GetTickTime() / 6 ) % 360;
			else if( m_pBtnRightTurn->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
				m_iAngle = (m_iAngle + 360 - GetGame()->GetTickTime() / 6) % 360;
			m_pPet->Tick(GetGame()->GetTickTime());
			m_pImgPet->SetRenderCallback(DlgModelRenderFixDist,	(DWORD)m_pPet, (DWORD)(&m_fModelSize), m_iAngle);
		}
		else
			m_pImgPet->SetRenderCallback(NULL,	(DWORD)m_pPet, MRT_PET, m_iAngle);
		return;
	}
	AString strPath = GetGame()->GetDataPath(pPet->GetDBEssence()->file_to_shown[pPet->GetEssence().shape-1]);
	if (!m_bLoaded || strPath!=m_pPet->GetFilePath())
	{
		if (m_bLoaded)
		{
			m_pPet->Release();
			m_bLoaded = false;
		}
		
		m_pA3DEngine->GetA3DSkinMan()->SetReplaceShaderFile("armor.sdr", A3DSkinMan::SHADERREPLACE_REFLECTPREFIX);
		if ( m_pPet->Load(strPath) )
		{
			//		if (m_pPet->GetSkinNum()==0)
			{
				A3DSkin* pSkin;
				char szSkin[MAX_PATH];
				strncpy(szSkin, strPath, MAX_PATH);
				glb_ChangeExtension(szSkin, "ski");
				
				if (pSkin = GetGame()->LoadA3DSkin(szSkin, true))
					m_pPet->GetA3DSkinModel()->AddSkin(pSkin, true);
			}
			const A3DAABB& chAABB = m_pPet->HasCHAABB() ? m_pPet->GetCHAABB() : m_pPet->GetModelAABB();
			m_fModelSize = chAABB.Extents.y;
			m_pPet->SetGfxUseLOD(false);
			m_pPet->SetPos(-chAABB.Center);
			m_pPet->SetDirAndUp(A3DVECTOR3(0, 0, -1.0f), A3DVECTOR3(0, 1.0f, 0));
			m_pPet->SetAutoUpdateFlag(false);
			m_pPet->Tick(1);
			m_pPet->SetAutoUpdateFlag(true);
			m_pPet->PlayActionByName(CECNPC::GetActionName(CECNPC::ACT_IDLE), 1.0f, false);
			m_pPet->QueueAction(CECNPC::GetActionName(CECNPC::ACT_STAND));
			m_bLoaded = true;
		}
		m_pA3DEngine->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
	}
	// 渲染3D造型
	if (m_bLoaded)
	{
		if( m_pBtnLeftTurn->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
			m_iAngle = (m_iAngle + GetGame()->GetTickTime() / 6 ) % 360;
		else if( m_pBtnRightTurn->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
			m_iAngle = (m_iAngle + 360 - GetGame()->GetTickTime() / 6) % 360;
		m_pPet->Tick(GetGame()->GetTickTime());
		m_pImgPet->SetRenderCallback(DlgModelRenderFixDist,	(DWORD)m_pPet, (DWORD)(&m_fModelSize), m_iAngle);
	}
	else
		m_pImgPet->SetRenderCallback(NULL,	(DWORD)m_pPet, MRT_PET, m_iAngle);

	pPet->Freeze(true);
	// 更新显示
	if (pPet->GetEssence().identify)
	{
		UpdateDisplay(pPet);

		ACString strText;
		PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_Lord");			// 主人
		AUI_ConvertChatString(pPet->GetOwnerName(), strText, false);
		AUICTranslate trans;
		ACString ownerName = trans.ReverseTranslate(strText);		// 名字中有'\\'，转为'\\'+'\\'，因为updatehint的时候会调用translate
		pLab->SetText(ownerName);

		CECStringTab* pDescTab = GetGame()->GetItemDesc();
		pLab = (PAUILABEL)GetDlgItem("Txt_Nature");				// 特质
		
		for (int i = 0; i < 13; i++)
		{
			if (pPet->GetEssence().talents[i] == 10)
			{
				CECSkill* pSkill = new CECSkill(1210 + i, 1);
				if (pSkill)
				{
					pLab->SetText(GetGame()->GetSkillDesc()->GetWideString(pSkill->GetSkillID() * 10));
					delete pSkill;
				}
				break;
			}
		}
		
		pLab = (PAUILABEL)GetDlgItem("Txt_BasicType");			// 限界
		pLab->SetText(pDescTab->GetWideString(ITEMDESC_PET_MAINTYPE0+pPet->GetEssence().main_type));

	}
}
