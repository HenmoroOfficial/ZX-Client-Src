// File		: DlgTeacherMan.cpp
// Creator	: Xiao Zhou
// Date		: 2007/2/19

#include "AFI.h"
#include "AUI\\AUICTranslate.h"
#include "AUI\\CSplit.h"
#include "DlgTeacherMan.h"
#include "DlgCharacter.h"
#include "DlgHost.h"
#include "DlgInputName.h"
#include "DlgSkill.h"
#include "DlgSkillMan.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_Faction.h"
#include "EC_HostPlayer.h"
#include "EC_Skill.h"
#include "sectlist_re.hpp"
#include "sectinvite.hrp"
#include "sectrecruit_re.hpp"
#include "sectexpel_re.hpp"
#include "sectquit.hpp"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgTeacherMan, CDlgBase)

AUI_ON_COMMAND("add*",		OnCommandAdd)
AUI_ON_COMMAND("kick",		OnCommandKick)
AUI_ON_COMMAND("invite",	OnCommandInvite)
AUI_ON_COMMAND("refresh",	OnCommandRefresh)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgTeacherMan, CDlgBase)

AUI_ON_EVENT("List_Member",	WM_RBUTTONUP,	OnEventRButtonUpMember)
AUI_ON_EVENT("Skill_*",		WM_LBUTTONDOWN,	OnEventLButtonDownSkill)
AUI_ON_EVENT("Skill_*",		WM_RBUTTONDOWN,	OnEventRButtonDownSkill)

AUI_END_EVENT_MAP()

CDlgTeacherMan::CDlgTeacherMan()
{
}

CDlgTeacherMan::~CDlgTeacherMan()
{
}

bool CDlgTeacherMan::OnInitDialog()
{
	m_pList_Member = (PAUILISTBOX)GetDlgItem("List_Member");
	m_pBtn_Invite = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Invite");
	m_pBtn_Kick = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Kick");
	m_pBtn_Refresh = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Refresh");
	m_pTxt_TeacherName = (PAUILABEL)GetDlgItem("Txt_TeacherName");
	m_pTxt_Line = (PAUILABEL)GetDlgItem("Txt_Line");
	m_pTxt_StuNum = (PAUILABEL)GetDlgItem("Txt_StuNum");
	m_dwNextRefresh = 0;

	return true;
}

void CDlgTeacherMan::OnShowDialog()
{
	if( m_dwNextRefresh == 0 )
		OnCommandRefresh("");
}

void CDlgTeacherMan::OnCommandInvite(const char * szCommand)
{
	CDlgInputName *pDlg;
	pDlg = (CDlgInputName*)GetGameUIMan()->m_pDlgInputName;
	pDlg->SetData(CDlgInputName::INPUTNAME_ADD_SECT);
	pDlg->Show(true, true);
	pDlg->m_pTxtInput->SetText(_AL(""));
}

void CDlgTeacherMan::OnCommandKick(const char * szCommand)
{
	int nSel = m_pList_Member->GetCurSel();
	if( nSel >= 0 && nSel < m_pList_Member->GetCount() ) 
	{
		ACString strText;
		CSplit s(m_pList_Member->GetText(nSel));
		CSPLIT_STRING_VECTOR vec = s.Split(_AL("\t"));
		strText.Format(GetStringFromTable(1010), vec[0]);
		PAUIDIALOG pMsgBox;
		GetGameUIMan()->MessageBox("Game_SectExpel", strText, MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pMsgBox->SetData(m_pList_Member->GetItemData(nSel));
	}
}

void CDlgTeacherMan::OnCommandAdd(const char * szCommand)
{
	int id = atoi(szCommand + strlen("add"));
	int nCondition = GetHostPlayer()->CheckSkillLearnCondition(id, true);
	if( nCondition == 0 )
		GetGameSession()->c2s_CmdLearnSkill(id);
}

void CDlgTeacherMan::OnCommandRefresh(const char * szCommand)
{
	if( GetTickCount() >= m_dwNextRefresh )
	{
		GetGameSession()->sect_list();
		m_dwNextRefresh = GetTickCount() + 30000;
	}
}

void CDlgTeacherMan::OnTick()
{
	ACString strText;
	m_pBtn_Refresh->Enable(GetTickCount() >= m_dwNextRefresh);
	m_pBtn_Invite->Show(GetHostPlayer()->GetMaster() == GetHostPlayer()->GetCharacterID());
	m_pBtn_Kick->Show(GetHostPlayer()->GetMaster() == GetHostPlayer()->GetCharacterID());
	int nTalentSize = 0;
	short idTalent[10];
	if( CDlgSkill::GetSelectSkill() )
	{
		CECSkill *pSkill = new CECSkill(CDlgSkill::GetSelectSkill(), 1);
		nTalentSize = pSkill->GetTalentSize();
		for(int i = 0; i < nTalentSize; i++ )
			idTalent[i] = pSkill->GetTalent()[i];
		delete pSkill;
	}
	PAUIOBJECTLISTELEMENT pElement = GetFirstControl();
	while( pElement )
	{
		if( strstr(pElement->pThis->GetName(), "Skill_") )
		{
			int id = atoi(pElement->pThis->GetName() + strlen("Skill_"));
			PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(pElement->pThis);
			if( id == 0 )
			{
				pImage->ClearCover();
				pImage->SetData(0);
				pImage->SetDataPtr(NULL);
				pElement = pElement->pNext;
				continue;
			}
			
			CECSkill* pSkill = GetHostPlayer()->GetSkill(id);
			bool bNewSkill = false;
			if( !pSkill || pSkill->GetSkillLevel() == 0 )
			{
				pSkill = new CECSkill(id, 0);
				bNewSkill = true;
				strText.Format(_AL("%d"), 0);
				pImage->SetData(0);
				pImage->SetDataPtr(NULL);
				pImage->SetColor(A3DCOLORRGB(128, 128, 128));
			}
			else
			{
				strText.Format(_AL("%d"), pSkill->GetSkillLevel());
				bool bPositive = pSkill->GetType() != CECSkill::TYPE_PASSIVE && pSkill->GetType() != CECSkill::TYPE_LIVE;
				pImage->SetColor(A3DCOLORRGB(255, 255, 255));
				GetGameUIMan()->m_pDlgSkill->SetImage(pImage, pSkill, bPositive);
				if( !bPositive )
					pImage->SetDataPtr(NULL);
			}
			pImage->SetText(strText);
			int nCondition = GetHostPlayer()->CheckSkillLearnCondition(id, true);
			char szName[20];
			sprintf(szName, "Btn_%02d", id);
			if( GetDlgItem(szName) )
				GetDlgItem(szName)->Show(GetHostPlayer()->GetMaster() == GetHostPlayer()->GetCharacterID() && nCondition == 0);
			if( pSkill )
			{
				if( pSkill->GetSkillID() == CDlgSkill::GetSelectSkill() )
					pImage->SetCover(CDlgSkill::m_pA2DSpriteSkillSelect, (GetTickCount() / 200) % 7, 1);
				else
				{
					bool b = false;
					int i;
					for(i = 0; i < nTalentSize; i++)
						if( idTalent[i] == pSkill->GetSkillID() )
							b = true;
					for(i = 0; i < pSkill->GetTalentSize(); i++)
						if( pSkill->GetTalent()[i] == CDlgSkill::GetSelectSkill() )
							b = true;
					if( b )
						pImage->SetCover(CDlgSkill::m_pA2DSpriteSkillHilight, (GetTickCount() / 200) % 6, 1);
					else
						pImage->SetCover(NULL, 0, 1);
				}
				if( bNewSkill )
				{
					AString strFile;
					af_GetFileTitle(pSkill->GetIconFile(), strFile);
					strFile.MakeLower();
					pImage->SetCover(
						GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_SKILL],
						GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_SKILL][strFile]);
					const wchar_t* szDesc = pSkill->GetDesc();
					AUICTranslate trans;
					if( szDesc )
						pImage->SetHint(trans.Translate(szDesc));
					else
						pImage->SetHint(_AL(""));

					delete pSkill;
				}
			}
			else
			{
				pImage->ClearCover();
				pImage->SetText(_AL(""));
				pImage->SetHint(_AL(""));
			}
		}
		
		pElement = pElement->pNext;
	}
	if( GetHostPlayer()->GetMaster() == GetHostPlayer()->GetCharacterID() )
	{
		PAUIPROGRESS pProgress = (PAUIPROGRESS)GetDlgItem("Pro_Reputation");
		int level = 0;
		int reputation = GetHostPlayer()->GetRegionReputation(8);
		while( reputation >= teacherlevel[level + 1] )
			level++;
		GetDlgItem("Txt_Reputation")->SetText(GetStringFromTable(1140 + level));
		int nNowReputation = reputation - teacherlevel[level];
		int nMaxReputation = teacherlevel[level + 1] - teacherlevel[level];
		pProgress->SetProgress(int(double(nNowReputation) / nMaxReputation * AUIPROGRESS_MAX));
		strText.Format(_AL("%d/%d"), reputation, teacherlevel[level + 1]);
		pProgress->SetHint(strText);
	}
}

bool CDlgTeacherMan::Render()
{
	return CDlgBase::Render();
}

void CDlgTeacherMan::OnEventRButtonUpMember(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	int nSel = m_pList_Member->GetCurSel();
	if( nSel >= 0 && nSel < m_pList_Member->GetCount() ) 
	{
		GetGameUIMan()->PopupPlayerContextMenu(m_pList_Member->GetItemData(nSel));
	}
}

void CDlgTeacherMan::OnSectInvite(const SectInvite* p)
{
	GNET::SectInviteArg* pArg = (GNET::SectInviteArg*)(p->GetArgument());
	ACString strMasterName;
	_cp_str(strMasterName, pArg->role_name.begin(), pArg->role_name.size());
	GetGameRun()->AddPlayerName(pArg->roleid, strMasterName);
	GetGameUIMan()->PopupSectInviteMenu(pArg->roleid, reinterpret_cast<DWORD>(p));
}

void CDlgTeacherMan::OnSectRecruitRe(const GNET::SectRecruit_Re* p)
{
	GNET::SectRecruit_Re* pData = (GNET::SectRecruit_Re*)p;
	ACString strName((const ACHAR*)p->name.begin(), p->name.size() / sizeof(ACHAR));
	ACString strText;
	GetGameRun()->AddPlayerName(pData->disciple, strName);
	if( pData->retcode != ERR_SUCCESS )
	{
		GetGameUIMan()->AddChatMessage(GetStringFromTable(4600 + pData->retcode), GP_CHAT_SYSTEM);
	}
	else
	{
		strText.Format(GetStringFromTable(1004), strName);
		GetGameUIMan()->AddChatMessage(strText, GP_CHAT_SYSTEM);
		m_dwNextRefresh = 0;
		if( IsShow() )
			OnCommandRefresh("");
	}
}

void CDlgTeacherMan::OnSectExpelRe(const GNET::SectExpel_Re* p)
{
	GNET::SectExpel_Re* pData = (GNET::SectExpel_Re*)p;
	ACString strName((const ACHAR*)p->name.begin(), p->name.size() / sizeof(ACHAR));
	ACString strText;
	GetGameRun()->AddPlayerName(pData->disciple, strName);
	if( pData->retcode == ERR_SUCCESS )
	{
		strText.Format(GetStringFromTable(1005), strName);
		GetGameUIMan()->AddChatMessage(strText, GP_CHAT_SYSTEM);
		m_dwNextRefresh = 0;
		if( IsShow() )
			OnCommandRefresh("");
	}
}

void CDlgTeacherMan::OnSectQuit(const GNET::SectQuit* p)
{
	GNET::SectQuit* pData = (GNET::SectQuit*)p;
	ACString strName((const ACHAR*)p->name.begin(), p->name.size() / sizeof(ACHAR));
	ACString strText;
	GetGameRun()->AddPlayerName(pData->disciple, strName);
	if( pData->reason == 0 )
	{
		strText.Format(GetStringFromTable(1006), strName);
		GetGameUIMan()->AddChatMessage(strText, GP_CHAT_SYSTEM);
	}
	else
	{
		strText.Format(GetStringFromTable(1007), strName);
		GetGameUIMan()->AddChatMessage(strText, GP_CHAT_SYSTEM);
	}
	m_dwNextRefresh = 0;
	if( IsShow() )
		OnCommandRefresh("");
}

void CDlgTeacherMan::OnSectListRe(const GNET::SectList_Re* p)
{
	GNET::SectList_Re* pData = (GNET::SectList_Re*)p;
	m_pList_Member->ResetContent();
	ACString strText;
	ACString strMasterName;
	_cp_str(strMasterName, pData->name.begin(), pData->name.size());
	GetGameRun()->AddPlayerName(pData->master, strMasterName);
	m_pTxt_TeacherName->SetText(strMasterName);
	ACString strLine = GetGameUIMan()->GetLine(p->lineid);
	m_pTxt_Line->SetText(strLine);
	strText.Format(_AL("%d/%d"), p->disciples.size(), pData->capacity);
	m_pTxt_StuNum->SetText(strText);
	DWORD i;
	for(i = 0; i < p->disciples.size(); i++)
	{
		strLine = GetGameUIMan()->GetLine(p->disciples[i].lineid);
		ACString strName((const ACHAR*)p->disciples[i].name.begin(), p->disciples[i].name.size() / sizeof(ACHAR));
		strText.Format(_AL("%s\t%d\t%s\t%s"), strName, p->disciples[i].level, GetStringFromTable(2101 + p->disciples[i].occupation), strLine);
		m_pList_Member->AddString(strText);
		m_pList_Member->SetItemData(i, p->disciples[i].roleid);
		GetGameRun()->AddPlayerName(p->disciples[i].roleid, strName);
	}
	PAUIPROGRESS pProgress = (PAUIPROGRESS)GetDlgItem("Pro_Reputation");
	int level = 0;
	while( (int)pData->reputation >= teacherlevel[level + 1] )
		level++;
	GetDlgItem("Txt_Reputation")->SetText(GetStringFromTable(1140 + level));
	int nNowReputation = pData->reputation - teacherlevel[level];
	int nMaxReputation = teacherlevel[level + 1] - teacherlevel[level];
	pProgress->SetProgress(int(double(nNowReputation) / nMaxReputation * AUIPROGRESS_MAX));
	strText.Format(_AL("%d/%d"), pData->reputation, teacherlevel[level + 1]);
	pProgress->SetHint(strText);
}

void CDlgTeacherMan::OnEventLButtonDownSkill(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( !pObj->GetDataPtr("ptr_CECSkill"))
		return;

	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	POINT pt = 
	{
		GET_X_LPARAM(lParam) - p->X,
		GET_Y_LPARAM(lParam) - p->Y,
	};
	
	GetGameUIMan()->InvokeDragDrop(this, pObj, pt);
}

void CDlgTeacherMan::OnEventRButtonDownSkill(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	CDlgSkill::SelectSkill(atoi(pObj->GetName() + strlen("skill_")));
}

