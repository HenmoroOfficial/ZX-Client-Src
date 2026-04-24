// Filename	: DlgAction.cpp
// Creator	: Tom Zhou
// Date		: October 19, 2005

#include "AFI.h"
#include "AUI\\AUIImagePicture.h"
#include "DlgAction.h"
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "EC_Shortcut.h"
#include "EC_ShortcutSet.h"

#define new A_DEBUG_NEW

AUI_BEGIN_EVENT_MAP(CDlgAction, CDlgBase)

AUI_ON_EVENT("BscCmd_*",		WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("TeamCmd_*",		WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("TradeCmd_*",		WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("FaceCmd_*",		WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("InteractionCmd_*",		WM_LBUTTONDOWN,		OnEventLButtonDown)

AUI_END_EVENT_MAP()

CDlgAction::CDlgAction()
{
}

CDlgAction::~CDlgAction()
{
}

void CDlgAction::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( !pObj->GetDataPtr("ptr_CECShortcut") ) return;

	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();

	GetGameUIMan()->m_ptLButtonDown.x = GET_X_LPARAM(lParam) - p->X;
	GetGameUIMan()->m_ptLButtonDown.y = GET_Y_LPARAM(lParam) - p->Y;
	GetGameUIMan()->InvokeDragDrop(this, pObj, GetGameUIMan()->m_ptLButtonDown);
}

void CDlgAction::OnTick()
{
	int i, j, nMax;
	char szName[40];
	AString strFile;
	CECShortcut *pSCThis;
	AUIClockIcon *pClock;
	PAUIIMAGEPICTURE pImage;
	CECGameRun* pGameRun = GetGameRun();
	CECShortcutSet *a_pSC[] =
	{
		pGameRun->GetGenCmdShortcuts(),
		pGameRun->GetTeamCmdShortcuts(),
		pGameRun->GetTradeCmdShortcuts(),
		pGameRun->GetPoseCmdShortcuts(),
		pGameRun->GetActivePoseCmdShortcuts(),
	};
	char *a_pszPrefix[] = { "BscCmd_", "TeamCmd_","TradeCmd_", "FaceCmd_", "InteractionCmd_" };

	for( i = 0; i < sizeof(a_pSC) / sizeof(CECShortcutSet *); i++ )
	{
		for( j = 0; j < a_pSC[i]->GetShortcutNum(); j++ )
		{
			sprintf(szName, "%s%02d", a_pszPrefix[i], j + 1);
			pImage = (PAUIIMAGEPICTURE)GetDlgItem(szName);
			if( !pImage ) break;

			pSCThis = a_pSC[i]->GetShortcut(j);
			pImage->SetDataPtr(pSCThis,"ptr_CECShortcut");

			af_GetFileTitle(pSCThis->GetIconFile(), strFile);
			strFile.MakeLower();
			pImage->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_ACTION],
				GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_ACTION][strFile]);

			pClock = pImage->GetClockIcon();
			pClock->SetProgressRange(0, 1);
			pClock->SetProgressPos(1);
			if( pSCThis->GetCoolTime(&nMax) > 0 )
			{
				pClock->SetProgressRange(0, nMax);
				pClock->SetProgressPos(nMax - pSCThis->GetCoolTime());
				pClock->SetColor(A3DCOLORRGBA(0, 0, 0, 128));
			}
		}
	}
}
