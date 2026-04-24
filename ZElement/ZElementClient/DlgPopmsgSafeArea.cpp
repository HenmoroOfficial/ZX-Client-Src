// Filename	:	DlgPopmsgSafeArea.cpp
// Creator	:	Fu Chonggang
// Date		:	Aug 5, 2010

#include "DlgPopmsgSafeArea.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "EC_GameUIMan.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgPopmsgSafeArea, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",				OnCommand_CANCEL)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgPopmsgSafeArea, CDlgBase)


AUI_END_EVENT_MAP()

const int s_Internal = 10000;
const int s_ShowTime = 5000;
const int s_HideTime = 2000;

CDlgPopmsgSafeArea::CDlgPopmsgSafeArea()
{
}

CDlgPopmsgSafeArea::~CDlgPopmsgSafeArea()
{
}

bool CDlgPopmsgSafeArea::Release()
{
	return CDlgBase::Release();
}

bool CDlgPopmsgSafeArea::OnInitDialog()
{
	m_pMessage = (AUILabel *)GetDlgItem("unsafe");
	m_pBack = (AUIImagePicture *)GetDlgItem("Back");

	m_dwLastTime = 0;
	return true;
}

void CDlgPopmsgSafeArea::OnShowDialog()
{
}

void CDlgPopmsgSafeArea::OnTick()
{
	DWORD curTick = GetTickCount();
	if (curTick>(m_dwLastTime+s_ShowTime))
	{
		if (curTick>(m_dwLastTime+s_ShowTime+s_HideTime))
		{
			Show(false);
			return;
		}
		int t = curTick-(m_dwLastTime+s_ShowTime);
		SetWholeAlpha(( s_HideTime-t )*255/s_HideTime);
	}
}

bool CDlgPopmsgSafeArea::Render()
{
	if( !CDlgBase::Render() )
		return false;
	return true;
}

int CDlgPopmsgSafeArea::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	DWORD curTick = GetTickCount();
	if (strMsg == "show")
	{
		if (curTick<(m_dwLastTime+s_Internal))
			return 1;

		m_dwLastTime = curTick;
		SetWholeAlpha(255);
		Show(true, false, false);
		GetGameUIMan()->AddChatMessage(m_pMessage->GetText(), GP_CHAT_MISC);
	}
	return 0;
}

void CDlgPopmsgSafeArea::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
}

