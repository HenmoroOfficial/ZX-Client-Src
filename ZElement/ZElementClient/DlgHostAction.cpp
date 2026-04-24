// Filename	: DlgTeamContext.cpp
// Creator	: Tom Zhou
// Date		: September 1, 2005

#include "A3DDevice.h"
#include "DlgHostAction.h"
#include "EC_HostPlayer.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgHostAction, CDlgBase)

AUI_ON_COMMAND("Btn_CopyName",		OnCommand_CopyName)

AUI_END_COMMAND_MAP()

void CDlgHostAction::OnCommand_CopyName(const char * szCommand)
{
	if( OpenClipboard(m_pA3DDevice->GetDeviceWnd()) )
	{
		const ACHAR * szName = GetHostPlayer()->GetName();
		
		if (szName[1]==_AL(' '))	// GT图标后面有一个空格
			szName += 2;
		
		HLOCAL hLocal = GlobalAlloc(LMEM_ZEROINIT, a_strlen(szName) * 2 + 2);
		if( hLocal )
		{
			ACHAR *pszName = (ACHAR *)GlobalLock(hLocal);
			a_strcpy(pszName, szName);
			EmptyClipboard();
#ifdef UNICODE
			SetClipboardData(CF_UNICODETEXT, pszName);
#else
			SetClipboardData(CF_TEXT, pszName);
#endif
			GlobalUnlock(hLocal);
		}
		
		CloseClipboard();
	}

	Show(false);
}