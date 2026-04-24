// File		: DlgTeamMate.cpp
// Creator	: Xiao Zhou
// Date		: 2005/8/17

#include "DlgTeamMate.h"
#include "DlgTeamContext.h"
#include "EC_MsgDef.h"

#ifdef BACK_VERSION
#include "EC_Manager.h"
#else
#include "ECManager.h"
#include "ECManagerID.h"
#endif
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_Team.h"
#include "EC_HostPlayer.h"
#include "EC_World.h"
#include "EC_ManPlayer.h"
#include "EC_ElsePlayer.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIProgress.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgTeamMate, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",	OnCommandCANCAL)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgTeamMate, CDlgBase)

AUI_ON_EVENT(NULL,	WM_LBUTTONUP,		OnEventLButtonUp)
AUI_ON_EVENT("*",	WM_LBUTTONUP,		OnEventLButtonUp)
AUI_ON_EVENT(NULL,	WM_LBUTTONDBLCLK,	OnEventLButtonClick)
AUI_ON_EVENT("*",	WM_LBUTTONDBLCLK,	OnEventLButtonClick)
AUI_ON_EVENT(NULL,	WM_RBUTTONUP,		OnEventRButtonUp)
AUI_ON_EVENT("*",	WM_RBUTTONUP,		OnEventRButtonUp)

AUI_END_EVENT_MAP()

CDlgTeamMate::CDlgTeamMate()
{
	m_pImgProf = NULL;
	m_pTxtCharName = NULL;
	m_pTxtLV = NULL;
	m_pPrgsHP = NULL;
	m_pPrgsMP = NULL;
	m_pPrgsDP = NULL;
	m_pImgCombatMask = NULL;
	m_pImgLeader = NULL;
	for(int i = 0; i < CDLGTEAMMATE_ST_MAX; i ++)
		m_pImgSt[i] = NULL;
	
	m_nDeityLv = 0;
}

CDlgTeamMate::~CDlgTeamMate()
{
}

void CDlgTeamMate::DoDataExchange(bool bSave)
{
	CDlgBase::DoDataExchange(bSave);

	DDX_Control("Image_Prof", m_pImgProf);
	DDX_Control("Txt_CharName", m_pTxtCharName);
	DDX_Control("Txt_LV", m_pTxtLV);
	DDX_Control("Prgs_HP", m_pPrgsHP);
	DDX_Control("Prgs_MP", m_pPrgsMP);
	DDX_Control("Prgs_Dp", m_pPrgsDP);
	DDX_Control("CombatMask", m_pImgCombatMask);
	if(stricmp("Win_TeamMate1",GetName()) == 0)
		DDX_Control("Img_Leader", m_pImgLeader);
	char szName[40];
	for(int i = 0; i < CDLGTEAMMATE_ST_MAX; i ++)
	{
		sprintf(szName, "St_%d", i + 1);
		DDX_Control(szName, m_pImgSt[i]);
	}
}

void CDlgTeamMate::OnCommandCANCAL(const char *szCommand)
{
}

void CDlgTeamMate::OnEventLButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	int idMate = GetData();
	GetGameSession()->c2s_CmdSelectTarget(idMate);
}

void CDlgTeamMate::OnEventLButtonClick(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	int idMate = GetData();
	GetGameRun()->PostMessage(MSG_HST_FOLLOW, MAN_PLAYER, 0, idMate);
}

void CDlgTeamMate::OnEventRButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	CECTeam *pTeam = GetHostPlayer()->GetTeam();
	if( !pTeam ) return;

	CDlgTeamContext *pMenu;
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;

	if( pTeam->GetLeaderID() == GetHostPlayer()->GetCharacterID() )
		pMenu = GetGameUIMan()->m_pDlgTeamOther1;
	else
		pMenu = GetGameUIMan()->m_pDlgTeamOther2;
	CECPlayerMan *pPlayerMan = GetWorld()->GetPlayerMan();
	if( pPlayerMan->GetElsePlayer(GetData()) != NULL )
		pMenu->GetDlgItem("Follow")->Enable(true);
	else
		pMenu->GetDlgItem("Follow")->Enable(false);
	pMenu->SetData(GetData());
	
	// old pMenu->SetPos(x, y);
	pMenu->SetPosEx(x, y);

	pMenu->Show(true);
}
