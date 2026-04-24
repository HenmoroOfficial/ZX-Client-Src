/*
 * FILE: EC_Shortcut.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/1/5
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_Shortcut.h"
#include "EC_IvtrItem.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_GameSession.h"
#include "EC_IvtrTypes.h"
#include "EC_IvtrTalismanMain.h"
#include "EC_Resource.h"
#include "EC_Skill.h"
#include "EC_PetCorral.h"
#include "EC_FixedMsg.h"
#include "AUI\\AUIDialog.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "DlgTextFirework.h"
#include "DlgTreasureRadar.h"
#include "elementdataman.h"
#include "EC_IvtrConsume.h"
#include "EC_IvtrTaskItem.h"
#include "EC_Configs.h"
#include "EC_Player.h"
#include "EC_NPC.h"
#include "EC_ManNPC.h"
#include "EC_Monster.h"
#include "EC_World.h"
#include "EC_ManPlayer.h"
#include "EC_ElsePlayer.h"
#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Reference to External variables and functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local Types and Variables and Global variables
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECShortcut
//	
///////////////////////////////////////////////////////////////////////////

CECShortcut::CECShortcut(const CECShortcut& src)
{
	m_iSCType = src.m_iSCType;
}

//	Clone shortcut
CECShortcut* CECShortcut::Clone()
{
	return new CECShortcut(*this);
}

//	Get icon file
const char* CECShortcut::GetIconFile()
{
	//	Return a default icon file name
	static const char szFile[] = "Iconset\\铲.bmp";
	return szFile;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECSCSkill
//	
///////////////////////////////////////////////////////////////////////////

CECSCSkill::CECSCSkill()
{
	m_iSCType	= SCT_SKILL;
	m_pSkill	= NULL;
}

CECSCSkill::CECSCSkill(const CECSCSkill& src) : CECShortcut(src)
{
	if (src.GetSkill() && src.GetSkill()->IsCategory(GNET::CATEGORY_TRIGGERED))
		m_pSkill = new CECSkill(src.GetSkill()->GetSkillID(), src.GetSkill()->GetSkillLevel());
	else
		m_pSkill = src.m_pSkill;
}

CECSCSkill::~CECSCSkill()
{
	if (m_pSkill && m_pSkill->IsCategory(GNET::CATEGORY_TRIGGERED))
	{
		delete m_pSkill;
	}
}

//	Clone shortcut
CECShortcut* CECSCSkill::Clone()
{
	return new CECSCSkill(*this);
}

//	Initialize object
bool CECSCSkill::Init(CECSkill* pSkill)
{
	if (pSkill && pSkill->IsCategory(GNET::CATEGORY_TRIGGERED))
		m_pSkill = new CECSkill(pSkill->GetSkillID(), pSkill->GetSkillLevel());
	else
		m_pSkill = pSkill;
	return true;
}

//	Execute shortcut
bool CECSCSkill::Execute(unsigned int keycode)
{
	if (!m_pSkill)
		return false;
	
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	ASSERT(pHost);

	if (pHost->ApplySkillShortcut(m_pSkill->GetSkillID()))
		pHost->SetCurSkillKeyCode(keycode);

	return true;
}

bool CECSCSkill::OnKeyUp()
{
	if (!m_pSkill || !m_pSkill->IsChargeable())
		return false;
	
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	ASSERT(pHost);

	pHost->ApplySkillShortcutUp(m_pSkill->GetSkillID());
	return true;
}

//	Get icon file
const char* CECSCSkill::GetIconFile()
{
	return m_pSkill ? m_pSkill->GetIconFile() : "";
}

//	Get shortcut description text
const wchar_t* CECSCSkill::GetDesc()
{
	if (m_pSkill && m_pSkill->IsCategory(GNET::CATEGORY_TRIGGERED))
	{
		CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
		CECSkill* pHostSkill = pHost->GetSkill(m_pSkill->GetSkillID());
		if (pHostSkill)
		{
			m_pSkill->SetLevel(pHostSkill->GetSkillLevel());
		}
	}
	return m_pSkill ? m_pSkill->GetDesc() : _AL("");
}

//	Get item cool time
int CECSCSkill::GetCoolTime(int* piMax/* NULL */)
{
	if (!m_pSkill)
		return 0;

	if (piMax)
		*piMax = m_pSkill->GetCoolingTime();

	return m_pSkill->GetCoolingCnt();
}

void CECSCSkill::SetSkill(CECSkill* pSkill) 
{
	if (m_pSkill && m_pSkill->IsCategory(GNET::CATEGORY_TRIGGERED))
	{
		delete m_pSkill;
	}
	Init(pSkill);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECSCItem
//	
///////////////////////////////////////////////////////////////////////////

CECSCItem::CECSCItem()
{
	m_iSCType		= SCT_ITEM;
	m_iIvtr			= 0;
	m_iSlot			= 0;
	m_tidItem		= 0;
	m_bAutoFind		= false;
}

CECSCItem::CECSCItem(const CECSCItem& src) : CECShortcut(src)
{
	m_iIvtr			= src.m_iIvtr;
	m_iSlot			= src.m_iSlot;
	m_tidItem		= src.m_tidItem;
	m_strIconFile	= src.m_strIconFile;
	m_bAutoFind		= src.m_bAutoFind;
}

CECSCItem::~CECSCItem()
{
}

//	Clone shortcut
CECShortcut* CECSCItem::Clone()
{
	return new CECSCItem(*this);
}

//	Initialize object
bool CECSCItem::Init(int iIvtr, int iSlot, CECIvtrItem* pItem)
{
	m_iIvtr	= iIvtr;
	m_iSlot	= iSlot;

	if (!pItem->IsEquipment())
		m_bAutoFind = true;

	m_tidItem		= pItem->GetTemplateID();
	m_strIconFile	= pItem->GetIconFile();

	return true;
}

//	Get item object
CECIvtrItem* CECSCItem::GetItem()
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (!pHost)
	{
		ASSERT(pHost);
		return NULL;
	}

	//	Try to get item at m_iSlot at first
	CECInventory* pPack = pHost->GetPack(m_iIvtr);
	if (!pPack)
	{
		ASSERT(pPack);
		return NULL;
	}

	return pPack->GetItem(m_iSlot);
}

//	Execute shortcut
bool CECSCItem::Execute(unsigned int keycode)
{
	if (!m_tidItem)
	{
		ASSERT(0);
		return false;
	}

	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	ASSERT(pHost);

	CECIvtrItem* pItem = GetItem();
	if( pItem && pItem->GetClassID() == CECIvtrItem::ICID_TEXT_FIREWORK )
	{
		CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		CDlgTextFirework *pDlg = (CDlgTextFirework *)pGameUI->GetDialog("Win_TextFirework");
		pDlg->SetFirework(m_iSlot);
		pDlg->Show(true);
		return true;
	}
	if( pItem && pItem->GetClassID() == CECIvtrItem::ICID_LOTTERY )
	{
		CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		CECIvtrLottery *pLottery = (CECIvtrLottery*)pItem;
		PAUIDIALOG pDlg;
		if ( 0 == pLottery->GetDBEssence()->type )
		{
			pDlg = pGameUI->GetDialog("Win_Lottery");
		}
		else
		{
			pDlg = pGameUI->GetDialog("Win_Lottery2");
		}
		if( pDlg->GetData() != 9999 )
		{
			CECIvtrItem *pItem = pHost->GetPack()->GetItem(pDlg->GetData());
			if( pItem )
				pItem->Freeze(false);
		}
		pDlg->SetData(m_iSlot);
		pDlg->Show(true);
		return true;
	}
	else if( pItem && (pItem->GetClassID() == CECIvtrItem::ICID_LOTTERY2 || pItem->GetClassID() == CECIvtrItem::ICID_LOTTERY3) )
	{
		CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		PAUIDIALOG pDlg;
		if ( pItem->GetClassID() == CECIvtrItem::ICID_LOTTERY2 )
		{
			CECIvtrLottery2 *pLottery = (CECIvtrLottery2*)pItem;
			if (pLottery->GetDBEssence()->type==1)
				pDlg = pGameUI->GetDialog("Win_Lottery2");
			else
				pDlg = pGameUI->GetDialog("Win_Lottery3");
		}
		else
		{
			CECIvtrLottery3 *pLottery = (CECIvtrLottery3*)pItem;
			if (pLottery->GetDBEssence()->type==1)
				pDlg = pGameUI->GetDialog("Win_Lottery2");
			else
				pDlg = pGameUI->GetDialog("Win_Lottery3");
			}
		if( pDlg->GetData() != 9999 )
		{
			CECIvtrItem *pItem = pHost->GetPack()->GetItem(pDlg->GetData());
			if( pItem )
				pItem->Freeze(false);
		}
		pDlg->SetData(m_iSlot);
		pDlg->Show(true);
	}
	else if ( pItem && pItem->GetClassID()==CECIvtrItem::ICID_LOTTERY_TANGYUAN)
	{
		CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		PAUIDIALOG pDlg = pGameUI->GetDialog("Win_Lottery5");
		if( pDlg->GetData() != 9999 )
		{
			CECIvtrItem *pItem = pHost->GetPack()->GetItem(pDlg->GetData());
			if( pItem )
				pItem->Freeze(false);
		}
		pDlg->SetData(m_iSlot); 
		pDlg->Show(true);
	}
	if( pItem && pItem->GetClassID() == CECIvtrItem::ICID_BOOK )
	{
		CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		PAUIDIALOG pDlg = pGameUI->GetDialog("Win_Book");
		if( pDlg->GetData() != 9999 )
		{
			CECIvtrItem *pItem = pHost->GetPack()->GetItem(pDlg->GetData());
			if( pItem )
				pItem->Freeze(false);
		}
		pDlg->SetData(m_iSlot);
		pDlg->Show(true);
		return true;
	}
	if( pItem && !pItem->IsEquipment() && (pItem->GetStateFlag() & ITEM_STATE_BINDAFTEREQUIP) &&
		!(pItem->GetStateFlag() & ITEM_STATE_BINDED) )
	{
		const SPECIAL_ID_CONFIG& cfg = g_pGame->GetGameRun()->GetSpecialIDConfig();
		if (pItem->GetTemplateID() != cfg.id_destroying_matter)
		{
			PAUIDIALOG pMsgBox;
			CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			pGameUI->MessageBox("Game_ItemBind", pGameUI->GetStringFromTable(922), 
				MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
			pMsgBox->SetData(m_iSlot);
		}
		return true;
	}
	if(pItem && pItem->GetClassID() == CECIvtrItem::ICID_TASKDICE)
	{
		CDlgTreasureRadar* pTreasureRadar = (CDlgTreasureRadar*)(g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_DragonCampass"));
		int type = ((CECIvtrTaskDice*)pItem)->GetDBEssence()->type;
		if(type == 1)
		{
		    pTreasureRadar->UseTaskRectItem(pItem,m_iSlot);
		}
		else if(type == 2 || type == 3)
		{
			if(type == 3) pTreasureRadar = (CDlgTreasureRadar*)(g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_ZongziCampass"));
			
			if(pItem->IsFrozen())
			{
				pTreasureRadar->CheckRadarState(pItem,m_iSlot);
			}
			else
			{
				pTreasureRadar->UpdateRadarState(pItem,m_iSlot);
			}
		}
		else
		{
		    pHost->ApplyItemShortcut(m_iIvtr, m_iSlot);
		}
		
		return true;
	}
	if (!m_bAutoFind)
	{
		if( pItem && pItem->IsEquipment() && pHost->CanUseEquipment(((CECIvtrEquip*)pItem)) )
		{
			if( ((CECIvtrEquip*)pItem)->IsTalismanMain() && 
				((CECIvtrTalismanMain*)pItem)->GetEssence().ess.master_id == 0 )
			{
				PAUIDIALOG pMsgBox;
				CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
				pGameUI->MessageBox("Game_EquipBind2", pGameUI->GetStringFromTable(873), 
					MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
				pMsgBox->SetData(m_iSlot);
			}
			else if( pItem->GetStateFlag() & ITEM_STATE_BINDAFTEREQUIP )
			{
				PAUIDIALOG pMsgBox;
				CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
				pGameUI->MessageBox("Game_EquipBind2", pGameUI->GetStringFromTable(811), 
					MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
				pMsgBox->SetData(m_iSlot);
			}
			else
				pHost->ApplyItemShortcut(m_iIvtr, m_iSlot);
		}
		else
			pHost->ApplyItemShortcut(m_iIvtr, m_iSlot);
		return true;
	}

	//	Try to get item at m_iSlot at first
	if (!pItem || pItem->GetTemplateID() != m_tidItem)
	{
		CECInventory* pPack = pHost->GetPack(m_iIvtr);

		int i, iCandidate = -1;

		//	Are there some same kind items exists ? Some items with different
		//	id but have the same effect. For example, when we counldn't find
		//	middle heal bottle, we can use small or large heal bottles as candidates.
		for (i=0; i < pPack->GetSize(); i++)
		{
			CECIvtrItem* pTempItem = pPack->GetItem(i);
			if (!pTempItem)
				continue;

			int idSlotItem = pTempItem->GetTemplateID();
			if (idSlotItem == m_tidItem)
			{
				m_iSlot = i;
				break;
			}
			else if (iCandidate < 0 && CECIvtrItem::IsCandidate(m_tidItem, pTempItem))
				iCandidate = i;
		}

		if (i >= pPack->GetSize())
		{
			if (iCandidate >= 0)
			{
				m_iSlot = iCandidate;
				UpdateItemData();
			}
			else
			{
				//	Couldn't find proper item, this shortcut should has been removed
				ASSERT(0);
				return false;
			}
		}
	}

	if (CECInventory* pInv = pHost->GetPack(m_iIvtr))
	{
		CECIvtrItem* pItem = pInv->GetItem(m_iSlot);
		if (pItem && pItem->GetClassID() == CECIvtrItem::ICID_PROP_ADD_ITEM)
		{
			AUIDialog* pDlg = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_AttriUp");
			if (!pDlg->IsShow())
				pDlg->Show(true);

			g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->CommonMessage("Win_AttriUp", "set_propadd_item", m_iSlot, (long)pItem);
			return true;
		}
	}
	pHost->ApplyItemShortcut(m_iIvtr, m_iSlot);

	return true;
}

//	Set new position of item referenced by this shortcut
void CECSCItem::MoveItem(int iIvtr, int iSlot)
{
	m_iIvtr = iIvtr;
	m_iSlot = iSlot;

	//	Update item associated data
	UpdateItemData();
}

//	Update item associated data after m_iSlot changed
void CECSCItem::UpdateItemData()
{
	//	Update shortcut icon file if necessary
	CECIvtrItem* pItem = GetItem();
	if (pItem)
	{
		//	If item id changes
		if (pItem->GetTemplateID() != m_tidItem)
		{
			m_tidItem		= pItem->GetTemplateID();
			m_strIconFile	= pItem->GetIconFile();
		}
	}
	else
	{
		m_tidItem = 0;
		m_strIconFile.Empty();
	}
}

//	Get shortcut description text
const wchar_t* CECSCItem::GetDesc()
{
	CECIvtrItem* pItem = GetItem();
	if (!pItem)
		return _AL("");

	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	const wchar_t * pDesc = pItem->GetDesc(CECIvtrItem::DESC_NORMAL, pHost->GetEquipment());
	if (!pDesc)
	{
		pHost->GetIvtrItemDetailData(m_iIvtr, m_iSlot);
		return _AL("");
	}

	return pDesc;
}

//	Get item cool time
int CECSCItem::GetCoolTime(int* piMax/* NULL */)
{
	CECIvtrItem* pItem = GetItem();
	return pItem ? pItem->GetCoolTime(piMax) : 0;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECSCCommand
//	
///////////////////////////////////////////////////////////////////////////

CECSCCommand::CECSCCommand(int iCommand)
{
	m_iSCType	= SCT_COMMAND;
	m_iCommand	= iCommand;
	m_dwParam	= 0;
}

CECSCCommand::CECSCCommand(const CECSCCommand& src) : CECShortcut(src)
{
	m_iCommand	= src.m_iCommand;
	m_dwParam	= src.m_dwParam;
}

CECSCCommand::~CECSCCommand()
{
}

//	Clone shortcut
CECShortcut* CECSCCommand::Clone()
{
	return new CECSCCommand(*this);
}

//	Execute shortcut
bool CECSCCommand::Execute(unsigned int keycode)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (!pHost || !pHost->IsAllResReady())
		return false;

	if (GetCoolTime())
	{
		bool bForbidCmd = true;
		if (m_iCommand == CMD_RUSHFLY && pHost->GetRushFlyFlag())
			bForbidCmd = false;

		if ((m_iCommand == CMD_PET_COMBINE1 || m_iCommand == CMD_PET_COMBINE2) && pHost->GetPetCombinedType() != -1)
			bForbidCmd = false;

		if (bForbidCmd)
		{
			if (m_iCommand != CMD_SELLBOOTH)
				g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_CMD_INCOOLTIME);
			return false;
		}
	}

	switch (m_iCommand)
	{
	case CMD_SITDOWN:			pHost->CmdSitDown(!pHost->IsSitting());			break;
	case CMD_WALKRUN:			pHost->CmdWalkRun(!pHost->GetWalkRunFlag());	break;
	case CMD_NORMALATTACK:		pHost->CmdNormalAttack();						break;
	case CMD_FINDTARGET:		pHost->CmdFindTarget();							break;
	case CMD_ASSISTATTACK:		pHost->CmdAssistAttack();						break;
	case CMD_INVITETOTEAM:		pHost->CmdInviteToTeam();						break;
	case CMD_LEAVETEAM:			pHost->CmdLeaveTeam();							break;
	case CMD_KICKTEAMMEM:		pHost->CmdKickTeamMember();						break;
	case CMD_FINDTEAM:			pHost->CmdFindTeam();							break;
	case CMD_STARTTRADE:		pHost->CmdStartTrade();							break;
	case CMD_SELLBOOTH:			pHost->CmdSellBooth();							break;
	case CMD_BUYBOOTH:			pHost->CmdBuyBooth();							break;
	case CMD_PLAYPOSE:			pHost->CmdStartPose((int)m_dwParam);			break;
	case CMD_PLAYACTIVEPOSE:	pHost->CmdStartActivePose((int)m_dwParam);
	case CMD_INVITETOFACTION:	pHost->CmdInviteToFaction();					break;
	case CMD_FLY:				pHost->CmdFly();								break;
	case CMD_PICKUP:			pHost->CmdPickup();								break;
	case CMD_GATHER:			pHost->CmdGather();								break;
	case CMD_RUSHFLY:			pHost->CmdRushFly();							break;
	case CMD_BINDBUDDY:			pHost->CmdBindBuddy(pHost->GetSelectedTarget());break;
	case CMD_PET_CALL:			pHost->CmdPetCall();							break;
	case CMD_PET_COMBINE1:		pHost->CmdPetCombine1();						break;
	case CMD_PET_COMBINE2:		pHost->CmdPetCombine2();						break;
	default:
		return false;
	}
	
	return true;
}

//	Get icon file
const char* CECSCCommand::GetIconFile()
{
	const char* szIconFile = "";

	switch (m_iCommand)
	{
	case CMD_SITDOWN:			szIconFile = res_IconFile(RES_ICON_CMD_SITDOWN);			break;
	case CMD_WALKRUN:			szIconFile = res_IconFile(RES_ICON_CMD_WALKRUN);			break;
	case CMD_NORMALATTACK:		szIconFile = res_IconFile(RES_ICON_CMD_NORMALATTACK);		break;
	case CMD_FINDTARGET:		szIconFile = res_IconFile(RES_ICON_CMD_FINDTARGET);			break;		
	case CMD_ASSISTATTACK:		szIconFile = res_IconFile(RES_ICON_CMD_ASSISTATTACK);		break;
	case CMD_INVITETOTEAM:		szIconFile = res_IconFile(RES_ICON_CMD_INVITETOTEAM);		break;
	case CMD_LEAVETEAM:			szIconFile = res_IconFile(RES_ICON_CMD_LEAVETEAM);			break;
	case CMD_KICKTEAMMEM:		szIconFile = res_IconFile(RES_ICON_CMD_KICKTEAMMEM);		break;
	case CMD_FINDTEAM:			szIconFile = res_IconFile(RES_ICON_CMD_FINDTEAM);			break;
	case CMD_STARTTRADE:		szIconFile = res_IconFile(RES_ICON_CMD_STARTTRADE);			break;
	case CMD_SELLBOOTH:			szIconFile = res_IconFile(RES_ICON_CMD_SELLBOOTH);			break;
	case CMD_BUYBOOTH:			szIconFile = res_IconFile(RES_ICON_CMD_BUYBOOTH);			break;
	case CMD_INVITETOFACTION:	szIconFile = res_IconFile(RES_ICON_CMD_INVITETOFACTION);	break;
	case CMD_FLY:				szIconFile = res_IconFile(RES_ICON_CMD_FLY);				break;
	case CMD_PICKUP:			szIconFile = res_IconFile(RES_ICON_CMD_PICKUP);				break;
	case CMD_GATHER:			szIconFile = res_IconFile(RES_ICON_CMD_GATHER);				break;
	case CMD_RUSHFLY:			szIconFile = res_IconFile(RES_ICON_CMD_RUSHFLY);			break;
	case CMD_BINDBUDDY:			szIconFile = res_IconFile(RES_ICON_CMD_BINDBUDDY);			break;
	case CMD_PET_CALL:			szIconFile = res_IconFile(RES_ICON_CMD_PET_CALL);			break;
	case CMD_PET_COMBINE1:		szIconFile = res_IconFile(RES_ICON_CMD_PET_COMBINE1);		break;
	case CMD_PET_COMBINE2:		szIconFile = res_IconFile(RES_ICON_CMD_PET_COMBINE2);		break;

	case CMD_PLAYPOSE:
	{
		switch (m_dwParam)
		{
		case ROLEEXP_WAVE:		szIconFile = res_IconFile(RES_ICON_CMD_EXP_WAVE);			break;
		case ROLEEXP_NOD:		szIconFile = res_IconFile(RES_ICON_CMD_EXP_NOD);			break;
		case ROLEEXP_SHAKEHEAD:	szIconFile = res_IconFile(RES_ICON_CMD_EXP_SHAKEHEAD);		break;
		case ROLEEXP_SHRUG:		szIconFile = res_IconFile(RES_ICON_CMD_EXP_SHRUG);			break;
		case ROLEEXP_LAUGH:		szIconFile = res_IconFile(RES_ICON_CMD_EXP_LAUGH);			break;
		case ROLEEXP_ANGRY:		szIconFile = res_IconFile(RES_ICON_CMD_EXP_ANGRY);			break;
		case ROLEEXP_STUN:		szIconFile = res_IconFile(RES_ICON_CMD_EXP_STUN);			break;
		case ROLEEXP_DEPRESSED:	szIconFile = res_IconFile(RES_ICON_CMD_EXP_DEPRESSED);		break;
		case ROLEEXP_KISSHAND:	szIconFile = res_IconFile(RES_ICON_CMD_EXP_KISSHAND);		break;
		case ROLEEXP_SHY:		szIconFile = res_IconFile(RES_ICON_CMD_EXP_SHY);			break;
		case ROLEEXP_SALUTE:	szIconFile = res_IconFile(RES_ICON_CMD_EXP_SALUTE);			break;
		case ROLEEXP_SITDOWN:	szIconFile = res_IconFile(RES_ICON_CMD_EXP_SITDOWN);		break;
		case ROLEEXP_ASSAULT:	szIconFile = res_IconFile(RES_ICON_CMD_EXP_ASSAULT);		break;
		case ROLEEXP_THINK:		szIconFile = res_IconFile(RES_ICON_CMD_EXP_THINK);			break;
		case ROLEEXP_DEFIANCE:	szIconFile = res_IconFile(RES_ICON_CMD_EXP_DEFIANCE);		break;
		case ROLEEXP_VICTORY:	szIconFile = res_IconFile(RES_ICON_CMD_EXP_VICTORY);		break;
		case ROLEEXP_GAPE:		szIconFile = res_IconFile(RES_ICON_CMD_EXP_GAPE);			break;
		case ROLEEXP_KISS:		szIconFile = res_IconFile(RES_ICON_CMD_EXP_KISS);			break;
		case ROLEEXP_FIGHT:		szIconFile = res_IconFile(RES_ICON_CMD_EXP_FIGHT);			break;
		case ROLEEXP_ATTACK1:	szIconFile = res_IconFile(RES_ICON_CMD_EXP_ATTACK1);		break;
		case ROLEEXP_ATTACK2:	szIconFile = res_IconFile(RES_ICON_CMD_EXP_ATTACK2);		break;
		case ROLEEXP_ATTACK3:	szIconFile = res_IconFile(RES_ICON_CMD_EXP_ATTACK3);		break;
		case ROLEEXP_ATTACK4:	szIconFile = res_IconFile(RES_ICON_CMD_EXP_ATTACK4);		break;
		case ROLEEXP_DEFENCE:	szIconFile = res_IconFile(RES_ICON_CMD_EXP_DEFENCE);		break;	
		case ROLEEXP_FALL:		szIconFile = res_IconFile(RES_ICON_CMD_EXP_FALL);			break;
		case ROLEEXP_FALLONGROUND:	szIconFile = res_IconFile(RES_ICON_CMD_EXP_FALLONGROUND);	break;
		case ROLEEXP_LOOKAROUND:	szIconFile = res_IconFile(RES_ICON_CMD_EXP_LOOKAROUND);		break;	
		case ROLEEXP_DANCE:		szIconFile = res_IconFile(RES_ICON_CMD_EXP_DANCE);			break;
//		case ROLEEXP_PUPPETSHOW:	szIconFile = res_IconFile(REC_ICON_CMD_EXP_PUPPET);     break;
		default:
			ASSERT(0);
			break;
		}

		break;
	}

	case CMD_PLAYACTIVEPOSE:
	{
		switch (m_dwParam)
		{
			case ROLEEXP_AP_PROPOSE:	szIconFile = res_IconFile(REC_INCO_CMD_EXP_PROPOSE);			break;
			default:
				ASSERT(0);
				break;
		}

		break;
	}
	default:
		return "";
	}

	return szIconFile;
}

//	Get shortcut description text
const wchar_t* CECSCCommand::GetDesc()
{
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	const wchar_t* szDesc = _AL("");

	switch (m_iCommand)
	{
	case CMD_SITDOWN:			szDesc = pDescTab->GetWideString(CMDDESC_SITDOWN);			break;
	case CMD_WALKRUN:			szDesc = pDescTab->GetWideString(CMDDESC_WALKRUN);			break;
	case CMD_NORMALATTACK:		szDesc = pDescTab->GetWideString(CMDDESC_NORMALATTACK);		break;
	case CMD_FINDTARGET:		szDesc = pDescTab->GetWideString(CMDDESC_FINDTARGET);		break;
	case CMD_ASSISTATTACK:		szDesc = pDescTab->GetWideString(CMDDESC_ASSISTATTACK);		break;
	case CMD_INVITETOTEAM:		szDesc = pDescTab->GetWideString(CMDDESC_INVITETOTEAM);		break;
	case CMD_LEAVETEAM:			szDesc = pDescTab->GetWideString(CMDDESC_LEAVETEAM);		break;
	case CMD_KICKTEAMMEM:		szDesc = pDescTab->GetWideString(CMDDESC_KICKTEAMMEM);		break;
	case CMD_FINDTEAM:			szDesc = pDescTab->GetWideString(CMDDESC_FINDTEAM);			break;
	case CMD_STARTTRADE:		szDesc = pDescTab->GetWideString(CMDDESC_STARTTRADE);		break;
	case CMD_SELLBOOTH:			szDesc = pDescTab->GetWideString(CMDDESC_SELLBOOTH);		break;
	case CMD_BUYBOOTH:			szDesc = pDescTab->GetWideString(CMDDESC_BUYBOOTH);			break;
	case CMD_INVITETOFACTION:	szDesc = pDescTab->GetWideString(CMDDESC_INVITETOFACTION);	break;
	case CMD_FLY:				szDesc = pDescTab->GetWideString(CMDDESC_FLY);				break;
	case CMD_PICKUP:			szDesc = pDescTab->GetWideString(CMDDESC_PICKUP);			break;
	case CMD_GATHER:			szDesc = pDescTab->GetWideString(CMDDESC_GATHER);			break;
	case CMD_RUSHFLY:			szDesc = pDescTab->GetWideString(CMDDESC_RUSHFLY);			break;
	case CMD_BINDBUDDY:			szDesc = pDescTab->GetWideString(CMDDESC_BINDBUDDY);		break;
	case CMD_PET_CALL:			szDesc = pDescTab->GetWideString(CMDDESC_PET_CALL);			break;
	case CMD_PET_COMBINE1:		szDesc = pDescTab->GetWideString(CMDDESC_PET_COMBINE1);		break;
	case CMD_PET_COMBINE2:		szDesc = pDescTab->GetWideString(CMDDESC_PET_COMBINE2);		break;

	case CMD_PLAYPOSE:
	{
		switch (m_dwParam)
		{
		case ROLEEXP_WAVE:			szDesc = pDescTab->GetWideString(FACEDESC_WAVEHAND);	break;
		case ROLEEXP_NOD:			szDesc = pDescTab->GetWideString(FACEDESC_NOD);			break;		
		case ROLEEXP_SHAKEHEAD:		szDesc = pDescTab->GetWideString(FACEDESC_SHADEHEAD);	break;	
		case ROLEEXP_SHRUG:			szDesc = pDescTab->GetWideString(FACEDESC_SHRUG);		break;	
		case ROLEEXP_LAUGH:			szDesc = pDescTab->GetWideString(FACEDESC_LAUGH);		break;		
		case ROLEEXP_ANGRY:			szDesc = pDescTab->GetWideString(FACEDESC_ANGRY);		break;		
		case ROLEEXP_STUN:			szDesc = pDescTab->GetWideString(FACEDESC_FAINT);		break;		
		case ROLEEXP_DEPRESSED:		szDesc = pDescTab->GetWideString(FACEDESC_SAD);			break;		
		case ROLEEXP_KISSHAND:		szDesc = pDescTab->GetWideString(FACEDESC_KISSHAND);	break;		
		case ROLEEXP_SHY:			szDesc = pDescTab->GetWideString(FACEDESC_SHY);			break;		
		case ROLEEXP_SALUTE:		szDesc = pDescTab->GetWideString(FACEDESC_SALUTE);		break;	
		case ROLEEXP_SITDOWN:		szDesc = pDescTab->GetWideString(FACEDESC_SITDOWN);		break;	
		case ROLEEXP_ASSAULT:		szDesc = pDescTab->GetWideString(FACEDESC_CHARGE);		break;	
		case ROLEEXP_THINK:			szDesc = pDescTab->GetWideString(FACEDESC_THINK);		break;		
		case ROLEEXP_DEFIANCE:		szDesc = pDescTab->GetWideString(FACEDESC_CHALLENGE);	break;	
		case ROLEEXP_VICTORY:		szDesc = pDescTab->GetWideString(FACEDESC_WIN);			break;	
		case ROLEEXP_GAPE:			szDesc = pDescTab->GetWideString(FACEDESC_GAPE);		break;
		case ROLEEXP_KISS:			szDesc = pDescTab->GetWideString(FACEDESC_KISS);		break;
		case ROLEEXP_FIGHT:			szDesc = pDescTab->GetWideString(FACEDESC_FIGHT);		break;
		case ROLEEXP_ATTACK1:		szDesc = pDescTab->GetWideString(FACEDESC_ATTACK1);		break;
		case ROLEEXP_ATTACK2:		szDesc = pDescTab->GetWideString(FACEDESC_ATTACK2);		break;
		case ROLEEXP_ATTACK3:		szDesc = pDescTab->GetWideString(FACEDESC_ATTACK3);		break;
		case ROLEEXP_ATTACK4:		szDesc = pDescTab->GetWideString(FACEDESC_ATTACK4);		break;
		case ROLEEXP_DEFENCE:		szDesc = pDescTab->GetWideString(FACEDESC_DEFENCE);		break;	
		case ROLEEXP_FALL:			szDesc = pDescTab->GetWideString(FACEDESC_FALL);		break;
		case ROLEEXP_FALLONGROUND:	szDesc = pDescTab->GetWideString(FACEDESC_FALLONGROUND);break;
		case ROLEEXP_LOOKAROUND:	szDesc = pDescTab->GetWideString(FACEDESC_LOOKAROUND);	break;	
		case ROLEEXP_DANCE:			szDesc = pDescTab->GetWideString(FACEDESC_DANCE);		break;
//		case ROLEEXP_PUPPETSHOW:	szDesc = pDescTab->GetWideString(FACEDESC_PUPPETSHOW);  break;
		default:
			ASSERT(0);
			break;
		}

		break;
	}

	case CMD_PLAYACTIVEPOSE:
	{
		switch(m_dwParam)
		{
		case ROLEEXP_AP_PROPOSE:		szDesc = pDescTab->GetWideString(FACEDESC_PROPOSE);	break;
		default:
			ASSERT(0);
			break;
		}
	
		break;
	}
	default:
		return _AL("");
	}
	
	return szDesc;
}

//	Get item cool time
int CECSCCommand::GetCoolTime(int* piMax/* NULL */)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	int iTime = 0;

	switch (m_iCommand)
	{
	case CMD_SELLBOOTH:
	case CMD_INVITETOTEAM:
	case CMD_STARTTRADE:
		if (pHost->GetSrcServerID()!=0)
		{
			iTime = 1;
			if (piMax)	*piMax = 1;
		}
		break;

	case CMD_RUSHFLY:

		break;

	case CMD_PLAYPOSE:
	case CMD_PLAYACTIVEPOSE:

		iTime = pHost->GetCoolTime(GP_CT_EMOTE, piMax);
		break;

	case CMD_BINDBUDDY:
	{
		CECCounter& cnt = pHost->GetBindCmdCoolCnt();
		iTime = cnt.GetPeriod() - cnt.GetCounter();

		if (piMax)
			*piMax = cnt.GetPeriod();

		break;
	}
	case CMD_PET_COMBINE1:

		iTime = pHost->GetCoolTime(GP_CT_PET_COMBINE1, piMax);
		break;

	case CMD_PET_COMBINE2:

		iTime = pHost->GetCoolTime(GP_CT_PET_COMBINE2, piMax);
		break;

	default:

		if (piMax)	*piMax = 0;
		break;
	}

	return iTime;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECSCSkillGrp
//	
///////////////////////////////////////////////////////////////////////////

CECSCSkillGrp::CECSCSkillGrp()
{ 
	m_iSCType	= SCT_SKILLGRP;
	m_iGroupIdx	= -1;
}

CECSCSkillGrp::CECSCSkillGrp(const CECSCSkillGrp& src) : CECShortcut(src)
{
	m_iGroupIdx	= src.m_iGroupIdx;
	m_strDesc	= src.m_strDesc;
}

//	Initialize object
bool CECSCSkillGrp::Init(int iGroupIdx)
{
	if (iGroupIdx > EC_COMBOSKILL_NUM)
	{
		ASSERT(0);
		m_iGroupIdx = 0;
	}
	else
		m_iGroupIdx = iGroupIdx;

	CECStringTab* pStrTab = g_pGame->GetItemDesc();
	m_strDesc.Format(pStrTab->GetWideString(CMDDESC_SKILLGROUP), m_iGroupIdx);
	return true;
}

//	Clone shortcut
CECShortcut* CECSCSkillGrp::Clone()
{
	return new CECSCSkillGrp(*this);
}

//	Execute shortcut
bool CECSCSkillGrp::Execute(unsigned int keycode)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	pHost->ApplyComboSkill(m_iGroupIdx);
	return true;
}

//	Get icon file
const char* CECSCSkillGrp::GetIconFile()
{
	return "";
}

//	Get shortcut description text
const wchar_t* CECSCSkillGrp::GetDesc()
{
	return m_strDesc;
}

//	Get item cool time
int CECSCSkillGrp::GetCoolTime(int* piMax/* NULL */)
{
	return 0;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECSCSkill
//	
///////////////////////////////////////////////////////////////////////////

CECSCSkillDumb::CECSCSkillDumb()
{
	m_iSCType	= SCT_SKILLDUMB;
	m_iSkillId	= 0;
}

CECSCSkillDumb::CECSCSkillDumb(const CECSCSkillDumb& src) : CECShortcut(src)
{
	m_iSkillId = src.m_iSkillId;
}

CECSCSkillDumb::~CECSCSkillDumb()
{
}

//	Clone shortcut
CECShortcut* CECSCSkillDumb::Clone()
{
	return new CECSCSkillDumb(*this);
}

//	Initialize object
bool CECSCSkillDumb::Init(int id)
{
	m_iSkillId = id;
	return true;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECSCTarget
//	
///////////////////////////////////////////////////////////////////////////

CECSCTarget::CECSCTarget()
{
	m_iSCType		= SCT_TARGET;

	m_nTargetType	= TT_UNKNOWN;

}

CECSCTarget::CECSCTarget(const CECSCTarget& src) : CECShortcut(src)
{
	m_nTargetType	= src.m_nTargetType;

	m_nTargetID		= src.m_nTargetID;
	m_nTargetLevel	= src.m_nTargetLevel;
	m_nTargetProf	= src.m_nTargetProf;
	m_nFeishengCnt	= src.m_nFeishengCnt;
	m_nCultivation	= src.m_nCultivation;

	m_strName		= src.m_strName;

	m_strIconFile	= src.m_strIconFile;

}

CECSCTarget::~CECSCTarget()
{
}

//	Clone shortcut
CECShortcut* CECSCTarget::Clone()
{
	return new CECSCTarget(*this);
}

//	Initialize object
bool CECSCTarget::Init(int nTargetType, int id, int level,int prof, int feishengcnt, int cultivation)
{
	if(nTargetType >= NUM_TT || nTargetType <= 0)
		return false;

	m_nTargetType	= nTargetType;
	
	if(m_nTargetType == TT_PLAYER)
	{
		m_nTargetID		= id;
		m_nTargetLevel	= level;
		m_nTargetProf	= prof;

		m_nFeishengCnt	= feishengcnt;
		m_nCultivation	= cultivation;
	}
	else if (m_nTargetType == TT_MONSTER)
	{
		m_nTargetID		= id;
		m_nTargetLevel	= level;
		m_nTargetProf	= prof;
	}

	BuildDesc();

	return true;
}


//	Execute shortcut
bool CECSCTarget::Execute(unsigned int keycode)
{
	if(!CanExecute())
		return false;

	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	ASSERT(pHost);

	int idCurSel = pHost->GetSelectedTarget();
	if(idCurSel != 0)	// 已经选择了一个对象，防止重复向server发select命令
	{
		// 判断当前选择对象跟目标对象是否是同一个
		if(ISPLAYERID(idCurSel) && m_nTargetType == TT_PLAYER)
		{
			CECPlayer* player = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(idCurSel);
			if(!player)
				return false;

			int cid = player->GetCharacterID();
			if(cid != m_nTargetID)
			{
				SelectTarget();
				return true;
			}
		}
		else if(ISNPCID(idCurSel) && m_nTargetType == TT_MONSTER)
		{
			CECNPC* npc = g_pGame->GetGameRun()->GetWorld()->GetNPCMan()->GetNPC(idCurSel);
			if(!npc)
				return false;

			int tid = npc->GetTemplateID();
			if(tid != m_nTargetID)
			{
				SelectTarget();
				return true;
			}
		}

		SelectTarget();
	}
	else	// 当前选择为空
	{
		SelectTarget();
	}

	return true;
}

//  Check whether can execute 
bool CECSCTarget::CanExecute()
{
	if(m_nTargetType == TT_PLAYER)
	{
		CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
		if(pHost->GetCharacterID() == m_nTargetID) // self
		{
			// 此处update一下数据
			m_nTargetLevel	= pHost->GetBasicProps().iLevel;
			m_nTargetProf	= pHost->GetBasicProps().iProfession;
			m_nFeishengCnt	= pHost->GetRebornCount();
			m_nCultivation	= pHost->GetCultivation();

			BuildDesc();

			return true;
		}

		float fCloneDist = 999999.0f; // 分身
		CECElsePlayer* player = (CECElsePlayer*)g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(m_nTargetID);
		if(!player)	//玩家不在周围
			return false;

		if(player->GetBasicProps().iProfession >= 45 && player->GetBasicProps().iProfession <= 49) //是怀光 
		{
			int cloneid = player->GetCloneID();
			if(cloneid != 0) // 有分身
			{
				CECPlayer* pClonePlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(cloneid);
				if(!pClonePlayer)
					return false;

				fCloneDist = pClonePlayer->CalcDist(pHost->GetPos(), false);				
			}
		}

		float fDist = player->GetDistToHostH();
		if(fDist < 20.0f || fCloneDist < 20.0f)
		{
			// 此处update一下数据
			m_nTargetLevel	= player->GetBasicProps().iLevel;
			m_nTargetProf	= player->GetBasicProps().iProfession;
			m_nFeishengCnt	= player->GetRebornCount();
			m_nCultivation	= player->GetCultivation();

			BuildDesc();

			return true;
		}
	}
	else if(m_nTargetType == TT_MONSTER)
	{
		CECNPC* npc = g_pGame->GetGameRun()->GetWorld()->GetNPCMan()->GetNPCByTemplateID(m_nTargetID);
		if(npc)
			return true;
	}

	return false;
}

void CECSCTarget::BuildDesc()
{
	if(m_nTargetType == TT_PLAYER)
	{
		m_strName = g_pGame->GetGameRun()->GetPlayerName(m_nTargetID, false);
//		if(m_strName.GetLength() == 0)
//		{
//			g_pGame->GetGameSession()->GetPlayerBriefInfo(1, &m_nTargetID, 2);
//		}

		ACString strLevel;
		if(m_nFeishengCnt != 0)
			strLevel.Format(g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetStringFromTable(7011), m_nTargetLevel);
		else
			strLevel.Format(g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetStringFromTable(7200), m_nTargetLevel);

		ACString strCultivation;
		strCultivation.Format(g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetStringFromTable(7000+m_nCultivation));
		
		ACString strProf;
		strProf.Format(g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetStringFromTable(m_nTargetProf + PROFESSION_START));

		m_strDesc.Format(_AL("%s%s\r%s%s%s%s"), g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetStringFromTable(13302), m_strName,  strLevel,
			g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetStringFromTable(13303), strCultivation, strProf);	
	}
	else if (m_nTargetType == TT_MONSTER)
	{
		//	Get database data
		elementdataman* pDB = g_pGame->GetElementDataMan();
		DATA_TYPE DataType;
		//	Get database data
		const MONSTER_ESSENCE* pMonEssence = (MONSTER_ESSENCE*)pDB->get_data_ptr(m_nTargetID, ID_SPACE_ESSENCE, DataType);
		if(!pMonEssence)
			return;

		m_strName = pMonEssence->name;
		ACString strLevel;
		strLevel.Format(g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetStringFromTable(7200), m_nTargetLevel);

		m_strDesc.Format(_AL("%s%s\r%s"),g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetStringFromTable(13302), m_strName, strLevel);
	}
}


//  Select a target
void CECSCTarget::SelectTarget()
{
	if(m_nTargetType == TT_PLAYER)
	{
		// 判断是不是怀光，如果是，判断是否有分身；选择真身和分身距离host比较近的一个
		CECPlayer* player = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(m_nTargetID);
		if(!player)
			return;

		CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
		if(m_nTargetID != pHost->GetCharacterID() && player->GetBasicProps().iProfession >= 45 && player->GetBasicProps().iProfession <= 49 ) 
		{
			int cloneid = player->GetCloneID();
			if(cloneid != 0) // 有分身
			{
				CECPlayer* pClonePlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(cloneid);
				if(!pClonePlayer)
					return;

				float fDist = player->CalcDist(pHost->GetPos(), false);
				float fCloneDist = pClonePlayer->CalcDist(pHost->GetPos(), false);

				if(fDist < fCloneDist)
					g_pGame->GetGameSession()->c2s_CmdSelectTarget(m_nTargetID);
				else
					g_pGame->GetGameSession()->c2s_CmdSelectTarget(pClonePlayer->GetCharacterID());	

				return;
			}
		}
		
		g_pGame->GetGameSession()->c2s_CmdSelectTarget(m_nTargetID);	
	}
	else if(m_nTargetType == TT_MONSTER)
	{
		// 寻找周围npc
		CECNPC* pNearestNPC = g_pGame->GetGameRun()->GetWorld()->GetNPCMan()->GetNPCByTemplateID(m_nTargetID);
		if(!pNearestNPC)
			return;

		int nid = pNearestNPC->GetNPCID();
		g_pGame->GetGameSession()->c2s_CmdSelectTarget(nid);
	}
}
