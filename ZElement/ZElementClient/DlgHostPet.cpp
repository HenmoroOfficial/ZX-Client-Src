// File		: DlgHostPet.cpp
// Creator	: Xiao Zhou
// Date		: 2005/8/17

#include "AFI.h"
#include "DlgHostPet.h"
#include "DlgPetPackage.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_Game.h"
#include "EC_IvtrConsume.h"
#include "EC_Inventory.h"
#include "EC_NPC.h"
#include "EC_ManNPC.h"
#include "EC_World.h"
#include "ElementSkill.h"
#include "EC_Shortcut.h"

#include "AUI/AUICTranslate.h"
#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgHostPet, CDlgBase)

AUI_ON_COMMAND("Btn_Feed",		OnCommandAutoFood)
AUI_ON_COMMAND("Pet_Setting",	OnCommandPetSetting)
AUI_ON_COMMAND("combine1",		OnCommandCombine1)
AUI_ON_COMMAND("combine2",		OnCommandCombine2)
AUI_ON_COMMAND("withdraw",		OnCommandWithDraw)
AUI_ON_COMMAND("detail",		OnCommandDetail)
AUI_ON_COMMAND("close",			OnCommand_close)
AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgHostPet, CDlgBase)

AUI_ON_EVENT("Pic_HeadFrame",		WM_LBUTTONUP,		OnEventLButtonUp_Head)

AUI_END_EVENT_MAP()

int CDlgHostPet::m_nAutoFoodHp = 0;
int CDlgHostPet::m_nAutoFoodMp = 0;
int CDlgHostPet::m_nAutoFoodFull = 0;

DWORD CDlgHostPet::m_dwTickAutoFoodHp = 0;
DWORD CDlgHostPet::m_dwTickAutoFoodMp = 0;
DWORD CDlgHostPet::m_dwTickAutoFoodFull = 0;

CDlgHostPet::CDlgHostPet()
{
}

CDlgHostPet::~CDlgHostPet()
{
}

void CDlgHostPet::OnCommandAutoFood(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_PetAutoFood");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgHostPet::OnCommandPetSetting(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Pet_AttackSetting");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgHostPet::OnCommandCombine1(const char * szCommand)
{
	GetHostPlayer()->CmdPetCombine1();
}

void CDlgHostPet::OnCommandCombine2(const char * szCommand)
{
	GetHostPlayer()->CmdPetCombine2();
}

void CDlgHostPet::OnCommandWithDraw(const char * szCommand)
{
	GetHostPlayer()->CmdPetCall();
}

void CDlgHostPet::OnCommandDetail(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_PetDetail");
	pDlg->SetData(GetHostPlayer()->GetCurPetIndex());
	pDlg->Show(!pDlg->IsShow());
	if(pDlg->IsShow())
	{
		CECPetBedge *pPet = (CECPetBedge *)GetHostPlayer()->GetPetPack()->GetItem(GetHostPlayer()->GetCurPetIndex());
		if(pPet && pPet->GetEssence().reborn_cnt>0)
			GetGameSession()->c2s_CmdGetPetProps();
	}
}

void CDlgHostPet::OnCommandCancel(const char * szCommand)
{
	GetGameUIMan()->RespawnMessage();
}

void CDlgHostPet::OnCommand_close(const char * szCommand)
{
	if( stricmp(m_szName, "Win_PetHead") == 0 )
	{
		Show(false);
		GetGameUIMan()->GetDialog("Win_PetHeadClose")->Show(true);
		GetGameUIMan()->GetDialog("Win_PetHeadClose")->AlignTo(GetGameUIMan()->GetDialog("Win_CharHead"), AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
					AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
	}
	else
	{
		Show(false);
		GetGameUIMan()->GetDialog("Win_PetHead")->Show(true);
	}
}

bool CDlgHostPet::OnInitDialog()
{
	if( stricmp(m_szName, "Win_PetHead") == 0 )
	{
//		m_pBtn_Feed = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Feed");
		m_pPic_HeadFrame = (PAUIIMAGEPICTURE)GetDlgItem("Pic_HeadFrame");
		m_pImg_Hungry = (PAUIIMAGEPICTURE)GetDlgItem("Img_Hungry");
		m_pPrgs_Hp = (PAUIPROGRESS)GetDlgItem("Prgs_Hp");
		m_pPrgs_Mp = (PAUIPROGRESS)GetDlgItem("Prgs_Mp");
		m_pPrgs_Exp = (PAUIPROGRESS)GetDlgItem("Prgs_Exp");
		m_pTxt_Name = (PAUILABEL)GetDlgItem("Txt_Name");
		m_pTxt_Lv = (PAUILABEL)GetDlgItem("Txt_Lv");
		if (m_pTxt_Lv)
		{
			m_pTxt_Lv->SetAlign(AUIFRAME_ALIGN_CENTER);
		}
		
		m_dwTickAutoFoodHp = 0;
		m_dwTickAutoFoodMp = 0;
		m_dwTickAutoFoodFull = 0;
	}


	return true;
}

void CDlgHostPet::OnTick()
{
	if( m_szName == "Win_PetHead" && GetHostPlayer()->GetCurPetIndex() != -1 )
	{
		CECPetBedge *pPet = (CECPetBedge *)GetHostPlayer()->GetPetPack()->GetItem(GetHostPlayer()->GetCurPetIndex());
		if( pPet )
		{
			ACHAR szText[1024];
			AString strFile;
			af_GetFileTitle(GetGame()->GetDataPath(pPet->GetDBEssence()->file_head_icon[pPet->GetEssence().face]), strFile);
			strFile.MakeLower();
			m_pPic_HeadFrame->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_PET],
				GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_PET][strFile]);
			m_pTxt_Name->SetText(pPet->GetName());
			a_sprintf(szText, _AL("%d"), pPet->GetEssence().level);
			m_pTxt_Lv->SetText(szText);
			a_sprintf(szText, _AL("%d/%d"), pPet->GetEssence().cur_hp, GetHostPlayer()->GetCurPetMaxHp());
			m_pPrgs_Hp->SetHint(szText);
			if( GetHostPlayer()->GetCurPetMaxHp() > 0 )
				m_pPrgs_Hp->SetProgress(int(AUIPROGRESS_MAX * ((double)pPet->GetEssence().cur_hp / GetHostPlayer()->GetCurPetMaxHp())));
			a_sprintf(szText, _AL("%d/%d"), pPet->GetEssence().cur_vigor, GetHostPlayer()->GetCurPetMaxVp());
			m_pPrgs_Mp->SetHint(szText);
			if( GetHostPlayer()->GetCurPetMaxVp() > 0 )
				m_pPrgs_Mp->SetProgress(int(AUIPROGRESS_MAX * ((double)pPet->GetEssence().cur_vigor / GetHostPlayer()->GetCurPetMaxVp())));
			a_sprintf(szText, _AL("%d/%d"), pPet->GetEssence().exp, (int)CECPlayer::GetPetLevelUpExp(pPet->GetEssence().level + 1));
			m_pPrgs_Exp->SetHint(szText);
			if( CECPlayer::GetPetLevelUpExp(pPet->GetEssence().level + 1) > 0 )
				m_pPrgs_Exp->SetProgress(int(AUIPROGRESS_MAX * ((double)pPet->GetEssence().exp / CECPlayer::GetPetLevelUpExp(pPet->GetEssence().level + 1))));
			float hungry = float(pPet->GetEssence().cur_hunger_point) / pPet->GetEssence().max_hunger_point;
			if( hungry >= 0.75 )
			{
				m_pImg_Hungry->FixFrame(3);
				a_sprintf(szText, GetStringFromTable(9121), GetStringFromTable(4403));
				m_pImg_Hungry->SetHint(szText);
			}
			else if( hungry >= 0.5 )
			{
				m_pImg_Hungry->FixFrame(2);
				a_sprintf(szText, GetStringFromTable(9121), GetStringFromTable(4402));
				m_pImg_Hungry->SetHint(szText);
			}
			else if( hungry >= 0.25 )
			{
				m_pImg_Hungry->FixFrame(1);
				a_sprintf(szText, GetStringFromTable(9121), GetStringFromTable(4401));
				m_pImg_Hungry->SetHint(szText);
			}
			else
			{
				m_pImg_Hungry->FixFrame(0);
				a_sprintf(szText, GetStringFromTable(9121), GetStringFromTable(4400));
				m_pImg_Hungry->SetHint(szText);
			}
			//m_pBtn_Feed->SetHint(szText);
			CECNPCMan *pNPCMan = GetWorld()->GetNPCMan();
			CECNPC *pPetNPC = pNPCMan->GetNPC(GetHostPlayer()->GetCurPetId());
			if( pPetNPC )
			{
				for(int i = 0; ; i++ )
				{
					char szName[20];
					sprintf(szName, "Img_Buff%d", i + 1);
					PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem(szName);
					PAUILABEL pTxt = (PAUILABEL)GetDlgItem(szName);
					if( !pImg )
						break;
					if( i < pPetNPC->GetIconStateNum() )
					{
						const GNET::TeamState *pTS = GNET::TeamState::Query(pPetNPC->GetIconState(i).id);
						if( pTS )
							af_GetFileTitle(pTS->GetIcon(), strFile);
						else
							strFile = "";
						
						strFile.MakeLower();
						pImg->SetCover(
							GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_STATE],
							GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_STATE][strFile]);
						
						if( !pTS )
							pImg->SetHint(_AL(""));
						else
						{
							const ACHAR * szDesc = GetGame()->GetBuffDesc(pTS->GetID());
							AUICTranslate trans;
							if( szDesc )
								pImg->SetHint(trans.Translate(szDesc));
							else
								pImg->SetHint(trans.Translate(pTS->GetName()));
						}
						pTxt->SetText(_AL(""));
						pImg->Show(true);
					}
					else
						pImg->Show(false);
				}
			}
		}
	}
}

void CDlgHostPet::OnEventLButtonUp_Head(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( GetHostPlayer()->GetCurPetId() )
		GetGameSession()->c2s_CmdSelectTarget(GetHostPlayer()->GetCurPetId());
}

void CDlgHostPet::AutoFeed()
{
	CECInventory* pPetPack = GetHostPlayer()->GetPetPack();
	CECPetBedge* pPet = (CECPetBedge*)pPetPack->GetItem(GetHostPlayer()->GetCurPetIndex());
	if( !pPet )
		return;
	CECShortcut *pSC = (CECShortcut*)GetGameUIMan()->m_pDlgPetPackage->GetDlgItem("Img_AutoFood")->GetDataPtr("ptr_CECShortcut");
	if( pSC )
	{
		DWORD dwTick = GetTickCount();
		int nMask = 0;
		if( dwTick - m_dwTickAutoFoodHp > 500 && pPet->GetEssence().cur_hp * 100 / GetHostPlayer()->GetCurPetMaxHp() < m_nAutoFoodHp )
		{
			m_dwTickAutoFoodHp = dwTick;
			nMask |= 0x1;
		}
		if( dwTick - m_dwTickAutoFoodMp > 500 && pPet->GetEssence().cur_vigor * 100 / GetHostPlayer()->GetCurPetMaxVp() < m_nAutoFoodMp )
		{
			m_dwTickAutoFoodMp = dwTick;
			nMask |= 0x2;
		}
		if( dwTick - m_dwTickAutoFoodFull > 500 && pPet->GetEssence().cur_hunger_point < m_nAutoFoodFull )
		{
			m_dwTickAutoFoodFull = dwTick;
			nMask |= 0x4;
		}
		if( nMask )
		{
			CECSCItem *pSCItem = (CECSCItem*)pSC;
			struct auto_food_data
			{
				int index;
				int mask;
			};
			auto_food_data autofood;
			autofood.index = GetHostPlayer()->GetCurPetIndex();
			autofood.mask = nMask;
			GetGameSession()->c2s_CmdUseItemWithArg(IVTRTYPE_PACK, 1, (unsigned short)pSCItem->GetIvtrSlot(),
				pSCItem->GetItemTID(), &autofood, sizeof(auto_food_data));
		}
	}

}
