// Filename	: DlgBase.cpp
// Creator	: Tom Zhou
// Date		: October 11, 2005


#include "DlgBase.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_World.h"
#include "EC_HostPlayer.h"
#include "EC_GameUIMan.h"
#include "EC_UIData.h"
#include "AUI\\AUI.h"

// disable warning "extern on member function"
#pragma warning ( disable : 4630 )

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgBase, AUIDialog)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_Cancel",	OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_Help",		OnCommand_help)
AUI_ON_COMMAND("help",			OnCommand_help)
AUI_ON_COMMAND("Btn_Qshop_*",	OnCommand_OpenQShop)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgBase, AUIDialog)
AUI_END_EVENT_MAP()

CDlgBase::CDlgBase()
{
}

CDlgBase::~CDlgBase()
{
}

bool CDlgBase::OnInitDialog()
{
	if (!AUIDialog::OnInitDialog())
		return false;

	return true;
}

int CDlgBase::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CDlgBase::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
}

void CDlgBase::OnCommand_help(const char * szCommand)
{
	char szName[100];
	sprintf(szName, "%s_Help", GetName());
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog(szName);
	if( pDlg )
	{
		//pDlg->Show(!pDlg->IsShow());
		//pDlg->AlignTo(this, AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
		//		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
		AString strDlg = szName;
		GetGameUIMan()->CommonMessage("Win_HelpInfo","open",(WPARAM)(&strDlg),0);
	}
}

void CDlgBase::OnCommand_OpenQShop(const char *szCommand)
{
	PAUIOBJECT qshopBtn = GetFocus( );
	if (!qshopBtn)
		return;

	const char * name = qshopBtn->GetName( );
	char keyword[256] = {0};
	sscanf( name, "Btn_Qshop_%s", keyword );
		
	GetGameUIMan()->OpenQShop(keyword);
}

CECGame * CDlgBase::GetGame() { return g_pGame; }
CECGameRun * CDlgBase::GetGameRun() { return GetGame()->GetGameRun(); }
CECGameSession * CDlgBase::GetGameSession() { return GetGame()->GetGameSession(); }
CECWorld * CDlgBase::GetWorld() { return GetGameRun()->GetWorld(); }
CECHostPlayer * CDlgBase::GetHostPlayer() { return GetGameRun()->GetHostPlayer(); }
CECGameUIMan * CDlgBase::GetGameUIMan() { return static_cast<CECGameUIMan *>(m_pAUIManager); }
CECUIData * CDlgBase::GetUIData() { return (static_cast<CECGameUIMan *>(m_pAUIManager))->GetUIData(); }


void CDlgBase::SetItemText(const char* pItemName, const int iNumber)
{
	AWString strText;
	strText.Format(_AL("%d"),iNumber);
	SetItemText(pItemName,strText);
}

void CDlgBase::SetItemText(const char* pItemName, const wchar_t* pText)
{
	if (!pItemName || !pItemName[0])
	{
		ASSERT(0);
		return;
	}
	AUIObject* pObj = GetDlgItem(pItemName);
	if (!pObj)
	{
		ASSERT(0);
		return;
	}
	pObj->SetText(pText?pText:L"");
}

void CDlgBase::SetItemHint(const char* pItemName, const int iNumber)
{
	AWString strText;
	strText.Format(_AL("%d"),iNumber);
	SetItemHint(pItemName,strText);
}

void CDlgBase::SetItemHint(const char* pItemName, const wchar_t* pText)
{
	if (!pItemName || !pItemName[0])
	{
		ASSERT(0);
		return;
	}
	AUIObject* pObj = GetDlgItem(pItemName);
	if (!pObj)
	{
		ASSERT(0);
		return;
	}
	pObj->SetHint(pText?pText:L"");
}

void CDlgBase::SetItemColor(const char* pItemName, A3DCOLOR color)
{
	if (!pItemName || !pItemName[0])
	{
		ASSERT(0);
		return;
	}
	AUIObject* pObj = GetDlgItem(pItemName);
	if (!pObj)
	{
		ASSERT(0);
		return;
	}
	pObj->SetColor(color);
}

void CDlgBase::SetItemPos(const char* pItemName, int x, int y)
{
	if (!pItemName || !pItemName[0])
	{
		ASSERT(0);
		return;
	}
	AUIObject* pObj = GetDlgItem(pItemName);
	if (!pObj)
	{
		ASSERT(0);
		return;
	}
	pObj->SetPos(x, y);
}

void CDlgBase::SetItemSize(const char* pItemName, int wid, int hei)
{
	if (!pItemName || !pItemName[0])
	{
		ASSERT(0);
		return;
	}
	AUIObject* pObj = GetDlgItem(pItemName);
	if (!pObj)
	{
		ASSERT(0);
		return;
	}
	pObj->SetSize(wid, hei);
}


void CDlgBase::EnableItem(const char* pItemName, const bool bEnable)
{
	if (!pItemName)
	{
		ASSERT(0);
		return;
	}
	AUIObject* pObj = GetDlgItem(pItemName);
	if (!pObj)
	{
		ASSERT(0);
		return;
	}
	pObj->Enable(bEnable);
}

bool CDlgBase::IsItemEnabled(const char* pItemName)
{
	if (!pItemName)
	{
		ASSERT(0);
		return false;
	}
	AUIObject* pObj = GetDlgItem(pItemName);
	if (!pObj)
	{
		ASSERT(0);
		return false;
	}
	return pObj->IsEnabled();
}

void CDlgBase::ShowItem(const char* pItemName, const bool bShow)
{
	if (!pItemName)
	{
		ASSERT(0);
		return;
	}
	AUIObject* pObj = GetDlgItem(pItemName);
	if (!pObj)
	{
		ASSERT(0);
		return;
	}
	pObj->Show(bShow);	
}

bool CDlgBase::IsItemShow(const char* pItemName)
{
	if (!pItemName)
	{
		ASSERT(0);
		return false;
	}
	AUIObject* pObj = GetDlgItem(pItemName);
	if (!pObj)
	{
		ASSERT(0);
		return false;
	}
	return pObj->IsShow();	
}

const wchar_t*  CDlgBase::GetItemText(const char* pItemName)
{
	if (!pItemName)
	{
		ASSERT(0);
		return NULL;
	}
	AUIObject* pObj = GetDlgItem(pItemName);
	if (!pObj)
	{
		ASSERT(0);
		return NULL;
	}
	return pObj->GetText();
}

void CDlgBase::SetItemProgress(const char* pItemName, const float fProgress, const bool bHint)
{
	if (!pItemName)
	{
		ASSERT(0);
		return;
	}
	AUIObject* pObj = GetDlgItem(pItemName);
	if (!pObj || pObj->GetType()!=AUIOBJECT_TYPE_PROGRESS)
	{
		ASSERT(0);
		return;
	}
	AUIProgress* pProgress = dynamic_cast<AUIProgress*>(pObj);
	if (!pProgress)
	{
		ASSERT(0);
		return;
	}
	int iProgress = (int)(fProgress*AUIPROGRESS_MAX);
	a_Clamp(iProgress,0,AUIPROGRESS_MAX);
	pProgress->SetProgress(iProgress);
	if (bHint)
	{
		AWString strHint;
		strHint.Format(_AL("%.2f%%"), fProgress*100.0f);
		pProgress->SetHint(strHint);
	}
}


void CDlgBase::SetItemActionProgress(const char* pItemName, const float fProgress, const DWORD dwMSecs, const bool bHint)
{
	if (!pItemName)
	{
		ASSERT(0);
		return;
	}
	AUIObject* pObj = GetDlgItem(pItemName);
	if (!pObj || pObj->GetType()!=AUIOBJECT_TYPE_PROGRESS)
	{
		ASSERT(0);
		return;
	}
	AUIProgress* pProgress = dynamic_cast<AUIProgress*>(pObj);
	if (!pProgress)
	{
		ASSERT(0);
		return;
	}
	int iProgress = (int)(fProgress * AUIPROGRESS_MAX);
	a_Clamp(iProgress,0,AUIPROGRESS_MAX);
	pProgress->SetActionProgress(dwMSecs, iProgress);
	if (bHint)
	{
		AWString strHint;
		strHint.Format(_AL("%.2f%%"), fProgress*100.0f);
		pProgress->SetHint(strHint);
	}
}


bool CDlgBase::IsItemChecked(const char* pItemName)
{
	if (!pItemName)
	{
		ASSERT(0);
		return false;
	}
	AUIObject* pObj = GetDlgItem(pItemName);
	if (!pObj || pObj->GetType()!=AUIOBJECT_TYPE_CHECKBOX)
	{
		ASSERT(0);
		return false; 
	}
	AUICheckBox* pCheck = dynamic_cast<AUICheckBox*>(pObj);
	if (!pCheck)
	{
		ASSERT(0);
		return false;
	}
	return pCheck->IsChecked();
}

void CDlgBase::CheckItem(const char* pItemName, const bool bCheck)
{
	if (!pItemName)
	{
		ASSERT(0);
		return;
	}
	AUIObject* pObj = GetDlgItem(pItemName);
	if (!pObj || pObj->GetType()!=AUIOBJECT_TYPE_CHECKBOX)
	{
		ASSERT(0);
		return; 
	}
	AUICheckBox* pCheck = dynamic_cast<AUICheckBox*>(pObj);
	if (!pCheck)
	{
		ASSERT(0);
		return;
	}
	pCheck->Check(bCheck);
}

void CDlgBase::SetItemData(const char* pItemName, DWORD dwData, AString strName)
{
	if (!pItemName)
	{
		ASSERT(0);
		return;
	}
	AUIObject* pObj = GetDlgItem(pItemName);
	if (!pObj)
	{
		ASSERT(0);
		return;
	}
	pObj->SetData(dwData, strName);
}

DWORD CDlgBase::GetItemData(const char* pItemName, AString strName)
{
	if (!pItemName)
	{
		ASSERT(0);
		return 0;
	}
	AUIObject* pObj = GetDlgItem(pItemName);
	if (!pObj)
	{
		ASSERT(0);
		return 0;
	}
	return pObj->GetData(strName);
}

void CDlgBase::SetItemDataPtr(const char* pItemName, void * pvData, AString strName)
{
	if (!pItemName)
	{
		ASSERT(0);
		return;
	}
	AUIObject* pObj = GetDlgItem(pItemName);
	if (!pObj)
	{
		ASSERT(0);
		return;
	}
	pObj->SetDataPtr(pvData, strName);
}

void* CDlgBase::GetItemDataPtr(const char* pItemName, AString strName)
{
	if (!pItemName)
	{
		ASSERT(0);
		return NULL;
	}
	AUIObject* pObj = GetDlgItem(pItemName);
	if (!pObj)
	{
		ASSERT(0);
		return NULL;
	}
	return pObj->GetDataPtr(strName);
}

void CDlgBase::PutControlAbove( PAUIOBJECT pControl, PAUIOBJECT pRelativeTo )
{
	PAUIOBJECTLISTELEMENT pControlEl = NULL;
	PAUIOBJECTLISTELEMENT pRelativeEl = NULL;

	if(!pRelativeTo)
	{
		pRelativeEl = GetLastControl();
		if(pRelativeEl)
			pRelativeTo = pRelativeEl->pThis;
	}
	if(!pControl || pControl == pRelativeTo) return;
	if(pControl->GetParent() != this || (pRelativeTo && pRelativeTo->GetParent() != this)) return;

	PAUIOBJECTLISTELEMENT pElement = GetFirstControl();
	while( pElement )
	{
		if(pElement->pThis == pControl) pControlEl = pElement;
		else if(pElement->pThis == pRelativeTo) pRelativeEl = pElement;

		if(pControlEl && pRelativeEl) break;

		pElement = pElement->pNext;
	}

	if(pControlEl && pRelativeEl)
	{
		if(pControlEl->pLast) pControlEl->pLast->pNext = pControlEl->pNext;
		if(pControlEl->pNext) pControlEl->pNext->pLast = pControlEl->pLast;

		if(m_pListHead == pControlEl) m_pListHead = pControlEl->pNext;
		if(m_pListTail == pControlEl) m_pListTail = pControlEl->pLast;

		pControlEl->pLast = pRelativeEl;
		pControlEl->pNext = pRelativeEl->pNext;
		if (pControlEl->pNext) pControlEl->pNext->pLast = pControlEl;
		pRelativeEl->pNext = pControlEl;

		if(m_pListTail == pRelativeEl) m_pListTail = pControlEl;
	}
}

void CDlgBase::PutControlBelow( PAUIOBJECT pControl, PAUIOBJECT pRelativeTo )
{
	PAUIOBJECTLISTELEMENT pControlEl = NULL;
	PAUIOBJECTLISTELEMENT pRelativeEl = NULL;

	if(!pRelativeTo)
	{
		pRelativeEl = GetFirstControl();
		if(pRelativeEl)
			pRelativeTo = pRelativeEl->pThis;
	}
	if(!pControl || pControl == pRelativeTo) return;
	if(pControl->GetParent() != this || (pRelativeTo && pRelativeTo->GetParent() != this)) return;

	PAUIOBJECTLISTELEMENT pElement = GetLastControl();
	while( pElement )
	{
		if(pElement->pThis == pControl) pControlEl = pElement;
		else if(pElement->pThis == pRelativeTo) pRelativeEl = pElement;

		if(pControlEl && pRelativeEl) break;

		pElement = pElement->pLast;
	}

	if(pControlEl && pRelativeEl)
	{
		if(pControlEl->pNext) pControlEl->pNext->pLast = pControlEl->pLast;
		if(pControlEl->pLast) pControlEl->pLast->pNext = pControlEl->pNext;

		if(m_pListHead == pControlEl) m_pListHead = pControlEl->pNext;
		if(m_pListTail == pControlEl) m_pListTail = pControlEl->pLast;

		pControlEl->pNext = pRelativeEl;
		pControlEl->pLast = pRelativeEl->pLast;
		if (pControlEl->pLast) pControlEl->pLast->pNext = pControlEl;
		pRelativeEl->pLast = pControlEl;

		if(m_pListHead == pRelativeEl) m_pListHead = pControlEl;
	}
}