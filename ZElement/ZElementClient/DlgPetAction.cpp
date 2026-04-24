// File		: DlgPetAction.cpp
// Creator	: Xiao Zhou
// Date		: 2006/3/22

#include "AFI.h"
#include "AUI\\AUICTranslate.h"
#include "AUI\\AUICheckBox.h"
#include "DlgPetAction.h"
#include "DlgQuickBar.h"
#include "DlgPetPackage.h"
#include "DlgHostPet.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_GameSession.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_Global.h"
#include "EC_UIManager.h"
#include "EC_FixedMsg.h"
#include "EC_Utility.h"
#include "EC_World.h"
#include "EC_ManNPC.h"
#include "EC_NPC.h"
#include "EC_Skill.h"
#include "EC_IvtrConsume.h"
#include "EC_Inventory.h"
#include "EC_Shortcut.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgPetAction, CDlgBase)

AUI_ON_COMMAND("attack",		OnCommandAttack)
AUI_ON_COMMAND("follow",		OnCommandFollow)
AUI_ON_COMMAND("stop",			OnCommandStop)
AUI_ON_COMMAND("offensive",		OnCommandOffensive)
AUI_ON_COMMAND("defensive",		OnCommandDefensive)
AUI_ON_COMMAND("combat",		OnCommandCombat)
AUI_ON_COMMAND("save",			OnCommandSave)
AUI_ON_COMMAND("default",		OnCommandDefault)
AUI_ON_COMMAND("other",			OnCommandOther)
AUI_ON_COMMAND("Btn_Setting",	OnCommandPetSetting)
AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgPetAction, CDlgBase)

AUI_ON_EVENT("Item_*",			WM_LBUTTONDOWN,		OnEventLButtonDown_Skill)
AUI_ON_EVENT("Item_*",			WM_RBUTTONDOWN,		OnEventRButtonDown_Skill)
AUI_ON_EVENT("Img_AutoFood",	WM_LBUTTONDOWN,		OnEventLButtonDown_AutoFood)
AUI_ON_EVENT("Img_AutoFood",	WM_LBUTTONDBLCLK,	OnEventLButtonDblclk_AutoFood)
AUI_ON_EVENT("Img_AutoFood",	WM_RBUTTONUP,		OnEventLButtonDblclk_AutoFood)

AUI_END_EVENT_MAP()

abase::vector<int> CDlgPetAction::m_vecAutoCastID;
int CDlgPetAction::m_nStayState = 0;
int CDlgPetAction::m_nAggroState = 0;
int CDlgPetAction::m_idLastTarget = 0;
int CDlgPetAction::m_idLastSkill = 0;
DWORD CDlgPetAction::m_dwLastSkillTime = 0;

CDlgPetAction::CDlgPetAction()
{
	m_idLastSkill = 0;
	m_idLastTarget = 0;
}

CDlgPetAction::~CDlgPetAction()
{
}

bool CDlgPetAction::OnInitDialog()
{
	if( stricmp(m_szName, "Win_Pet_Skill") == 0 )
	{
		for(int i = 0; i < 8; i++ )
		{
			char szText[20];
			sprintf(szText, "Item_%0d", i + 1);
			m_pImg_Skill[i] = (PAUIIMAGEPICTURE)GetDlgItem(szText);
		}
	}
	
	return true;
}

void CDlgPetAction::OnShowDialog()
{
	if( stricmp(m_szName, "Win_PetAutoFood") == 0 )
	{
		ACString strText;
		strText.Format(_AL("%d"), CDlgHostPet::GetAutoFoodHp());
		GetDlgItem("Edit_Hp")->SetText(strText);
		((PAUIEDITBOX)GetDlgItem("Edit_Hp"))->SetIsNumberOnly(true);
		strText.Format(_AL("%d"), CDlgHostPet::GetAutoFoodMp());
		GetDlgItem("Edit_Mp")->SetText(strText);
		((PAUIEDITBOX)GetDlgItem("Edit_Mp"))->SetIsNumberOnly(true);
		strText.Format(_AL("%d"), CDlgHostPet::GetAutoFoodFull());
		GetDlgItem("Edit_Full")->SetText(strText);
		((PAUIEDITBOX)GetDlgItem("Edit_Full"))->SetIsNumberOnly(true);
	}
}

bool CDlgPetAction::Release()
{
	return CDlgBase::Release();
}

void CDlgPetAction::OnCommandCancel(const char * szCommand)
{
	if( stricmp(m_szName, "Win_Pet_Skill") == 0 )
		GetGameUIMan()->RespawnMessage();
	else
		Show(false);
}

void CDlgPetAction::OnCommandAttack(const char * szCommand)
{
	CECHostPlayer* pHost = GetHostPlayer();
	int idSelected = pHost->GetSelectedTarget();
	bool bForctAttack = pHost->GetForceAttackFlag(NULL);

	if( pHost->AttackableJudge(idSelected, bForctAttack) == 1 )
	{
		BYTE param = GetHostPlayer()->BuildPVPMask(bForctAttack);
		GetGameSession()->c2s_CmdPetCtrlCmd(GetHostPlayer()->GetCurPetIndex(), GetHostPlayer()->GetSelectedTarget(), 1, &param, sizeof(char));
	}
}

void CDlgPetAction::OnCommandFollow(const char * szCommand)
{
	int param = 0;
	GetGameSession()->c2s_CmdPetCtrlCmd(GetHostPlayer()->GetCurPetIndex(), 0, 2, &param, sizeof(int));
}

void CDlgPetAction::OnCommandStop(const char * szCommand)
{
	int param = 1;
	GetGameSession()->c2s_CmdPetCtrlCmd(GetHostPlayer()->GetCurPetIndex(), 0, 2, &param, sizeof(int));
}

void CDlgPetAction::OnCommandDefensive(const char * szCommand)
{
	int param = 0;
	GetGameSession()->c2s_CmdPetCtrlCmd(GetHostPlayer()->GetCurPetIndex(), 0, 3, &param, sizeof(int));
}

void CDlgPetAction::OnCommandOffensive(const char * szCommand)
{
	int param = 1;
	GetGameSession()->c2s_CmdPetCtrlCmd(GetHostPlayer()->GetCurPetIndex(), 0, 3, &param, sizeof(int));
}

void CDlgPetAction::OnCommandCombat(const char * szCommand)
{
	int param = 2;
	GetGameSession()->c2s_CmdPetCtrlCmd(GetHostPlayer()->GetCurPetIndex(), 0, 3, &param, sizeof(int));
}

void CDlgPetAction::OnCommandSave(const char * szCommand)
{
	int value = a_atoi(GetDlgItem("Edit_Hp")->GetText());
	a_Clamp(value, 0, 100);
	CDlgHostPet::SetAutoFoodHp(value);
	value = a_atoi(GetDlgItem("Edit_Mp")->GetText());
	a_Clamp(value, 0, 100);
	CDlgHostPet::SetAutoFoodMp(value);
	value = a_atoi(GetDlgItem("Edit_Full")->GetText());
	a_ClampFloor(value, 0);
	CDlgHostPet::SetAutoFoodFull(value);
	Show(false);
}

void CDlgPetAction::OnCommandDefault(const char * szCommand)
{
	GetDlgItem("Edit_Hp")->SetText(_AL("50"));
	GetDlgItem("Edit_Mp")->SetText(_AL("50"));
	GetDlgItem("Edit_Full")->SetText(_AL("100"));
}

void CDlgPetAction::OnCommandOther(const char * szCommand)
{
	PAUIDIALOG pDlg;
	Show(false);
	if( stricmp(m_szName, "Win_PetAction") == 0 )
		pDlg = GetGameUIMan()->GetDialog("Win_PetActionSmall");
	else
		pDlg = GetGameUIMan()->GetDialog("Win_PetAction");
	pDlg->Show(true);
	pDlg->AlignTo(this, AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT, 
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
}

void CDlgPetAction::OnCommandPetSetting(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_PetAutoFood");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgPetAction::OnEventLButtonDown_Skill(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	int nIndex = atoi(pObj->GetName() + strlen("Item_")) - 1;
	CastSkill(nIndex, false);
}

void CDlgPetAction::OnEventRButtonDown_Skill(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	int nIndex = atoi(pObj->GetName() + strlen("Item_")) - 1;
	CastSkill(nIndex, true);
	SetCapture(false);
}

void CDlgPetAction::CastSkill(int nIndex, bool bAutoCast)
{
	CECInventory* pPetPack = GetHostPlayer()->GetPetPack();
	CECPetBedge* pPet = (CECPetBedge*)pPetPack->GetItem(GetHostPlayer()->GetCurPetIndex());
	if( !pPet )
		return;
	
	int idSkill = pPet->GetEssence().skills[nIndex].id;
	if( idSkill == 0 )
		return;

	if( bAutoCast )
	{
		#pragma pack(1)

			struct CMDPARAM
			{
				int idSkill;
				unsigned char flag;
			};

		#pragma pack()
		CMDPARAM param;
		param.idSkill = idSkill;
		if( IsSkillAutoCast(idSkill) )
			param.flag = 0;
		else
			param.flag = 1;
		GetGameSession()->c2s_CmdPetCtrlCmd(GetHostPlayer()->GetCurPetIndex(), 0, 5, &param, sizeof(param));

		return;
	}
	
#pragma pack(1)

	struct CMDPARAM
	{
		int idSkill;
		BYTE byPVPMask;
	};

#pragma pack()

	CMDPARAM param;
	param.idSkill = idSkill;
	param.byPVPMask = GetHostPlayer()->BuildPVPMask(GetHostPlayer()->GetForceAttackFlag(NULL));

	int iSkillType = GNET::ElementSkill::GetType(param.idSkill);
	int idTarget = 0;

	idTarget = GetHostPlayer()->GetSelectedTarget();

	if (iSkillType == GNET::TYPE_ATTACK || iSkillType == GNET::TYPE_CURSE)
	{
		bool bForctAttack = GetHostPlayer()->GetForceAttackFlag(NULL);
		if (GetHostPlayer()->AttackableJudge(idTarget, bForctAttack) != 1)
			return;
	}

	if( idTarget == m_idLastTarget && param.idSkill == m_idLastSkill && 
		GetTickCount() - m_dwLastSkillTime < 2000 )
		return;

	GetGameSession()->c2s_CmdPetCtrlCmd(GetHostPlayer()->GetCurPetIndex(), idTarget, 4, &param, sizeof(param));
	SetCapture(false);

	m_idLastTarget = idTarget;
	m_idLastSkill = param.idSkill;
	m_dwLastSkillTime = GetTickCount();
}

void CDlgPetAction::OnEventLButtonDown_AutoFood(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( !pObj->GetDataPtr("ptr_CECShortcut") )
		return;
	
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	POINT pt = 
	{
		GET_X_LPARAM(lParam) - p->X,
		GET_Y_LPARAM(lParam) - p->Y,
	};
	
	GetGameUIMan()->m_ptLButtonDown = pt;
	GetGameUIMan()->InvokeDragDrop(this, pObj, pt);
}

void CDlgPetAction::OnEventLButtonDblclk_AutoFood(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	CECShortcut *pSC = (CECShortcut*)GetGameUIMan()->m_pDlgPetPackage->GetDlgItem("Img_AutoFood")->GetDataPtr("ptr_CECShortcut");
	if( pSC )
	{
		CECSCItem *pSCItem = (CECSCItem*)pSC;
		GetHostPlayer()->UseItemInPack(IVTRTYPE_PACK, pSCItem->GetIvtrSlot());
	}
}

void CDlgPetAction::SetPetAIState(char stay_state, char aggro_state)
{
	m_nStayState = stay_state;
	m_nAggroState = aggro_state;
}

void CDlgPetAction::OnTick()
{
	CECShortcut *pSC = (CECShortcut*)GetGameUIMan()->m_pDlgPetPackage->GetDlgItem("Img_AutoFood")->GetDataPtr("ptr_CECShortcut");
	if( GetDlgItem("Img_AutoFood"))
	{
		CDlgQuickBar::UpdateShortcut(pSC, (PAUIIMAGEPICTURE)GetDlgItem("Img_AutoFood"));
		if( pSC )
		{
			AUICTranslate trans;
			const ACHAR *pszHint = pSC->GetDesc();
			if( pszHint )
			GetDlgItem("Img_AutoFood")->SetHint(trans.Translate(pszHint));
			else
 			GetDlgItem("Img_AutoFood")->SetHint(_AL(""));
		}
		else
	 		GetDlgItem("Img_AutoFood")->SetHint(GetStringFromTable(984));
		if (stricmp(m_szName, "Win_PetAutoFood") == 0)
		{
			return;
		}
	}

	CECHostPlayer* pHost = GetHostPlayer();
	CECInventory* pPetPack = pHost->GetPetPack();
	if( GetHostPlayer()->GetCurPetIndex() == -1 )
		return;

	
	CECPetBedge* pPet = (CECPetBedge*)pPetPack->GetItem(GetHostPlayer()->GetCurPetIndex());
	if( !pPet )
		return;
	
	CheckRadioButton(2, m_nStayState);
	CheckRadioButton(1, m_nAggroState);
	
	CECStringTab* pDescTab = GetGame()->GetItemDesc();
	int i=0;
	for(int j = 0; j < 16; j++ )
	{
		if( j>=8 && j<12 )
			continue;
		if( i>=8 )
			break;
		
		AUIClockIcon *pClock;
		pClock = m_pImg_Skill[i]->GetClockIcon();
		int idSkill = pPet->GetEssence().skills[j].id;
		if( idSkill == 0 )
		{
			continue;
		}
		
		int iLevel =  pPet->GetEssence().skills[j].level;
		CECSkill *pSkill = new CECSkill(idSkill, iLevel);
		if( pSkill )
		{
			m_pImg_Skill[i]->SetData(idSkill);
			m_pImg_Skill[i]->SetHint(pSkill->GetDesc());
			const char* szIcon = GNET::ElementSkill::GetIcon(idSkill);
			AString strFile;
			af_GetFileTitle(szIcon, strFile);
			strFile.MakeLower();
			m_pImg_Skill[i]->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_SKILL],
				GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_SKILL][strFile]);
			
			GNET::UseRequirement Info;
			memset(&Info, 0, sizeof (Info));
			
			Info.mp	= pPet->GetEssence().cur_vigor;
			Info.form = 0;
			if( !pSkill->CheckCastCondition(Info) )
				m_pImg_Skill[i]->SetColor(A3DCOLORRGB(255, 255, 255));
			else
				m_pImg_Skill[i]->SetColor(A3DCOLORRGB(128, 128, 128));
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
			if( IsSkillAutoCast((int)m_pImg_Skill[i]->GetData()) )
				m_pImg_Skill[i]->SetCover(GetGameUIMan()->m_pA2DSpriteMask, 0, 1);
			else
				m_pImg_Skill[i]->SetCover(NULL, 0, 1);
			delete pSkill;
			i++;
		}
	}
	for( ; i<8; i++)
	{
		m_pImg_Skill[i]->ClearCover();
		m_pImg_Skill[i]->SetHint(_AL(""));
		m_pImg_Skill[i]->SetData(0);
	}
}

void CDlgPetAction::SetAutoCastID(int id, unsigned char flag)
{
	for(DWORD i = 0; i < m_vecAutoCastID.size(); i++)
		if( m_vecAutoCastID[i] == id )
		{
			if( flag == 0 )
			{
				m_vecAutoCastID.erase(m_vecAutoCastID.begin() + i);
				return;
			}
		}
	if( flag == 1 )
		m_vecAutoCastID.push_back(id);
}

void CDlgPetAction::ClearAutoCastID()
{
	m_vecAutoCastID.clear();
}

bool CDlgPetAction::IsSkillAutoCast(int id)
{
	for(DWORD i = 0; i < m_vecAutoCastID.size(); i++)
		if( m_vecAutoCastID[i] == id )
			return true;
	return false;
}

void CDlgPetAction::SetAutoFood(CECShortcut *pSC)
{
	AUIObject* pObj = GetDlgItem("Img_AutoFood");
	CECShortcut *pOldSC = (CECShortcut*)pObj->GetDataPtr("ptr_CECShortcut");
	if( pOldSC )
		delete pOldSC;
	PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)pObj;
	CDlgQuickBar::UpdateShortcut(pSC, pImage);
}
