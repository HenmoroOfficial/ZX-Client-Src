// Filename	: DlgCalendar.cpp
// Creator	: Fu Chonggang
// Date		: 2009/02/10

#include "DlgCalendar.h"
#include "DlgSnSInfo.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameUIMan.h"
#include "EC_Calendar.h"
#include "aui/AUILabel.h"
#include "aui/AUICheckBox.h"


#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgCalendar, CDlgBase)

AUI_ON_COMMAND("yearup",		OnCommandYearUp)
AUI_ON_COMMAND("yeardown",		OnCommandYearDown)
AUI_ON_COMMAND("ok",			OnCommandConfirm)
AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgCalendar, CDlgBase)

AUI_ON_EVENT("Txt_Day*",		WM_LBUTTONDOWN,		OnEventLButtonDay)
AUI_ON_EVENT("Txt_Day*",		WM_LBUTTONDBLCLK,	OnEventLButtonDblClkDay)

AUI_END_EVENT_MAP()

CDlgCalendar::CDlgCalendar()
{
	m_pYear = NULL;
	m_pMonth= NULL;
	m_pCalendar = NULL;
}

CDlgCalendar::~CDlgCalendar()
{
}

void CDlgCalendar::OnEventLButtonDay(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	PAUIOBJECT pObj1;
	for (int i=0; i<42; i++)
	{
		char szName[20];
		sprintf(szName,"Txt_Day%02d",i);
		pObj1 = GetDlgItem(szName);
		if (pObj1 == pObj &&pObj1->IsEnabled())
		{
			pObj1->SetColor(A3DCOLORRGB(255, 0, 0));
			m_pCalendar->m_iDay = a_atoi(pObj1->GetText());
		}
		else
		{
			pObj1->SetColor(A3DCOLORRGB(255, 255, 255));
		}
	}
}
void CDlgCalendar::OnEventLButtonDblClkDay(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	Show(false);
	if (GetGameUIMan()->GetDialog("Win_PersInfo")->IsShow())
	{
		CDlgSnsInfo *pDlgSnsInfo = (CDlgSnsInfo*)GetGameUIMan()->GetDialog("Win_PersInfo");
		pDlgSnsInfo->SetBirth(m_pCalendar->year(),m_pCalendar->month(),m_pCalendar->day());
	}
}

void CDlgCalendar::OnCommandYearUp(const char* szCommand)
{
	if (m_pYear->GetCurSel()<(m_pYear->GetCount()-1))
	{
		m_pYear->SetCurSel(m_pYear->GetCurSel()+1);
		m_pCalendar->m_iYear++;
	}
}

void CDlgCalendar::OnCommandYearDown(const char* szCommand)
{
	if (m_pYear->GetCurSel()>0)
	{
		m_pYear->SetCurSel(m_pYear->GetCurSel()-1);
		m_pCalendar->m_iYear--;
	}
}

void CDlgCalendar::OnCommandConfirm(const char* szCommand)
{
	Show(false);
	if (GetGameUIMan()->GetDialog("Win_PersInfo")->IsShow())
	{
		CDlgSnsInfo *pDlgSnsInfo = (CDlgSnsInfo*)GetGameUIMan()->GetDialog("Win_PersInfo");
		pDlgSnsInfo->SetBirth(m_pCalendar->year(),m_pCalendar->month(),m_pCalendar->day());
	}
}

void CDlgCalendar::OnCommandCancel(const char* szCommand)
{
	Show(false);
}

bool CDlgCalendar::OnInitDialog()
{
	int i;
	ACHAR szText[10];
	m_pMonth->ResetContent();
	for ( i=0; i<12; i++)
	{
		m_pMonth->AddString(GetStringFromTable(7111+i));
	}
	m_pYear->ResetContent();
	for (i=0; i<111; i++)
	{
		a_sprintf(szText,_AL("%d"),i+CECCalendar::YEAR_BASE);
		m_pYear->AddString(szText);
	}
	assert(!m_pCalendar);
	m_pCalendar = new CECCalendar;

	return true;
}

bool CDlgCalendar::Release()
{
	if (m_pCalendar)
	{
		delete m_pCalendar;
		m_pCalendar = NULL;
	}
	return CDlgBase::Release();
}

void CDlgCalendar::DoDataExchange(bool bSave)
{
	CDlgBase::DoDataExchange(bSave);

	DDX_Control("Combo_Year", m_pYear);
	DDX_Control("Combo_Month", m_pMonth);
}

void CDlgCalendar::OnShowDialog()
{
	tm	tmNow = g_pGame->GetServerLocalTime();
	m_pCalendar->m_iDay = tmNow.tm_mday;
	m_pMonth->SetCurSel(tmNow.tm_mon);
	ACHAR szText[20];
	a_sprintf(szText, _AL("%d"), tmNow.tm_year+1900);
	m_pYear->SetCurSel(tmNow.tm_year);
}

void CDlgCalendar::OnTick()
{
	PAUIOBJECT pObj;
	int month = m_pMonth->GetCurSel();
	m_pCalendar->m_iMonth = month;
	int year  = m_pYear->GetCurSel()+CECCalendar::YEAR_BASE;
	m_pCalendar->m_iYear = m_pYear->GetCurSel();
	int iStartDayInMonth = m_pCalendar->CalWeekStart();	// 0-6 0-ÈÕ

	int iday;
	if (0==month)
	{
		iday = CECCalendar::Days(year, CECCalendar::DECEMBER);
	}
	else
	{
		iday = CECCalendar::Days(year, month-1);
	}
	int i;
	for (i=iStartDayInMonth-1; i>=0; i--)
	{
		char szName[20];
		ACHAR szText[20];
		sprintf(szName, "Txt_Day%02d", i);
		pObj = GetDlgItem(szName);
		if (!pObj)
		{
			assert(0);
			return;
		}
		a_sprintf(szText, _AL("%d"), iday--);
		pObj->SetText(_AL(""));
		pObj->Enable(false);
	}

	iday = 1;
	if(m_pCalendar->m_iDay<1)
		m_pCalendar->m_iDay = 1;
	else if (m_pCalendar->m_iDay > CECCalendar::Days(year, month))
		m_pCalendar->m_iDay = CECCalendar::Days(year, month);
	for (i=iStartDayInMonth; i<CECCalendar::Days(year, month)+iStartDayInMonth; i++)
	{
		char szName[20];
		ACHAR szText[20];
		sprintf(szName, "Txt_Day%02d", i);
		pObj = GetDlgItem(szName);
		if (!pObj)
		{
			assert(0);
			return;
		}
		a_sprintf(szText, _AL("%d"), iday++);
		if (m_pCalendar->m_iDay == iday-1)
			pObj->SetColor(A3DCOLORRGB(255, 0, 0));
		else
			pObj->SetColor(A3DCOLORRGB(255, 255, 255));
		pObj->SetText(szText); 
		pObj->Enable(true);
	}

	iday = 1;
	for (i=iStartDayInMonth+CECCalendar::Days(year, month); i<42; i++)
	{
		char szName[20];
		ACHAR szText[20];
		sprintf(szName, "Txt_Day%02d", i);
		pObj = GetDlgItem(szName);
		if (!pObj)
		{
			assert(0);
			return;
		}
		a_sprintf(szText, _AL("%d"), iday++);
		pObj->SetText(_AL(""));
		pObj->Enable(false);
	}
}
