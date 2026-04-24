// File		: DlgPetSetting.cpp
// Creator	: Xiao Zhou
// Date		: 2008/4/3

#include "AFI.h"
#include "AUI\\AUIRadioButton.h"
#include "AUI\\AUIProgress.h"
#include "DlgPetSetting.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_IvtrConsume.h"
#include "EC_Inventory.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgPetSetting, CDlgBase)

AUI_ON_COMMAND("confirm",	OnCommandConfirm)

AUI_END_COMMAND_MAP()


CDlgPetSetting::CDlgPetSetting()
{
}

CDlgPetSetting::~CDlgPetSetting()
{
}

bool CDlgPetSetting::OnInitDialog()
{

	return true;
}

void CDlgPetSetting::OnShowDialog()
{
	CECPetBedge *pPet = (CECPetBedge *)GetHostPlayer()->GetPetPack()->GetItem(GetData());
	if( pPet )
	{
		if( stricmp(GetName(), "Win_PetProSetting") == 0 )
		{
			CheckRadioButton(1, pPet->GetEssence().rank);
			ACHAR szText[20];
			a_sprintf(szText, _AL("%d"), GetHostPlayer()->GetPetCivilization());
			GetDlgItem("Txt_Civil")->SetText(szText);
			GetDlgItem("Radio_1")->Enable(GetHostPlayer()->GetPetCivilization() >= 1 && GetPetRankNumber(1, GetData()) < 32 && pPet->GetEssence().level >= 10);
			GetDlgItem("Radio_2")->Enable(GetHostPlayer()->GetPetCivilization() >= 18000 && GetPetRankNumber(2, GetData()) < 16 && pPet->GetEssence().level >= 30);
			GetDlgItem("Radio_3")->Enable(GetHostPlayer()->GetPetCivilization() >= 144000 && GetPetRankNumber(3, GetData()) < 8 && pPet->GetEssence().level >= 60);
			GetDlgItem("Radio_4")->Enable(GetHostPlayer()->GetPetCivilization() >= 288000 && GetPetRankNumber(4, GetData()) < 4 && pPet->GetEssence().level >= 90);
			GetDlgItem("Radio_5")->Enable(GetHostPlayer()->GetPetCivilization() >= 1152000 && GetPetRankNumber(5, GetData()) < 2 && pPet->GetEssence().level >= 120);
			GetDlgItem("Radio_6")->Enable(GetHostPlayer()->GetPetCivilization() >= 2304000 && GetPetRankNumber(6, GetData()) < 1 && pPet->GetEssence().level >= 150);

			GetDlgItem("Radio_7")->Enable(GetHostPlayer()->GetPetCivilization() >= 1 && GetPetRankNumber(7, GetData()) < 32 && pPet->GetEssence().level >= 10);
			GetDlgItem("Radio_8")->Enable(GetHostPlayer()->GetPetCivilization() >= 9000 && GetPetRankNumber(8, GetData()) < 16 && pPet->GetEssence().level >= 30);
			GetDlgItem("Radio_9")->Enable(GetHostPlayer()->GetPetCivilization() >= 72000 && GetPetRankNumber(9, GetData()) < 8 && pPet->GetEssence().level >= 60);
			GetDlgItem("Radio_10")->Enable(GetHostPlayer()->GetPetCivilization() >= 144000 && GetPetRankNumber(10, GetData()) < 4 && pPet->GetEssence().level >= 90);
			GetDlgItem("Radio_11")->Enable(GetHostPlayer()->GetPetCivilization() >= 576000 && GetPetRankNumber(11, GetData()) < 2 && pPet->GetEssence().level >= 120);
			GetDlgItem("Radio_12")->Enable(GetHostPlayer()->GetPetCivilization() >= 1152000 && GetPetRankNumber(12, GetData()) < 1 && pPet->GetEssence().level >= 150);
		}
		else if( stricmp(GetName(), "Win_PetStsSetting") == 0 )
		{
			bool bLife = pPet->GetEssence().life > 0 && pPet->GetEssence().life > pPet->GetEssence().age;
			ACString strHint = _AL("");
			if( !bLife )
				strHint = GetStringFromTable(4501);
			((PAUIRADIOBUTTON)GetDlgItem("Radio_00"))->SetHint(strHint);
			((PAUIRADIOBUTTON)GetDlgItem("Radio_00"))->Enable(bLife);
			((PAUIRADIOBUTTON)GetDlgItem("Rdo_Combat01"))->SetHint(strHint);
			((PAUIRADIOBUTTON)GetDlgItem("Rdo_Combat01"))->Enable(bLife);
			((PAUIRADIOBUTTON)GetDlgItem("Rdo_Combat02"))->SetHint(strHint);
			((PAUIRADIOBUTTON)GetDlgItem("Rdo_Combat02"))->Enable(bLife);
			GetDlgItem("Radio_00")->Show(pPet->GetEssence().reborn_cnt<=0);
			GetDlgItem("Lab_0")->Show(pPet->GetEssence().reborn_cnt<=0);
			GetDlgItem("Rdo_Combat01")->Show(pPet->GetEssence().reborn_cnt>0);
			GetDlgItem("Rdo_Combat02")->Show(pPet->GetEssence().reborn_cnt>0);
			if( !pPet->GetEssence().identify )
			{
				if(strHint.IsEmpty() )
					strHint = GetStringFromTable(4502);
				else
					strHint = strHint + _AL("\r") + GetStringFromTable(4502);
			}
			ACString strHint1 = strHint, strHint2 = strHint;
			((PAUIRADIOBUTTON)GetDlgItem("Radio_10"))->Enable(bLife && pPet->GetEssence().identify && pPet->GetEssence().level >= 30);
			((PAUIRADIOBUTTON)GetDlgItem("Radio_11"))->Enable(bLife && pPet->GetEssence().identify && pPet->GetEssence().level >= 30);
			((PAUIRADIOBUTTON)GetDlgItem("Radio_12"))->Enable(bLife && pPet->GetEssence().identify && pPet->GetEssence().level >= 30);
			((PAUIRADIOBUTTON)GetDlgItem("Radio_13"))->Enable(bLife && pPet->GetEssence().identify && pPet->GetEssence().level >= 30);
			((PAUIRADIOBUTTON)GetDlgItem("Radio_14"))->Enable(bLife && pPet->GetEssence().identify && pPet->GetEssence().level >= 30);
			((PAUIRADIOBUTTON)GetDlgItem("Radio_15"))->Enable(bLife && pPet->GetEssence().identify && pPet->GetEssence().level >= 30);
			if( pPet->GetEssence().level < 30 )
			{
				if(strHint1.IsEmpty() )
					strHint1 = GetStringFromTable(4503);
				else
					strHint1 = strHint1 + _AL("\r") + GetStringFromTable(4503);
			}
			((PAUIRADIOBUTTON)GetDlgItem("Radio_10"))->SetHint(strHint1);
			((PAUIRADIOBUTTON)GetDlgItem("Radio_11"))->SetHint(strHint1);
			((PAUIRADIOBUTTON)GetDlgItem("Radio_12"))->SetHint(strHint1);
			((PAUIRADIOBUTTON)GetDlgItem("Radio_13"))->SetHint(strHint1);
			((PAUIRADIOBUTTON)GetDlgItem("Radio_14"))->SetHint(strHint1);
			((PAUIRADIOBUTTON)GetDlgItem("Radio_15"))->SetHint(strHint1);
			if( pPet->GetEssence().level < 60 )
			{
				if(strHint2.IsEmpty() )
					strHint2 = GetStringFromTable(4504);
				else
					strHint2 = strHint2 + _AL("\r") + GetStringFromTable(4504);
			}
			((PAUIRADIOBUTTON)GetDlgItem("Edit_Make"))->SetHint(strHint2);

			CheckRadioButton(1, pPet->GetEssence().main_status * 10 + pPet->GetEssence().sub_status);
			ACHAR szText[20];
			a_sprintf(szText, _AL("%d/100"), pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::ACUITY]);
			GetDlgItem("Lab_NumLive0")->SetText(szText);
			GetDlgItem("Lab_NumLive0")->SetAlign(AUIFRAME_ALIGN_CENTER);
			((PAUIPROGRESS)GetDlgItem("Pro_Live0"))->SetProgress(AUIPROGRESS_MAX * pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::ACUITY] / 100);
			a_sprintf(szText, _AL("%d/100"), pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::STAMINA]);
			GetDlgItem("Lab_NumLive1")->SetText(szText);
			GetDlgItem("Lab_NumLive1")->SetAlign(AUIFRAME_ALIGN_CENTER);
			((PAUIPROGRESS)GetDlgItem("Pro_Live1"))->SetProgress(AUIPROGRESS_MAX * pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::STAMINA] / 100);
			a_sprintf(szText, _AL("%d/100"), pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::SAWY]);
			GetDlgItem("Lab_NumLive2")->SetText(szText);
			GetDlgItem("Lab_NumLive2")->SetAlign(AUIFRAME_ALIGN_CENTER);
			((PAUIPROGRESS)GetDlgItem("Pro_Live2"))->SetProgress(AUIPROGRESS_MAX * pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::SAWY] / 100);
			a_sprintf(szText, _AL("%d/100"), pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::DOWRY]);
			GetDlgItem("Lab_NumLive3")->SetText(szText);
			GetDlgItem("Lab_NumLive3")->SetAlign(AUIFRAME_ALIGN_CENTER);
			((PAUIPROGRESS)GetDlgItem("Pro_Live3"))->SetProgress(AUIPROGRESS_MAX * pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::DOWRY] / 100);
			a_sprintf(szText, _AL("%d/100"), pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::WISDOM]);
			GetDlgItem("Lab_NumLive4")->SetText(szText);
			GetDlgItem("Lab_NumLive4")->SetAlign(AUIFRAME_ALIGN_CENTER);
			((PAUIPROGRESS)GetDlgItem("Pro_Live4"))->SetProgress(AUIPROGRESS_MAX * pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::WISDOM] / 100);
			a_sprintf(szText, _AL("%d/100"), pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::CNR]);
			GetDlgItem("Lab_NumLive5")->SetText(szText);
			GetDlgItem("Lab_NumLive5")->SetAlign(AUIFRAME_ALIGN_CENTER);
			((PAUIPROGRESS)GetDlgItem("Pro_Live5"))->SetProgress(AUIPROGRESS_MAX * pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::CNR] / 100);
		}
	}
}

void CDlgPetSetting::OnCommandConfirm(const char * szCommand)
{
	int nSlot = GetData();
	CECPetBedge *pPet = (CECPetBedge *)GetHostPlayer()->GetPetPack()->GetItem(nSlot);
	if( pPet )
	{
		if( stricmp(GetName(), "Win_PetProSetting") == 0 )
		{
			GetGameSession()->c2s_CmdPetSetRank(nSlot, pPet->GetTemplateID(), GetCheckedRadioButton(1));
			Show(false);
		}
		else if( stricmp(GetName(), "Win_PetStsSetting") == 0 )
		{
			if( GetCheckedRadioButton(1) == 0 )
			{
				int nCurPetIndex = GetHostPlayer()->GetBattlePetIndex();
				if( nCurPetIndex != nSlot )
				{
					CECIvtrItem *pItem;
					if( nCurPetIndex != -1 )
					{
						pItem = GetHostPlayer()->GetPetPack()->GetItem(nCurPetIndex);
						if( pItem )
							GetGameSession()->c2s_CmdPetSetStatus(nCurPetIndex, pItem->GetTemplateID(), 3, 0);
					}
				}
			}
			GetGameSession()->c2s_CmdPetSetStatus(nSlot, pPet->GetTemplateID(), GetCheckedRadioButton(1) / 10, GetCheckedRadioButton(1) % 10);
			Show(false);
		}
	}
}

int CDlgPetSetting::GetPetRankNumber(int rank, int index)
{
	int num = 0;
	for(int i = 0; i< GetHostPlayer()->GetPetPack()->GetSize(); i++)
	{
		CECPetBedge *pPet = (CECPetBedge *)GetHostPlayer()->GetPetPack()->GetItem(i);
		if( index != i && pPet && pPet->GetEssence().rank == rank )
			num++;
	}
	return num;
}
