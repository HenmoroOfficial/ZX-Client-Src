// File		: DlgTeamMain.cpp
// Creator	: Xiao Zhou
// Date		: 2005/8/17

#include "AFI.h"
#include "A2DSprite.h"
#include "DlgTeamMain.h"
#include "DlgTeamMate.h"
#include "DlgHost.h"
#include "EC_GameUIMan.h"
#include "EC_Game.h"
#include "EC_HostPlayer.h"
#include "EC_Team.h"
#include "EC_World.h"
#include "EC_ManPlayer.h"
#include "ElementSkill.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUICommon.h"
#include "AUI\\AUIProgress.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUICTranslate.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "gt_overlay.h"

#define new A_DEBUG_NEW
#define CDLGTEAMMAIN_MAXTEAMMATES		6

AUI_BEGIN_COMMAND_MAP(CDlgTeamMain, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",	OnCommandCANCAL)
AUI_ON_COMMAND("minimize",	OnCommandMinimize)
AUI_ON_COMMAND("arrow",		OnCommandArrow)
AUI_ON_COMMAND("Btn_Mic",	OnCommandGTEnterTeam)

AUI_END_COMMAND_MAP()

CDlgTeamMain::CDlgTeamMain()
{
	m_bShowTeamArrow = false;
}

CDlgTeamMain::~CDlgTeamMain()
{
}

void CDlgTeamMain::OnShowDialog()
{
	if(GetDlgItem("Btn_Mic"))
		GetDlgItem("Btn_Mic")->Show(GetHostPlayer()->GetSrcServerID()==0);
}

void CDlgTeamMain::OnCommandCANCAL(const char *szCommand)
{
}

void CDlgTeamMain::OnCommandMinimize(const char *szCommand)
{
	int nStatus = (int)GetData();
	if( nStatus == TEAM_STATUS_OPEN )
		SetData(TEAM_STATUS_CLOSE);
	else
		SetData(TEAM_STATUS_OPEN);
}

void CDlgTeamMain::OnCommandArrow(const char *szCommand)
{
	m_bShowTeamArrow = !m_bShowTeamArrow;
}

void CDlgTeamMain::OnCommandGTEnterTeam(const char *szCommand)
{
	if (!GetGameRun()->IsGTLogin())
	{
		GetGame()->GetGameSession()->get_platform_token("gt", 24, 2400);
	}
	
	// CECHostPlayer *pHost = GetHostPlayer();
	// CECTeam *pTeam = pHost->GetTeam();
	// Team ID是队长ID， 换队长或踢人之类的team ID会变
	// 我们服务器跟GT服务器做完映射后Id不一样，传0让服务器自己去查
	overlay::GTOverlay::Instance().EnterTeam(0/*pTeam->GetLeaderID()*/);
}

bool CDlgTeamMain::UpdateTeamInfo()
{
	CECWorld *pWorld = GetWorld();
	CECHostPlayer *pHost = GetHostPlayer();
	CECTeam *pTeam = pHost->GetTeam();
	int nNumMembers = pTeam ? pTeam->GetMemberNum() : 0;

	PAUIDIALOG pDlgLast = GetGameUIMan()->m_pDlgHost;
	if( nNumMembers > 0 )
	{
		if( !IsShow() )
		{
			Show(true);
			AlignTo(pDlgLast,
				AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
				AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_BOTTOM, 0, 50);
		}
		pDlgLast = this;
	}
	else if( IsShow() )
		Show(false);

	ACHAR szText[256];
	AString strFile;
	CDlgTeamMate *pDlgMate;
	PAUIPROGRESS pProgress;
	CECTeamMember *pMember;
	PAUIIMAGEPICTURE pImage;
	int i, j, idMateDlg = 1, idPlayer;
	int nStatus = (int)GetData();
	CECPlayerMan *pPlayerMan = pWorld->GetPlayerMan();
	const char *szCountry = GetGame()->GetConfigs()->GetCountryRegion();
	
	for( i = 0; i < nNumMembers; i++ )
	{
		pMember = pTeam->GetMemberByIndex(i);
		idPlayer = pMember->GetCharacterID();
		if( pHost->GetCharacterID() == idPlayer )
			continue;

		pDlgMate = GetGameUIMan()->m_pDlgTeamMate[idMateDlg - 1];
		if( nStatus == TEAM_STATUS_CLOSE )
		{
			if( pDlgMate->IsShow() )
				pDlgMate->Show(false);
		}
		else
		{
		//	if (pDlgMate->m_nDeityLv!=pMember->GetDeityLevel())
			{
				pDlgMate->m_nDeityLv = pMember->GetDeityLevel();
				AUIOBJECT_SETPROPERTY p;
				if (pDlgMate->m_nDeityLv>0)
					sprintf(p.fn, "Window\\Teammate_Fengshen.tga");
				else
					sprintf(p.fn, "Window\\TeamMate.tga");
				pDlgMate->SetProperty("Frame Image", &p);
			}
			PAUIIMAGEPICTURE pWallow = (PAUIIMAGEPICTURE)pDlgMate->GetDlgItem("Img_Captivation");
			if( pMember->GetWallowLev()>=0 && 0!=stricmp(szCountry, "kr") )
			{
				pWallow->Show(true);
				pWallow->FixFrame(pMember->GetWallowLev());
			}
			else
				pWallow->Show(false);
			pImage = pDlgMate->m_pImgProf;
			pImage->SetHint(GetStringFromTable(PROFESSION_START + pMember->GetProfession()));
			pImage->FixFrame(pMember->GetProfession());

			AUI_ConvertChatString(pMember->GetName(), szText);
			pDlgMate->m_pTxtCharName->SetText(szText);
			pDlgMate->m_pTxtCharName->SetColor(CECPlayer::DecideNameColor(pMember->GetPKLev()));

			a_sprintf(szText, _AL("%3d"), pMember->GetLevel());
			pDlgMate->m_pTxtLV->SetText(szText);

			a_sprintf(szText, _AL("HP: %d/%d"),	pMember->GetCurHP(), pMember->GetMaxHP());

			pProgress = pDlgMate->m_pPrgsHP;
			pProgress->SetProgress(pMember->GetCurHP()
				* AUIPROGRESS_MAX / max(pMember->GetMaxHP(), 1));
			pProgress->SetHint(szText);
			
			if(pMember->GetProfession() >= 108 && pMember->GetProfession() <= 112)
			{
				a_sprintf(szText, _AL("RAGE: %d/%d"),	pMember->GetCurRage(), pMember->GetMaxHP());
				pProgress = pDlgMate->m_pPrgsMP;

				AUIOBJECT_SETPROPERTY p;
				sprintf(p.fn, "zhuxian3\\Progress\\Prgs_Ap.tga");
				pProgress->SetProperty("Fill Image File", &p);
				pProgress->SetProgress(pMember->GetCurRage()
					* AUIPROGRESS_MAX / max(pMember->GetMaxHP(), 1));
				pProgress->SetHint(szText);
			}
			else
			{
				a_sprintf(szText, _AL("MP: %d/%d"),	pMember->GetCurMP(), pMember->GetMaxMP());
				pProgress = pDlgMate->m_pPrgsMP;

				AUIOBJECT_SETPROPERTY p;
				sprintf(p.fn, "Progress\\Prgs_TeamMp.tga");
				pProgress->SetProperty("Fill Image File", &p);
				pProgress->SetProgress(pMember->GetCurMP()
					* AUIPROGRESS_MAX / max(pMember->GetMaxMP(), 1));
				pProgress->SetHint(szText);
			}
			
			a_sprintf(szText, _AL("DP: %d/%d"),	pMember->GetCurDP(), pMember->GetMaxDP());
			pProgress = pDlgMate->m_pPrgsDP;
			pProgress->SetProgress(pMember->GetCurDP()
				* AUIPROGRESS_MAX / max(pMember->GetMaxDP(), 1));
			pProgress->SetHint(szText);
			pProgress->Show(pMember->GetDeityLevel()>0);

			if( !pDlgMate->IsShow() )
				pDlgMate->Show(true);

			pDlgMate->SetCanOutOfWindow(true);
			pDlgMate->AlignTo(pDlgLast,
				AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
				AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_BOTTOM);
			pDlgMate->SetCanMove(false);
			pDlgMate->SetData(pMember->GetCharacterID());

			pImage = pDlgMate->m_pImgCombatMask;
			if( pMember->IsFighting() )
			{
				pImage->Show(true);
				pImage->FadeInOut(1500);
			}
			else
			{
				pImage->Show(false);
				pImage->FadeInOut(0);
			}
			
			if( 1 == idMateDlg )	// Team leader must be the first one.
			{
				if( pTeam && pTeam->GetLeaderID() == idPlayer )
					pDlgMate->m_pImgLeader->Show(true);
				else
					pDlgMate->m_pImgLeader->Show(false);
			}

			for( j = 0; j < CDLGTEAMMATE_ST_MAX ; j++ )
			{
				pImage = pDlgMate->m_pImgSt[j];
				if( !pImage ) break;

				if( j < pMember->GetIconStateNum() )
				{
					const GNET::TeamState *pTS = GNET::TeamState::Query(pMember->GetIconState(j).id);
					if( pTS )
						af_GetFileTitle(pTS->GetIcon(), strFile);
					else
						strFile = "";
					
					strFile.MakeLower();
					pImage->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_STATE],
						GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_STATE][strFile]);
					if( !pTS )
						pImage->SetHint(_AL(""));
					else
					{
						const ACHAR * szDesc = GetGame()->GetBuffDesc(pTS->GetID());
						AUICTranslate trans;
						if( szDesc )
							pImage->SetHint(trans.Translate(szDesc));
						else
							pImage->SetHint(trans.Translate(pTS->GetName()));
					}
				}
				else
				{
					pImage->ClearCover();
					pImage->SetHint(_AL(""));
				}
			}

			pDlgLast = pDlgMate;
		}

		idMateDlg++;
	}

	for( i = idMateDlg; i < CDLGTEAMMAIN_MAXTEAMMATES; i++ )
	{
		pDlgMate = GetGameUIMan()->m_pDlgTeamMate[idMateDlg - 1];
		if( pDlgMate->IsShow() )
			pDlgMate->Show(false);

		idMateDlg++;
	}

	return true;
}
