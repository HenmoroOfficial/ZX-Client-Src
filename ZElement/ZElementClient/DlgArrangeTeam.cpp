// File		: DlgArrangeTeam.cpp
// Creator	: Xiao Zhou
// Date		: 2005/8/17

#include "DlgArrangeTeam.h"
#include "EC_GameUIMan.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_World.h"
#include "EC_HostPlayer.h"
#include "EC_Team.h"
#include "EC_ElsePlayer.h"
#include "EC_ManPlayer.h"
#include "EC_Configs.h"
#include "EC_Game.h"
//#include "AUI\\AUILabel.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgArrangeTeam, CDlgArrangeTeamSetting)

AUI_ON_COMMAND("kick",			OnCommandKick)
AUI_ON_COMMAND("invite",		OnCommandInvite)
AUI_ON_COMMAND("leave",			OnCommandLeave)
AUI_ON_COMMAND("disband",		OnCommandLeave)
AUI_ON_COMMAND("random",		OnCommandRandom)
AUI_ON_COMMAND("free",			OnCommandRandom)
AUI_ON_COMMAND("slogan",		OnCommandSlogan)
AUI_ON_COMMAND("showself",		OnCommandShowSelf)
AUI_ON_COMMAND("lfg",			OnCommandLfg)
AUI_ON_COMMAND("lfm",			OnCommandLfg)
AUI_ON_COMMAND("makeleader",	OnCommandMakeLeader)
AUI_ON_COMMAND("confirm",		OnCommandConfirm)
AUI_ON_COMMAND("Chk_Auto",		OnCommandAuto)
AUI_ON_COMMAND("Btn_AutoSetting",	OnCommandAutoSetting)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgArrangeTeam, CDlgArrangeTeamSetting)

AUI_ON_EVENT("Lst_Nearbypp",	WM_LBUTTONUP,	OnEventLButtonUp)
AUI_ON_EVENT("Lst_Nearbypp",	WM_RBUTTONUP,	OnEventRButtonUp)

AUI_END_EVENT_MAP()

CDlgArrangeTeam::CDlgArrangeTeam()
	:m_nPickupMode(GP_TMPU_RANDOM)
{
	m_pLstCurrentTeamMate = NULL;
	m_pLstNearbypp = NULL;
	m_pTxtName = NULL;
	m_pTxtGroupMsg = NULL;
	m_pChkSlogan = NULL;
	m_pChkShowself = NULL;
	m_pRdoLfgroup = NULL;
	m_pRdoLfmember = NULL;
	m_pRdoRandom = NULL;
	m_pRdoFree = NULL;
	m_pBtnLeave = NULL;
	m_pBtnKick = NULL;
	m_pBtnInvite = NULL;
	m_pBtnDisband = NULL;
}

CDlgArrangeTeam::~CDlgArrangeTeam()
{
}

void CDlgArrangeTeam::DoDataExchange(bool bSave)
{
	CDlgArrangeTeamSetting::DoDataExchange(bSave);

	DDX_Control("DEFAULT_Lst_CurrentTmate", m_pLstCurrentTeamMate);
	DDX_Control("Lst_Nearbypp", m_pLstNearbypp);
	DDX_Control("Txt_Name",	m_pTxtName);
	DDX_Control("Txt_GroupMsg", m_pTxtGroupMsg);
	DDX_Control("Chk_Slogan", m_pChkSlogan);
	//DDX_Control("Chk_Showself", m_pChkShowself);
	DDX_Control("Rdo_Lfgroup", m_pRdoLfgroup);
	DDX_Control("Rdo_Lfmember", m_pRdoLfmember);
	DDX_Control("Rdo_Random", m_pRdoRandom);
	DDX_Control("Rdo_Free", m_pRdoFree);
	DDX_Control("Btn_Leave", m_pBtnLeave);
	DDX_Control("Btn_Kick", m_pBtnKick);
	DDX_Control("Btn_Invite", m_pBtnInvite);
	DDX_Control("Btn_Disband", m_pBtnDisband);
}

void CDlgArrangeTeam::OnCommandKick(const char *szCommand)
{
	CECHostPlayer *pHost = GetHostPlayer();
	CECTeam *pTeam = pHost->GetTeam();

	if( pTeam && m_pLstCurrentTeamMate->GetCurSel() >= 0 &&
		m_pLstCurrentTeamMate->GetCurSel() < m_pLstCurrentTeamMate->GetCount() )
	{
		CECTeamMember *pMember = pTeam->GetMemberByIndex(m_pLstCurrentTeamMate->GetCurSel());
		if( pMember->GetCharacterID() == pHost->GetCharacterID() )
			GetGameUIMan()->AddChatMessage(GetGameUIMan()->GetStringFromTable(206), GP_CHAT_MISC);
		else
			GetGameSession()->c2s_CmdTeamKickMember(pMember->GetCharacterID());
	}
}

void CDlgArrangeTeam::OnCommandInvite(const char *szCommand)
{
	ACString strName = m_pTxtName->GetText();
	int idPlayer = GetGameRun()->GetPlayerID(strName);

	if( ISPLAYERID(idPlayer) )
	{
		GetGameSession()->c2s_CmdTeamInvite(idPlayer);
		GetGameUIMan()->AddChatMessage(GetGameUIMan()->GetStringFromTable(203), GP_CHAT_MISC);
	}
	else
		GetGameSession()->GetPlayerIDByName(strName, 1);
	//	GetGameUIMan()->AddChatMessage(GetGameUIMan()->GetStringFromTable(531), GP_CHAT_MISC);
}

void CDlgArrangeTeam::OnCommandLeave(const char *szCommand)
{
	CECHostPlayer *pHost = GetHostPlayer();
	CECTeam *pTeam = pHost->GetTeam();

	GetGameUIMan()->MessageBox("Game_TeamDisband", GetGameUIMan()->
		GetStringFromTable(235), MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
}

void CDlgArrangeTeam::OnCommandRandom(const char *szCommand)
{
	CECHostPlayer *pHost = GetHostPlayer();
	CECTeam *pTeam = pHost->GetTeam();

	if( pTeam )
	{
		GetGameUIMan()->AddChatMessage(GetGameUIMan()->GetStringFromTable(207), GP_CHAT_MISC);
		if( m_nPickupMode == GP_TMPU_RANDOM )
			CheckRadioButton(1, 1);
		else
			CheckRadioButton(1, 2);
	}
	else
	{
		if( 0 == stricmp(szCommand, "random") )
			m_nPickupMode = GP_TMPU_RANDOM;
		else
			m_nPickupMode = GP_TMPU_FREEDOM;
		GetGameSession()->c2s_CmdTeamSetPickupFlag(m_nPickupMode);
	}
}

void CDlgArrangeTeam::OnCommandSlogan(const char *szCommand)
{
	CECHostPlayer *pHost = GetGameRun()->GetHostPlayer();
	CECPlayer::TEAMREQ tr = pHost->GetTeamRequire();
	tr.bShowReq = m_pChkSlogan->IsChecked();
	if(NULL != m_pChkShowself)
	  tr.bShowLevel = m_pChkShowself->IsChecked();
	tr.iType = m_pRdoLfgroup->IsChecked() ? 0 : 1;
	pHost->SetTeamRequireText(tr);
	if( m_pChkSlogan->IsChecked() )
		m_pTxtGroupMsg->SetText(pHost->GetTeamReqText());
	else
		m_pTxtGroupMsg->SetText(_AL(""));
}

void CDlgArrangeTeam::OnCommandShowSelf(const char *szCommand)
{
	CECHostPlayer *pHost = GetGameRun()->GetHostPlayer();
	CECPlayer::TEAMREQ tr = pHost->GetTeamRequire();
	tr.bShowReq = m_pChkSlogan->IsChecked();
	if(NULL != m_pChkShowself)
	   tr.bShowLevel = m_pChkShowself->IsChecked();
	tr.iType = m_pRdoLfgroup->IsChecked() ? 0 : 1;
	pHost->SetTeamRequireText(tr);
	if( m_pChkSlogan->IsChecked() )
		m_pTxtGroupMsg->SetText(pHost->GetTeamReqText());
	else
		m_pTxtGroupMsg->SetText(_AL(""));
}

void CDlgArrangeTeam::OnCommandLfg(const char *szCommand)
{
	CECHostPlayer *pHost = GetGameRun()->GetHostPlayer();
	CECPlayer::TEAMREQ tr = pHost->GetTeamRequire();
	tr.bShowReq = m_pChkSlogan->IsChecked();
	if(NULL != m_pChkShowself)
	   tr.bShowLevel = m_pChkShowself->IsChecked();
	tr.iType = m_pRdoLfgroup->IsChecked() ? 0 : 1;
	pHost->SetTeamRequireText(tr);
	if( m_pChkSlogan->IsChecked() )
		m_pTxtGroupMsg->SetText(pHost->GetTeamReqText());
	else
		m_pTxtGroupMsg->SetText(_AL(""));
}

void CDlgArrangeTeam::OnCommandConfirm(const char *szCommand)
{
	CDlgArrangeTeamSetting::OnCommandConfirm("");
	CECHostPlayer *pHost = GetGameRun()->GetHostPlayer();
	CECPlayer::TEAMREQ tr = pHost->GetTeamRequire();
	tr.bShowReq = m_pChkSlogan->IsChecked();
	if(NULL != m_pChkShowself)
	   tr.bShowLevel = m_pChkShowself->IsChecked();
	tr.iType = m_pRdoLfgroup->IsChecked() ? 0 : 1;
	pHost->SetTeamRequire(tr, true);
	if(IsShow())
	  Show(false);
}

void CDlgArrangeTeam::OnCommandAuto(const char *szCommand)
{
	EC_AUTOTEAM_SETTING setting = GetGame()->GetConfigs()->GetAutoTeamSettings();
	PAUICHECKBOX pChk = (PAUICHECKBOX)GetDlgItem("Chk_Auto");
	setting.bUse = pChk->IsChecked();
	GetGame()->GetConfigs()->SetAutoTeamSettings(setting);

}

void CDlgArrangeTeam::OnCommandAutoSetting(const char *szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_ArrangeTeamSip_Setting");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgArrangeTeam::OnShowDialog()
{
	CDlgArrangeTeamSetting::OnShowDialog();

	CECHostPlayer *pHost = GetGameRun()->GetHostPlayer();
	CECPlayer::TEAMREQ tr = pHost->GetTeamRequire();
	m_pChkSlogan->Check(tr.bShowReq);
	if(NULL != m_pChkShowself)
	   m_pChkShowself->Check(tr.bShowLevel);
	if( tr.iType == 0 )
		m_pRdoLfgroup->Check(true);
	else
		m_pRdoLfmember->Check(true);
	if( m_pChkSlogan->IsChecked() )
		m_pTxtGroupMsg->SetText(pHost->GetTeamReqText());
	else
		m_pTxtGroupMsg->SetText(_AL(""));

	EC_AUTOTEAM_SETTING setting = GetGame()->GetConfigs()->GetAutoTeamSettings();
	PAUICHECKBOX pChkAuto = (PAUICHECKBOX)GetDlgItem("Chk_Auto");
	pChkAuto->Check(setting.bUse);
//	PAUILABEL pLab = (PAUILABEL)GetDlgItem("Lab_Nearbypp");
//	pLab->SetText(GetGameUIMan()->GetStringFromTable(pHost->GetSrcServerID()==0 ? 8001 : 8002));
}

void CDlgArrangeTeam::OnEventLButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	POINT ptPos = pObj->GetPos();
	PAUILISTBOX pList = (PAUILISTBOX)pObj;
	int nCurSel = pList->GetCurSel();
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - ptPos.x - p->X;
	int y = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;

	if( nCurSel >= 0 && nCurSel < pList->GetCount() &&
		pList->GetHitArea(x, y) == AUILISTBOX_RECT_TEXT )
	{
		ACString strName = GetGameRun()->GetPlayerName(pList->GetItemData(nCurSel), true);
		m_pTxtName->SetText(strName);
		GetDlgItem("Btn_Invite")->Enable(pList->GetItemData(nCurSel, 1)==0);
	}
}

void CDlgArrangeTeam::OnEventRButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	POINT ptPos = pObj->GetPos();
	PAUILISTBOX pList = (PAUILISTBOX)pObj;
	int nCurSel = pList->GetCurSel();
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - ptPos.x - p->X;
	int y = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;

	if( nCurSel >= 0 && nCurSel < pList->GetCount() &&
		pList->GetHitArea(x, y) == AUILISTBOX_RECT_TEXT )
	{
		int idPlayer = pList->GetItemData(nCurSel);

		x = GET_X_LPARAM(lParam) - p->X;
		y = GET_Y_LPARAM(lParam) - p->Y;
		GetGameUIMan()->PopupPlayerContextMenu(idPlayer, x, y);
	}
}

bool CDlgArrangeTeam::UpdateTeam(bool bUpdateNear)
{
	int i;
	ACString strText;
	CECWorld *pWorld = GetWorld();
	CECHostPlayer *pHost = GetGameRun()->GetHostPlayer();

	if( bUpdateNear && !pHost->IsInAnonymousMap())
	{
		CECPlayerMan *pPlayerMan = pWorld->GetPlayerMan();
		const APtrArray<CECElsePlayer *> &PlayerNear = pPlayerMan->GetPlayersInMiniMap();

		m_pLstNearbypp->ResetContent();
		for( i = 0; i < PlayerNear.GetSize(); i++ )
		{
			if( PlayerNear[i]->GetTeamRequire().bShowReq )
			{
				strText.Format(_AL("%s[%s]"), PlayerNear[i]->GetName(), PlayerNear[i]->
					GetTeamReqText());
			}
			else
				strText = PlayerNear[i]->GetName();
			m_pLstNearbypp->AddString(strText);
			if (PlayerNear[i]->GetSrcServerID()==pHost->GetSrcServerID())
			{
				m_pLstNearbypp->SetItemTextColor(i, A3DCOLORRGB(255,255,255));
				m_pLstNearbypp->SetItemData(i, 0, 1);
			}
			else
			{
				m_pLstNearbypp->SetItemTextColor(i, A3DCOLORRGB(255,0,0));
				m_pLstNearbypp->SetItemData(i, 1, 1);
			}
			m_pLstNearbypp->SetItemData(i, PlayerNear[i]->GetCharacterID());
		}
		m_pLstNearbypp->SetCurSel(-1);

		CECPlayer::TEAMREQ tr = pHost->GetTeamRequire();
		tr.iLevel = pHost->GetBasicProps().iLevel;
		tr.iProfession = pHost->GetProfession();
		pHost->SetTeamRequire(tr, false);
		CheckRadioButton(2, tr.iType + 1);
	}
	
	CECTeamMember *pMember;
	CECTeam *pTeam = pHost->GetTeam();
	int nNumMembers = pTeam ? pTeam->GetMemberNum() : 0;
	PAUILISTBOX pListMate = m_pLstCurrentTeamMate;
	int nCurSel = pListMate->GetCurSel();
	
	pListMate->ResetContent();
	for( i = 0; i < nNumMembers; i++ )
	{
		pMember = pTeam->GetMemberByIndex(i);
		pListMate->AddString(pMember->GetName());
	}
	if( nCurSel < pListMate->GetCount() )
		pListMate->SetCurSel(nCurSel);

	if( pTeam )
	{
		m_pBtnLeave->Enable(true);
		m_pRdoRandom->Enable(false);
		m_pRdoFree->Enable(false);
		if( pTeam->GetLeaderID() == pHost->GetCharacterID() )
		{
			m_pBtnKick->Enable(true);
			m_pBtnInvite->Enable(true);
			m_pBtnDisband->Enable(true);
		}
		else
		{
			m_pBtnKick->Enable(false);
			m_pBtnInvite->Enable(false);
			m_pBtnDisband->Enable(false);
		}
		m_nPickupMode = pTeam->GetPickupFlag();
	}
	else
	{
		m_pBtnKick->Enable(false);
		m_pBtnLeave->Enable(false);
		m_pBtnInvite->Enable(true);
		m_pBtnDisband->Enable(false);
		m_pRdoRandom->Enable(true);
		m_pRdoFree->Enable(true);
	}

	if( m_nPickupMode == GP_TMPU_RANDOM )
		CheckRadioButton(1, 1);
	else
		CheckRadioButton(1, 2);

	return true;
}

void CDlgArrangeTeam::OnCommandMakeLeader(const char * szCommand)
{
	CECTeam *pTeam = GetHostPlayer()->GetTeam();

	if( pTeam && m_pLstCurrentTeamMate->GetCurSel() >= 0 &&
		m_pLstCurrentTeamMate->GetCurSel() < m_pLstCurrentTeamMate->GetCount() )
	{
		CECTeamMember *pMember = pTeam->GetMemberByIndex(m_pLstCurrentTeamMate->GetCurSel());
		if( pMember->GetCharacterID() != GetHostPlayer()->GetCharacterID() )
			GetGameSession()->c2s_CmdTeamChangeLeader(pMember->GetCharacterID());
	}
}
