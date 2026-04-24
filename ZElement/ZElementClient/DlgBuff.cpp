// Filename	: DlgBuff.cpp
// Creator	: Xiao Zhou
// Date		: 2006/6/1

#include "AFI.h"
#include "AUI\\AUIDef.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUILabel.h"
#include "DlgBuff.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "EC_UIManager.h"
#include "EC_HostPlayer.h"
#include "EC_Global.h"
#include "EC_Configs.h"
#include "ElementSkill.h"
#include "AUI\\AUICTranslate.h"

#define new A_DEBUG_NEW

const ACHAR DefaultFontName[]=TEXT("方正细黑一简体");
const A3DCOLOR DefaultFontColor=0xFFFFFFFF;


AUI_BEGIN_COMMAND_MAP(CDlgBuff, CDlgBase)

AUI_ON_COMMAND("IDCANCEL", OnCommand_CANCEL)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgBuff, CDlgBase)

AUI_ON_EVENT(NULL,		WM_LBUTTONDOWN,		OnEvent)
AUI_ON_EVENT(NULL,		WM_LBUTTONUP,		OnEvent)
AUI_ON_EVENT(NULL,		WM_LBUTTONDBLCLK,	OnEvent)
AUI_ON_EVENT(NULL,		WM_RBUTTONUP,		OnEvent)
AUI_ON_EVENT(NULL,		WM_RBUTTONDOWN,		OnEvent)
AUI_ON_EVENT(NULL,		WM_RBUTTONDBLCLK,	OnEvent)
AUI_ON_EVENT(NULL,		WM_MOUSEMOVE,		OnEvent)
AUI_ON_EVENT(NULL,		WM_MOUSEWHEEL,		OnEvent)

AUI_END_EVENT_MAP()

CDlgBuff::CDlgBuff()
{
}

CDlgBuff::~CDlgBuff()
{
}

bool CDlgBuff::OnInitDialog()
{
	for (int i=0; ; i++)
	{
		char szName[20];
		sprintf(szName,"Txt_Time%d",i);
		PAUILABEL pLab = (PAUILABEL)GetDlgItem(szName);
		if (!pLab)
		{
			break;
		}
		pLab->SetAlign(AUIFRAME_ALIGN_CENTER);
	}
	((PAUIIMAGEPICTURE)GetDlgItem("Img_Captivation"))->FixFrame(0);
	SetCanMove(false);
	return true;
}
void CDlgBuff::OnCommand_CANCEL(const char * szCommand)
{
}

void CDlgBuff::OnEvent(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	GetGameUIMan()->RespawnMessage();
}

void CDlgBuff::OnTick()
{
	AString strFile;

	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Buff");
	const char *szCountry = g_pGame->GetConfigs()->GetCountryRegion();
	int nStart;
	if( GetGameRun()->GetWallowInfo().anti_wallow_active && 0!=stricmp(szCountry, "kr") )
	{
		nStart = 1;
		GetDlgItem("Img_Buff0")->Show(false);
		GetDlgItem("Txt_Time0")->Show(false);
	}
	else
	{
		nStart = 0;
		GetDlgItem("Img_Buff0")->Show(true);
		GetDlgItem("Txt_Time0")->Show(true);
	}
	for(int i = 0; ; i++ )
	{
		char szName[20];
		sprintf(szName, "Img_Buff%d", i + nStart);
		PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)pDlg->GetDlgItem(szName);
		sprintf(szName, "Txt_Time%d", i + nStart);
		PAUILABEL pTxt = (PAUILABEL)pDlg->GetDlgItem(szName);
		if( !pImg )
			break;
		if( i < GetHostPlayer()->GetIconStateNum() )
		{
			const GNET::TeamState *pTS = GNET::TeamState::Query(GetHostPlayer()->GetIconState(i).id);
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
			if (GetHostPlayer()->GetIconState(i).iOverlayCnt>1)
			{
				ACString strText;
				strText.Format(_AL("%d"), GetHostPlayer()->GetIconState(i).iOverlayCnt);
				pImg->SetText(strText);
			}
			else
				pImg->SetText(_AL(""));
			pTxt->SetText(GetBuffTime(GetHostPlayer()->GetIconState(i).iEndTime));

			pImg->Show(true);
			pTxt->Show(true);
		}
		else
		{
			pImg->Show(false);
			pTxt->Show(false);
		}
	}
}

ACString CDlgBuff::GetBuffTime(int iEndTime)
{
	if( iEndTime == 0 )
		return _AL("");

	int time = iEndTime - g_pGame->GetServerGMTTime();
	if( time <= 0 )
		return _AL("");

	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	ACString strTime;
	if( time >= 3600 )
		strTime.Format(pGameUI->GetStringFromTable(7322), (time - 1) / 3600 + 1);
	else if( time >= 60 )
		strTime.Format(pGameUI->GetStringFromTable(7321), (time - 1) / 60 + 1);
	else
		strTime.Format(pGameUI->GetStringFromTable(7320), time);
	if( time < 30 )
		strTime = _AL("^ff0000") + strTime;
	return strTime;
}