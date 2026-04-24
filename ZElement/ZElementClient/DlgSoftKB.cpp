// File		: DlgSoftKB.cpp
// Creator	: Fu Chonggang
// Date		: 2009/02/03

#include "DlgSoftKB.h"

#include "EC_GameUIMan.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgSoftKB, CDlgBase)

AUI_ON_COMMAND("Char_*",		OnCommandChar)
AUI_ON_COMMAND("switch",		OnCommandSwitch)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgSoftKB, CDlgBase)

AUI_END_EVENT_MAP()

CDlgSoftKB::CDlgSoftKB()
{
}

CDlgSoftKB::~CDlgSoftKB()
{
}

void CDlgSoftKB::OnShowDialog()
{
	m_pDlgActive = NULL;
	m_pObjActive = NULL;
}

void CDlgSoftKB::OnTick()
{
	if (NULL==m_pDlgActive)
	{
		PAUIDIALOG pDlg;
		pDlg = GetGameUIMan()->GetDialog("Win_InputString");
		if (pDlg->IsShow())
		{
			m_pDlgActive = pDlg;
		}
		pDlg = GetGameUIMan()->GetDialog("Win_InputString3");
		if (pDlg->IsShow())
		{
			m_pDlgActive = pDlg;
		}
		pDlg = GetGameUIMan()->GetDialog("Win_InputString9");
		if (pDlg->IsShow())
		{
			m_pDlgActive = pDlg;
		}
		pDlg = GetGameUIMan()->GetDialog("Win_GoldPwdChange");
		if (pDlg->IsShow())
		{
			m_pDlgActive = pDlg;
		}
		pDlg = GetGameUIMan()->GetDialog("Win_GoldPwdInput");
		if (pDlg->IsShow())
		{
			m_pDlgActive = pDlg;
		}
	}
	
	if (NULL==m_pDlgActive)
		return;

	PAUIOBJECT pObj = m_pDlgActive->GetFocus();
	if (pObj)
	{
		m_pObjActive = pObj;
	}
}

void CDlgSoftKB::OnCommandChar(const char * szCommand)
{
	if( NULL==m_pDlgActive || NULL==m_pObjActive )
		return;
	ACString strText = m_pObjActive->GetText();
	strText += GetDlgItem(szCommand)->GetText();
	m_pObjActive->SetText(strText);
	GetGameUIMan()->BringWindowToTop(m_pDlgActive);
	m_pDlgActive->ChangeFocus(m_pObjActive);
}

void CDlgSoftKB::OnCommandSwitch(const char * szCommand)
{
	ACHAR szText[40];
	PAUIOBJECT pObj = GetDlgItem("Switch");
	DWORD dwUpper = 1 - pObj->GetData();
	char c, szName[40], cBase = dwUpper ? 'A' : 'a';
	
	for( c = 'a'; c <= 'z'; c++ )
	{
		sprintf(szName, "Char_%c", c);
		a_sprintf(szText, _AL("%c"), cBase + c - 'a');
		GetDlgItem(szName)->SetText(szText);
	}
	pObj->SetData(dwUpper);
}

