// TimeRange.cpp : implementation file
//

#include "stdafx.h"
#include "elementeditor.h"
#include "TimeRange.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static inline bool IsLeapYear(int year)
{
	if((year % 4) != 0)  return false;
	if((year % 400) == 0) return true;
	if((year % 100) == 0) return false;
	return true;
}

static inline int GetMDay(int year, int mon)
{
	static int mday[] = { 31,28,31,30,31,30,31,31,30,31,30,31};
	int d = mday[mon];
	if(mon == 1 && IsLeapYear(year)) d = d+1;
	return d;
}


/////////////////////////////////////////////////////////////////////////////
// CTimeRange dialog

const char* szWeek0[8] = 
{
	"任意",
	"周日",
	"周一",
	"周二",
	"周三",
	"周四",
	"周五",
	"周六"
};
CTimeRange::CTimeRange(CWnd* pParent /*=NULL*/)
	: CDialog(CTimeRange::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTimeRange)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pTimeStart = NULL;
	m_pTimeEnd = NULL;
	m_bReadOnly = false;
}

void CTimeRange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTimeRange)
	DDX_Control(pDX, IDC_START_YEAR, m_cbStartYear);
	DDX_Control(pDX, IDC_START_WEEK, m_cbStartWeek);
	DDX_Control(pDX, IDC_START_MOUTH, m_cbStartMonth);
	DDX_Control(pDX, IDC_START_MINUTE, m_cbStartMinute);
	DDX_Control(pDX, IDC_START_HOUR, m_cbStartHour);
	DDX_Control(pDX, IDC_START_DAY, m_cbStartDay);
	DDX_Control(pDX, IDC_END_YEAR, m_cbEndYear);
	DDX_Control(pDX, IDC_END_WEEK, m_cbEndWeek);
	DDX_Control(pDX, IDC_END_MOUTH, m_cbEndMonth);
	DDX_Control(pDX, IDC_END_MINUTE, m_cbEndMinute);
	DDX_Control(pDX, IDC_END_HOUR, m_cbEndHour);
	DDX_Control(pDX, IDC_END_DAY, m_cbEndDay);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTimeRange, CDialog)
	//{{AFX_MSG_MAP(CTimeRange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimeRange message handlers
BOOL CTimeRange::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitCombox();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTimeRange::InitCombox()
{
	CString str,other;
	int sel = 0, i;

	if(!m_pTimeStart || !m_pTimeEnd)
		return;

	if(m_bReadOnly)
	{
		CWnd* pWnd = GetDlgItem(IDC_START_YEAR);
		pWnd->EnableWindow(false);
		pWnd = GetDlgItem(IDC_START_MOUTH);
		pWnd->EnableWindow(false);
		pWnd = GetDlgItem(IDC_START_DAY);
		pWnd->EnableWindow(false);
		pWnd = GetDlgItem(IDC_START_WEEK);
		pWnd->EnableWindow(false);
		pWnd = GetDlgItem(IDC_START_MINUTE);
		pWnd->EnableWindow(false);
		pWnd = GetDlgItem(IDC_START_HOUR);
		pWnd->EnableWindow(false);
		

		pWnd = GetDlgItem(IDC_END_YEAR);
		pWnd->EnableWindow(false);
		pWnd = GetDlgItem(IDC_END_MOUTH);
		pWnd->EnableWindow(false);
		pWnd = GetDlgItem(IDC_END_DAY);
		pWnd->EnableWindow(false);
		pWnd = GetDlgItem(IDC_END_WEEK);
		pWnd->EnableWindow(false);
		pWnd = GetDlgItem(IDC_END_MINUTE);
		pWnd->EnableWindow(false);
		pWnd = GetDlgItem(IDC_END_HOUR);
		pWnd->EnableWindow(false);
	}


	// start year
	if(m_pTimeStart->nYear == -1)
		sel = 0;
	else
		sel = m_pTimeStart->nYear - 2010;

	m_cbStartYear.InsertString(0, "任意");

	for(i=1;i<=8;i++)
	{
		str.Format("%d", 2010+i);
		m_cbStartYear.InsertString(i, str);
	}
	m_cbStartYear.SetCurSel(sel);

	// start month
	other.Format("%d", m_pTimeStart->nMouth + 1);
	sel = 0;
	m_cbStartMonth.InsertString(0,"任意");
	for(i=0;i<12;i++)
	{
		str.Format("%d", i+1);
		m_cbStartMonth.InsertString(i+1, str);
		if(other == str)
			sel = i+1;
	}
	m_cbStartMonth.SetCurSel(sel);

	// start day
	other.Format("%d",m_pTimeStart->nDay);
	sel=0;
	m_cbStartDay.InsertString(0,"任意");
	for( i = 1; i < 32; ++i)
	{
		str.Format("%d",i);
		m_cbStartDay.InsertString(i,str);
		if(other==str) sel = i;
	}
	m_cbStartDay.SetCurSel(sel);

	// start week
	other = szWeek0[m_pTimeStart->nWeek+1];
	for(i =0; i < 8; ++i)
	{
		str.Format(szWeek0[i]);
		m_cbStartWeek.InsertString(i,str);
		if(other==str) sel = i;
	}
	m_cbStartWeek.SetCurSel(sel);

	// start hour
	other.Format("%d",m_pTimeStart->nHours);
	sel=0;
	m_cbStartHour.InsertString(0,"任意");
	for( i = 0; i < 24; ++i)
	{
		str.Format("%d",i);
		m_cbStartHour.InsertString(i+1,str);
		if(other==str) sel = i+1;
	}
	m_cbStartHour.SetCurSel(sel);

	// start minutes
	other.Format("%d",m_pTimeStart->nMinutes);
	sel=0;
	m_cbStartMinute.InsertString(0,"任意");
	for( i = 0; i < 60; ++i)
	{
		str.Format("%d",i);
		m_cbStartMinute.InsertString(i+1,str);
		if(other==str) sel = i + 1;
	}
	m_cbStartMinute.SetCurSel(sel);


	// end year
	if(m_pTimeEnd->nYear == -1)
		sel = 0;
	else
		sel = m_pTimeEnd->nYear - 2010;

	m_cbEndYear.InsertString(0, "任意");

	for(i=1;i<=8;i++)
	{
		str.Format("%d", 2010+i);
		m_cbEndYear.InsertString(i, str);
	}
	m_cbEndYear.SetCurSel(sel);

	// end month
	other.Format("%d", m_pTimeEnd->nMouth + 1);
	sel = 0;
	m_cbEndMonth.InsertString(0,"任意");
	for(i=0;i<12;i++)
	{
		str.Format("%d", i+1);
		m_cbEndMonth.InsertString(i+1, str);
		if(other == str)
			sel = i+1;
	}
	m_cbEndMonth.SetCurSel(sel);

	// end day
	other.Format("%d",m_pTimeEnd->nDay);
	sel=0;
	m_cbEndDay.InsertString(0,"任意");
	for( i = 1; i < 32; ++i)
	{
		str.Format("%d",i);
		m_cbEndDay.InsertString(i,str);
		if(other==str) sel = i;
	}
	m_cbEndDay.SetCurSel(sel);

	// end week
	other = szWeek0[m_pTimeEnd->nWeek+1];
	for(i =0; i < 8; ++i)
	{
		str.Format(szWeek0[i]);
		m_cbEndWeek.InsertString(i,str);
		if(other==str) sel = i;
	}
	m_cbEndWeek.SetCurSel(sel);

	// end hour
	other.Format("%d",m_pTimeEnd->nHours);
	sel=0;
	m_cbEndHour.InsertString(0,"任意");
	for( i = 0; i < 24; ++i)
	{
		str.Format("%d",i);
		m_cbEndHour.InsertString(i+1,str);
		if(other==str) sel = i+1;
	}
	m_cbEndHour.SetCurSel(sel);

	// end minutes
	other.Format("%d",m_pTimeEnd->nMinutes);
	sel=0;
	m_cbEndMinute.InsertString(0,"任意");
	for( i = 0; i < 60; ++i)
	{
		str.Format("%d",i);
		m_cbEndMinute.InsertString(i+1,str);
		if(other==str) sel = i + 1;
	}
	m_cbEndMinute.SetCurSel(sel);
}

void CTimeRange::OnOK() 
{
	CString txt;
	int sel;
	
	// start time
	sel = m_cbStartYear.GetCurSel();
	m_cbStartYear.GetLBText(sel,txt);
	if(txt=="任意") m_pTimeStart->nYear = -1;
	else m_pTimeStart->nYear = (int)atof(txt);

	sel = m_cbStartMonth.GetCurSel();
	m_cbStartMonth.GetLBText(sel,txt);
	if(txt=="任意") m_pTimeStart->nMouth = -1;
	else m_pTimeStart->nMouth = (int)atof(txt) - 1;

	sel = m_cbStartWeek.GetCurSel();
	m_cbStartWeek.GetLBText(sel,txt);
	if(txt=="任意") m_pTimeStart->nWeek = -1;
	else m_pTimeStart->nWeek = sel - 1;

	sel = m_cbStartDay.GetCurSel();
	m_cbStartDay.GetLBText(sel,txt);
	if(txt=="任意") m_pTimeStart->nDay = -1;
	else m_pTimeStart->nDay = (int)atof(txt);

	sel = m_cbStartHour.GetCurSel();
	m_cbStartHour.GetLBText(sel,txt);
	if(txt=="任意") m_pTimeStart->nHours = -1;
	else m_pTimeStart->nHours = (int)atof(txt);

	sel = m_cbStartMinute.GetCurSel();
	m_cbStartMinute.GetLBText(sel,txt);
	if(txt=="任意") m_pTimeStart->nMinutes = -1;
	else m_pTimeStart->nMinutes = (int)atof(txt);

	// end time
	sel = m_cbEndYear.GetCurSel();
	m_cbEndYear.GetLBText(sel,txt);
	if(txt=="任意") m_pTimeEnd->nYear = -1;
	else m_pTimeEnd->nYear = (int)atof(txt);

	sel = m_cbEndMonth.GetCurSel();
	m_cbEndMonth.GetLBText(sel,txt);
	if(txt=="任意") m_pTimeEnd->nMouth = -1;
	else m_pTimeEnd->nMouth = (int)atof(txt) - 1;

	sel = m_cbEndWeek.GetCurSel();
	m_cbEndWeek.GetLBText(sel,txt);
	if(txt=="任意") m_pTimeEnd->nWeek = -1;
	else m_pTimeEnd->nWeek = sel - 1;

	sel = m_cbEndDay.GetCurSel();
	m_cbEndDay.GetLBText(sel,txt);
	if(txt=="任意") m_pTimeEnd->nDay = -1;
	else m_pTimeEnd->nDay = (int)atof(txt);

	sel = m_cbEndHour.GetCurSel();
	m_cbEndHour.GetLBText(sel,txt);
	if(txt=="任意") m_pTimeEnd->nHours = -1;
	else m_pTimeEnd->nHours = (int)atof(txt);

	sel = m_cbEndMinute.GetCurSel();
	m_cbEndMinute.GetLBText(sel,txt);
	if(txt=="任意") m_pTimeEnd->nMinutes = -1;
	else m_pTimeEnd->nMinutes = (int)atof(txt);


	// do some checking....
	if(!IsValidTime())
	{
		MessageBox("时间段格式设置错误！请仔细检查后重新设置。");
		return;
	}
	
	CDialog::OnOK();
}



bool CTimeRange::IsValidTime()
{

	//标识是否存在
	bool year_flag  = false;
	bool month_flag = false;
	bool mday_flag  = false;
	bool wday_flag  = false;
	bool hour_flag  = false;
	bool minute_flag = false;

	//开始时间年不为任意，结束时间的年也不能为任意，月、日、周、小时、分钟情况一致
	if(m_pTimeStart->nYear >= 0)
	{
		if(m_pTimeEnd->nYear < 0 || m_pTimeStart->nYear > m_pTimeEnd->nYear)
			return false;

		year_flag = true;
	}

	//年为任意时，开始月不能小于结束月,其他时间也这样检查
	if(m_pTimeStart->nMouth >= 0)
	{
		if(m_pTimeEnd->nMouth < 0 || (!year_flag && (m_pTimeStart->nMouth > m_pTimeEnd->nMouth)))
			return false;

		month_flag = true;
	}

	if(m_pTimeStart->nDay > 0)
	{
		if(m_pTimeEnd->nDay < 0 || (!month_flag && (m_pTimeStart->nDay > m_pTimeEnd->nDay)))
			return false;

		//检查具体月的天数是否符合逻辑，必须是在月有值的情况
		if(year_flag && month_flag)
		{
			int start_mdays = GetMDay(m_pTimeStart->nYear, m_pTimeStart->nMouth);
			int end_mdays   = GetMDay(m_pTimeEnd->nYear, m_pTimeEnd->nMouth);
			if(start_mdays < m_pTimeStart->nDay || end_mdays < m_pTimeEnd->nDay)
				return false;
		}
		else if(month_flag)
		{
			//没有具体的年时，2月按平年计算
			int start_mdays = GetMDay(2001, m_pTimeStart->nMouth);
			int end_mdays   = GetMDay(2001, m_pTimeEnd->nMouth);
			if(start_mdays < m_pTimeStart->nDay || end_mdays < m_pTimeEnd->nDay)
				return false;
		}

		mday_flag = true;
	}

	if(m_pTimeStart->nWeek >= 0)
	{
		if(m_pTimeEnd->nWeek < 0)
			return false;

		wday_flag = true;
	}

	if(m_pTimeStart->nHours >= 0)
	{
		if(m_pTimeEnd->nHours < 0 || (!mday_flag && !wday_flag && (m_pTimeStart->nHours > m_pTimeEnd->nHours)))
			return false;

		hour_flag = true;
	}

	if(m_pTimeStart->nMinutes >= 0)
	{
		if(m_pTimeEnd->nMinutes < 0 || (!hour_flag && (m_pTimeStart->nMinutes > m_pTimeEnd->nMinutes)))
			return false;

		//极端情况，都有值，而且前面都相等，开始的分钟大于结束的分钟，悲催的程序员！
		if(year_flag && month_flag && (mday_flag || wday_flag) && hour_flag)
		{
			if( (m_pTimeStart->nYear == m_pTimeEnd->nYear)
			 && (m_pTimeStart->nMouth == m_pTimeEnd->nMouth)
			 && (  (mday_flag && (m_pTimeStart->nDay == m_pTimeEnd->nDay)) 
				|| (wday_flag && (m_pTimeStart->nWeek == m_pTimeEnd->nWeek))  )
			 && (m_pTimeStart->nHours == m_pTimeEnd->nHours)
			 && (m_pTimeStart->nMinutes > m_pTimeEnd->nMinutes) )
				return false;
		}

		minute_flag = true;
	}

	if(!year_flag && !month_flag && !mday_flag && !wday_flag && !hour_flag && !minute_flag)
		return false;

	return true;
}
