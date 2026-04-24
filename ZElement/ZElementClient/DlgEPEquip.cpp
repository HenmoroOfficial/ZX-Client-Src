// File		: DlgEPEquip.cpp
// Creator	: Xiao Zhou
// Date		: 2005/12/15

#include "AFI.h"
#include "DlgEPEquip.h"
#include "DlgSkill.h"
#include "DlgSkillTree.h"
#include "DlgItemLock.h"
#include "DlgFriendSeeking.h"
#include "DlgSnsInfo.h"
#include "DlgSkillMan.h"
#include "AUI\\AUICTranslate.h"
#include "AUI\\AUIRadioButton.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrEquip.h"
#include "EC_IvtrTalismanMain.h"
#include "EC_IvtrRune.h"
#include "EC_Faction.h"
#include "EC_Player.h"
#include "EC_ManPlayer.h"
#include "EC_World.h"
#include "EC_Game.h"
#include "EC_LoginPlayer.h"
#include "EC_Global.h"
#include "EC_GameRun.h"
#include "EC_Game.h"
#include "EC_Model.h"
#include "EC_Friend.h"
#include "EC_GameSession.h"
#include "EC_IvtrConsume.h"
#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgEPEquip, CDlgBase)

AUI_ON_COMMAND("wear",				OnCommand_Fashion)
AUI_ON_COMMAND("normal",			OnCommand_Fashion)
AUI_ON_COMMAND("showequip",			OnCommand_ShowEquip)
AUI_ON_COMMAND("info",				OnCommand_Information)
AUI_ON_COMMAND("IDCANCEL",			OnCommand_CANCEL)

AUI_END_COMMAND_MAP()

CDlgEPEquip::CDlgEPEquip()
{
	m_pPlayer = NULL;
}

CDlgEPEquip::~CDlgEPEquip()
{
	A3DRELEASE(m_pPlayer);
}

void CDlgEPEquip::OnCommand_Fashion(const char * szCommand)
{
	if( m_pPlayer )
		m_pPlayer->SetFashionMode(((PAUIRADIOBUTTON)GetDlgItem("Rdo_FashDress"))->IsChecked() );
}

void CDlgEPEquip::OnCommand_ShowEquip(const char * szCommand)
{
	bool bCheck = m_pChk_ShowEquip->IsChecked();
	int i;
	for( i = 0; i < SIZE_EQUIPIVTR; i++ )
		if( m_pImgEquip[i] )
			m_pImgEquip[i]->Show(!bCheck);
}

void CDlgEPEquip::OnCommand_Information(const char * szCommand)
{
	if (m_nID<=0)
	{
		return;
	}
	CDlgFriendSeek::MsgID2PlayerInfoMap::iterator iter = CDlgFriendSeek::m_id2PlayerInfoCache.find(m_nID);
	if (iter != CDlgFriendSeek::m_id2PlayerInfoCache.end())
	{
		CDlgFriendSeek::PlayerInfoCache playerInfoCache = CDlgFriendSeek::m_id2PlayerInfoCache[m_nID];
		CECFriendMan *pMan = GetHostPlayer()->GetFriendMan();
		if (!playerInfoCache.playerinfo.is_private || pMan->GetFriendByID(m_nID))
		{
			Show(false);
			CDlgSnsInfo *pDlgSnsInfo = (CDlgSnsInfo *)GetGameUIMan()->GetDialog("Win_PersInfoCheck");
			pDlgSnsInfo->SetInfo(playerInfoCache, true);
		}
		else
		{
			GetGameUIMan()->MessageBox("",GetStringFromTable(8177), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		}
	}
	else
	{
		GetGameSession()->sns_get_playerinfo(1,m_nID);
	}
}

void CDlgEPEquip::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
}

bool CDlgEPEquip::OnInitDialog()
{
	char szItem[20];
	int i;
	for( i = 0; i < SIZE_EQUIPIVTR; i++ )
	{
		sprintf(szItem, "Equip_%02d", i);
		m_pImgEquip[i] = (PAUIIMAGEPICTURE)GetDlgItem(szItem);
	}
	

	m_pImgPet = (PAUIIMAGEPICTURE)GetDlgItem("Equip_Pet");
	m_pTxt_Name = (PAUILABEL)GetDlgItem("Txt_Name");
	m_pChk_ShowEquip = (PAUICHECKBOX)GetDlgItem("Chk_ShowEquip");
	m_pTxt_Name->SetAlign(AUIFRAME_ALIGN_CENTER);

	m_pImg_Char = (PAUIIMAGEPICTURE)GetDlgItem("Pic_Part");
	m_pBtn_LeftTurn = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_LeftTurn");
	m_pBtn_RightTurn = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_RightTurn");

	return true;
}

void CDlgEPEquip::OnShowDialog()
{
}

void CDlgEPEquip::UpdateEquip(int idPlayer)
{
	if( GetGameUIMan()->m_bReplayMode )
		return;

	CECPlayer *pPlayer = GetWorld()->GetPlayerMan()->GetPlayer(idPlayer);
	if( !pPlayer )
	{
		m_nID = 0;
		return;
	}
	m_nID = idPlayer;

	SetData(idPlayer);
	Show(true);

	ACHAR szText[40];
	CECInventory *pInventory = GetHostPlayer()->GetEPEquipPack();
	CECIvtrItem *pItem;
	PAUIIMAGEPICTURE pObj;
	CECIvtrEquip *pEquip;
	int i;
	bool bSpecEquip;

	m_pTxt_Name->SetText(pPlayer->GetName());

	int equips[SIZE_EQUIPIVTR];

	for( i = 0; i < SIZE_EQUIPIVTR; i++ )
	{
		bSpecEquip = false;
		pObj = m_pImgEquip[i];
		if( !pObj )
			continue;

		pItem = pInventory->GetItem(i);
		if( pItem )
		{
			pEquip = (CECIvtrEquip *)pItem;
			equips[i] = pItem->GetTemplateID();
			if( i >= EQUIPIVTR_FASHION_EYE && i <= EQUIPIVTR_FASHION_BACK 
				|| i == EQUIPIVTR_FASHION_WEAPON)
			{
				CECIvtrEquip * pEquip = (CECIvtrEquip *) pItem;
				if(pEquip->GetNewEssenceForAstrology().reserved1 != 0)
				{
					CECIvtrIndividualityItem* pItem = 	(CECIvtrIndividualityItem*)CECIvtrItem::CreateItem(pEquip->GetNewEssenceForAstrology().reserved1, 0, 1);
					if(pItem && pItem->GetClassID() == CECIvtrItem::ICID_INDIVIDUALITY_ITEM)
					{
						WORD nColorId = pItem->GetDBEssence()->index;
						equips[i] |= (nColorId << 16) & 0x7fffffff;
						delete pItem;
					}
				}
			}
			if( pItem->GetCount() > 1 )
			{
				a_sprintf(szText, _AL("%d"), pItem->GetCount());
				pObj->SetText(szText);
			}
			else
				pObj->SetText(_AL(""));

			AString strFile;
			af_GetFileTitle(pItem->GetIconFile(), strFile);
			strFile.MakeLower();
			pObj->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
				GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
			pObj->SetDataPtr(pItem,"ptr_CECIvtrItem");
			
			WORD num_estone = pEquip->GetEssence().num_estone;

			if (pItem->IsLocked())
			{
				pObj->SetCover(CDlgItemLock::m_pA2DSpriteLock, 0, 1);
			}
			if (pItem->GetClassID() == CECIvtrItem::ICID_EQUIP && num_estone >= 9)
			{
				if (pItem->IsLocked())
				{
					pObj->SetCover(CDlgSkill::m_pA2DSpriteSkillSpecEquipLock, (GetTickCount() / 200) % 6, 1);
				}
				else
				{
					pObj->SetCover(CDlgSkill::m_pA2DSpriteSkillSpecEquip, (GetTickCount() / 200) % 6, 1);
				}
			}
		}
		else
		{
			equips[i] = 0;
			pObj->ClearCover();
			pObj->SetText(_AL(""));
			pObj->SetDataPtr(NULL);
		}
	}

	pItem = NULL;
	for(int j = 0; j < IVTRSIZE_PETPACK; j++)
	{
		pItem = GetHostPlayer()->GetEPPetPack()->GetItem(j);
		if( pItem )
			break;
	}
	if( pItem )
	{
		AString strFile;
		af_GetFileTitle(pItem->GetIconFile(), strFile);
		strFile.MakeLower();
		m_pImgPet->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
			GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
		m_pImgPet->SetDataPtr(pItem,"ptr_CECIvtrItem");
	}
	else
	{
		m_pImgPet->SetDataPtr(NULL);
		m_pImgPet->ClearCover();
		m_pImgPet->SetHint(GetStringFromTable(309));
	}

	A3DRELEASE(m_pPlayer);
	m_pPlayer = new CECLoginPlayer(g_pGame->GetGameRun()->GetWorld()->GetPlayerMan());
	m_nAngle = 0;

	m_pPlayer->GetBasicProps() = pPlayer->GetBasicProps();
	if (!m_pPlayer->Load(pPlayer->GetPhysique(), pPlayer->GetProfession(), pPlayer->GetFaceid(), pPlayer->GetHairid(), pPlayer->GetGender(), equips))
	{
		m_pPlayer->Release();
		delete m_pPlayer;
		m_pPlayer = 0;
	}
	else
	{
		if( pPlayer->InFashionMode() )
		{
			m_pPlayer->SetFashionMode(true);
			CheckRadioButton(1, 2);
		}
		else
		{
			m_pPlayer->SetFashionMode(false);
			CheckRadioButton(1, 1);
		}
		m_pPlayer->SetPos(A3DVECTOR3(0));
		m_pPlayer->SetDirAndUp(A3DVECTOR3(0, 0, -1.0f), A3DVECTOR3(0, 1.0f, 0));
		m_pPlayer->GetPlayerModel()->SetAutoUpdateFlag(false);
		m_pPlayer->SetFashionMode(pPlayer->InFashionMode());
	}
	GetGameUIMan()->GetDialog("Win_PetEquip")->SetData(idPlayer);
	GetGameUIMan()->GetDialog("Win_PetEquip")->Show(true);
	GetGameUIMan()->CommonMessage("Win_Fengshen_Soul_Check", "init", pPlayer->GetCultivation(), pPlayer->GetBasicProps().iDeityLevel);
	if( CDlgSkillTree::m_pSkillTreeActive )
		CDlgSkillTree::m_pSkillTreeActive->Show(false);
	if( CDlgSkillTree::m_pTalentTreeActive )
		CDlgSkillTree::m_pTalentTreeActive->Show(false);
	if( CDlgSkillTree::m_pFortuneActive )
		CDlgSkillTree::m_pFortuneActive->Show(false);
	
	CDlgSkillMan *pSkillMan = (CDlgSkillMan*)GetGameUIMan()->GetDialog("Win_Skillzx2");
	pSkillMan->SetData(idPlayer);
	pSkillMan->Show(true);
//	GetGameUIMan()->m_pDlgSkill->OnCommandSkillTree("");
	Show(true);
}

void CDlgEPEquip::UpdateIcon()
{
	if( GetGameUIMan()->m_bReplayMode )
		return;

	CECInventory *pInventory = GetHostPlayer()->GetEPEquipPack();
	CECIvtrItem *pItem;
	PAUIIMAGEPICTURE pObj;
	CECIvtrEquip *pEquip;
	int i;
	bool bSpecEquip;

	for( i = 0; i < SIZE_EQUIPIVTR; i++ )
	{
		bSpecEquip = false;
		pObj = m_pImgEquip[i];
		if( !pObj )
			continue;

		pItem = pInventory->GetItem(i);
		if( pItem )
		{
			if (pItem->IsLocked())
			{
				pObj->SetCover(CDlgItemLock::m_pA2DSpriteLock, 0, 1);
			}
			
			if( ( pItem->GetClassID()==CECIvtrItem::ICID_EQUIP && ((CECIvtrEquip*)pItem)->GetEssence().num_estone>=9 ) ||
				( pItem->GetClassID()==CECIvtrItem::ICID_TALISMAN_MAINPART && ((CECIvtrTalismanMain*)pItem)->GetRebornCount()>0 ) ||
				( pItem->GetClassID()==CECIvtrItem::ICID_RUNE && ((CECIvtrRune*)pItem)->GetEssence().quality >= 3) )
			{
				if (pItem->IsLocked())
				{
					pObj->SetCover(CDlgSkill::m_pA2DSpriteSkillSpecEquipLock, (GetTickCount() / 200) % 6, 1);
				}
				else
				{
					pObj->SetCover(CDlgSkill::m_pA2DSpriteSkillSpecEquip, (GetTickCount() / 200) % 6, 1);
				}
			}
		}
		else
		{
			pObj->ClearCover();
			pObj->SetText(_AL(""));
			pObj->SetDataPtr(NULL);
		}
	}
}

bool CDlgEPEquip::Tick(void)
{
	UpdateIcon();
	if (m_pPlayer)
	{
		if( m_pBtn_LeftTurn->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
			m_nAngle = (m_nAngle + 360 - GetGame()->GetTickTime() / 6 ) % 360;
		else if( m_pBtn_RightTurn->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
			m_nAngle = (m_nAngle + GetGame()->GetTickTime() / 6) % 360;
		m_pPlayer->Stand();
		m_pPlayer->Tick(g_pGame->GetTickTime());

		m_pImg_Char->SetRenderCallback(
			PlayerRenderDemonstration,
			(DWORD)m_pPlayer,
			m_nAngle);
	}

	return CDlgBase::Tick();
}
