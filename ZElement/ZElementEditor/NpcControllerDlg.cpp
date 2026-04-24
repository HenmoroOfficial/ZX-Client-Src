// CNpcControllerDlg.cpp : implementation file
//

#include "global.h"
#include "Render.h"
#include "elementeditor.h"
#include "NpcControllerDlg.h"
#include "CommonFileDlg.h"
#include "TimeRange.h"
#include "TimeSegment.h"

#include <A3D.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNpcControllerDlg dialog
const char* szWeek[8] = 
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

const char* szYears[] = 
{
		"任意",
		"2006",
		"2007",
		"2008",
		"2009",
		"2010",
		"2011",
		"2012",
		"2013",
		"2014",
		"2015",
		"2016",
		"2017",
		"2018",
};


void SetDateTime(CTimeSegment::DATETIMESEG* pTime1, CONTROLLER_TIME* pTime2)
{
	pTime1->nYear	= pTime2->nYear;
	pTime1->nMonth	= pTime2->nMouth;
	pTime1->nDay	= pTime2->nDay;
	pTime1->nHour	= pTime2->nHours;
	pTime1->nMin	= pTime2->nMinutes;
	pTime1->nWeek	= pTime2->nWeek;
}

CNpcControllerDlg::CNpcControllerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNpcControllerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNpcControllerDlg)
	m_strID = _T("");
	m_strName = _T("");
	m_nStopTime = 0;
	m_nActiveTime = 0;
	m_bIsInitActive = FALSE;
	m_bTime1Invailid = FALSE;
	m_bTime2Invailid = FALSE;
	m_nControllerID = 0;
	m_nActiveTimeRange = 0;
	m_bIsRepeatActive = FALSE;
	//}}AFX_DATA_INIT
	InitControllerObj(&m_Object);
	m_bReadOnly = false;
}


void CNpcControllerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNpcControllerDlg)
	DDX_Control(pDX, IDC_LIST_TIME_RANGE, m_TimeRangeList);
	DDX_Control(pDX, IDC_COMBO2_YEAR, m_cb2Year);
	DDX_Control(pDX, IDC_COMBO2_WEEK, m_cb2Week);
	DDX_Control(pDX, IDC_COMBO2_MOUTH, m_cb2Mouth);
	DDX_Control(pDX, IDC_COMBO2_MINU, m_cb2Minute);
	DDX_Control(pDX, IDC_COMBO2_HOURS, m_cb2Hours);
	DDX_Control(pDX, IDC_COMBO2_DAY, m_cb2Day);
	DDX_Control(pDX, IDC_COMBO1_YEAR, m_cb1Year);
	DDX_Control(pDX, IDC_COMBO1_WEEK, m_cb1Week);
	DDX_Control(pDX, IDC_COMBO1_MOUTH, m_cb1Mouth);
	DDX_Control(pDX, IDC_COMBO1_HOURS, m_cb1Hours);
	DDX_Control(pDX, IDC_COMBO1_MINU, m_cb1Minute);
	DDX_Control(pDX, IDC_COMBO1_DAY, m_cb1Day);
	DDX_Text(pDX, IDC_EDIT_CONTROLLER_ID, m_strID);
	DDX_Text(pDX, IDC_EDIT_OBJ_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_STOP_TIMES, m_nStopTime);
	DDX_Text(pDX, IDC_EDIT_ACTIVE_TIMES, m_nActiveTime);
	DDX_Check(pDX, IDC_CHECK_ISACTIVEINIT, m_bIsInitActive);
	DDX_Check(pDX, IDC_CHECK1_TIMEINVAILID, m_bTime1Invailid);
	DDX_Check(pDX, IDC_CHECK2_TIMEINVAILID, m_bTime2Invailid);
	DDX_Text(pDX, IDC_EDIT_CONTROLLERID, m_nControllerID);
	DDX_Text(pDX, IDC_EDIT_TIME_RANGE, m_nActiveTimeRange);
	DDX_Check(pDX, IDC_CHECK_ISACTIVE_REPEAT, m_bIsRepeatActive);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNpcControllerDlg, CDialog)
	//{{AFX_MSG_MAP(CNpcControllerDlg)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_CHECK1_TIMEINVAILID, OnCheck1Timeinvailid)
	ON_BN_CLICKED(IDC_CHECK2_TIMEINVAILID, OnCheck2Timeinvailid)
	ON_BN_CLICKED(IDC_TIME_RANGE_EDIT, OnTimeRangeEdit)
	ON_BN_CLICKED(IDC_TIME_RANGE_DEL, OnTimeRangeDel)
	ON_BN_CLICKED(IDC_TIME_RANGE_ADD3, OnTimeRangeAdd3)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TIME_RANGE, OnDblclkListTimeRange)
	ON_BN_CLICKED(IDC_RADIO_UNION, OnRadioUnion)
	ON_BN_CLICKED(IDC_RADIO_AND, OnRadioAnd)
	ON_BN_CLICKED(IDC_TIME_RANGE_CHECK2, OnTimeRangeCheck2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNpcControllerDlg message handlers

void CNpcControllerDlg::OnButtonBrowse() 
{
}

BOOL CNpcControllerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitCombox();
	m_strID.Format("%d",m_Object.id);
	m_strName = m_Object.szName;
	m_nStopTime = m_Object.nStopTime;
	m_nActiveTime = m_Object.nWaitTime;
	m_bIsInitActive = m_Object.bActived;
	m_nControllerID = m_Object.nControllerID;
	m_bTime1Invailid = !m_Object.bActiveTimeInvalid;
	m_bTime2Invailid = !m_Object.bStopTimeInvalid;
	m_nActiveTimeRange = m_Object.nActiveTimeRange;
	m_bIsRepeatActive = m_Object.bRepeatActived;


	m_TimeRangeList.InsertColumn(0, "编号", LVCFMT_LEFT, 40);
	m_TimeRangeList.InsertColumn(1, "起始时间", LVCFMT_LEFT, 260);
	m_TimeRangeList.InsertColumn(2, "终止时间", LVCFMT_LEFT, 260);
	m_TimeRangeList.SetExtendedStyle(m_TimeRangeList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	int nAnd = m_Object.GetTimeSegLogic();
	if(nAnd == 0)
		SetTimeRangeType(false);
	else if(nAnd == 1)
		SetTimeRangeType(true);

	UpdateList();
	
	UpdateData(false);
	
	if(m_bReadOnly)
	{
		CWnd *pWnd = GetDlgItem(IDC_EDIT_OBJ_NAME);
		pWnd->EnableWindow(false);
		pWnd = GetDlgItem(IDC_EDIT_DYNAMIC_OBJECT_PATH);
		pWnd->EnableWindow(false);
		pWnd = GetDlgItem(IDC_EDIT_ID);
		pWnd->EnableWindow(false);

		pWnd = GetDlgItem(IDC_EDIT_ACTIVE_TIMES);
		pWnd->EnableWindow(false);

		pWnd = GetDlgItem(IDC_EDIT_STOP_TIMES);
		pWnd->EnableWindow(false);

		pWnd = GetDlgItem(IDC_CHECK1_TIMEINVAILID);
		pWnd->EnableWindow(false);

		pWnd = GetDlgItem(IDC_CHECK2_TIMEINVAILID);
		pWnd->EnableWindow(false);
		pWnd = GetDlgItem(IDC_CHECK_ISACTIVEINIT);
		pWnd->EnableWindow(false);
		pWnd = GetDlgItem(IDC_CHECK_ISACTIVE_REPEAT);
		pWnd->EnableWindow(false);
		pWnd = GetDlgItem(IDC_EDIT_CONTROLLERID);
		pWnd->EnableWindow(false);

		pWnd = GetDlgItem(IDC_EDIT_TIME_RANGE);
		pWnd->EnableWindow(false);

		EnableCombox1(false);
		EnableCombox2(false);
		EnableRadioButton(false);
	}else
	{
		if(m_bTime1Invailid) 
		{
			EnableCombox1(true);
			CWnd *pWnd = GetDlgItem(IDC_EDIT_TIME_RANGE);
			pWnd->EnableWindow(true);
		}else 
		{
			EnableCombox1(false);
			CWnd *pWnd = GetDlgItem(IDC_EDIT_TIME_RANGE);
			pWnd->EnableWindow(false);
		}
		if(m_bTime2Invailid) EnableCombox2(true);
		else EnableCombox2(false);
	}

	if(m_Object.GetTimeSegNum() > 0)
	{
		m_Object.bActived = false;

		CButton* pWnd = (CButton *)GetDlgItem(IDC_CHECK_ISACTIVEINIT);
		pWnd->SetCheck(false);
		pWnd->EnableWindow(false);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNpcControllerDlg::EnableCombox1(bool bEnable)
{
	
	CWnd* pWnd = GetDlgItem(IDC_COMBO1_YEAR);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_COMBO1_MOUTH);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_COMBO1_WEEK);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_COMBO1_DAY);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_COMBO1_HOURS);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_COMBO1_MINU);
	pWnd->EnableWindow(bEnable);
}

void CNpcControllerDlg::EnableCombox2(bool bEnable)
{
	CWnd* pWnd = GetDlgItem(IDC_COMBO2_YEAR);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_COMBO2_MOUTH);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_COMBO2_WEEK);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_COMBO2_DAY);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_COMBO2_HOURS);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_COMBO2_MINU);
	pWnd->EnableWindow(bEnable);
}

void CNpcControllerDlg::EnableRadioButton(bool bEnable)
{
	CWnd* pWnd = GetDlgItem(IDC_RADIO_UNION);
	pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_RADIO_AND);
	pWnd->EnableWindow(bEnable);
}

void CNpcControllerDlg::UpdateList()
{
	m_TimeRangeList.DeleteAllItems();

	for(int i=0; i< m_Object.GetTimeSegNum(); i++)
	{
		CONTROLLER_TIME timeStart = m_Object.aTimeSeg[i*2];
		CONTROLLER_TIME timeEnd = m_Object.aTimeSeg[i*2+1];
		CString str;
		int nCol = m_TimeRangeList.GetItemCount();
		str.Format("%d", nCol+1);

		int nItem = m_TimeRangeList.InsertItem(nCol,str);		

		// start time
		str.Format("%d年%d月%d日, %d时%d分, 周:%s", timeStart.nYear, (timeStart.nMouth<0)?-1:timeStart.nMouth+1, timeStart.nDay, timeStart.nHours, timeStart.nMinutes, CString(szWeek[timeStart.nWeek + 1]));
		str.Replace("-1", "任意");
		m_TimeRangeList.SetItemText(nItem, 1, str);
		

		str.Format("%d年%d月%d日, %d时%d分, 周:%s", timeEnd.nYear, (timeEnd.nMouth<0)?-1:timeEnd.nMouth+1, timeEnd.nDay, timeEnd.nHours, timeEnd.nMinutes, CString(szWeek[timeEnd.nWeek + 1]));
		str.Replace("-1", "任意");

		m_TimeRangeList.SetItemText(nItem, 2, str);
	}

}

void CNpcControllerDlg::InitCombox()
{
	CString str,other;
	int sel = 0;
	
	//2
	if(m_Object.mStopTime.nYear==-1) sel = 0;
	else sel = m_Object.mStopTime.nYear - 2005;

	int y;
	for(y = 0; y < sizeof(szYears)/4; ++y)
		m_cb2Year.InsertString(y,szYears[y]);
	m_cb2Year.SetCurSel(sel);
	
	other = szWeek[m_Object.mStopTime.nWeek+1];
	int i;
	for( i =0; i < 8; ++i)
	{
		str.Format(szWeek[i]);
		m_cb2Week.InsertString(i,str);
		if(other==str) sel = i;
	}
	m_cb2Week.SetCurSel(sel);
	
	other.Format("%d",m_Object.mStopTime.nMouth + 1);
	sel=0;
	m_cb2Mouth.InsertString(0,"任意");
	for( i = 0; i < 12; ++i)
	{
		str.Format("%d",i+1);
		m_cb2Mouth.InsertString(i+1,str);
		if(other==str) sel = i+1;
	}
	m_cb2Mouth.SetCurSel(sel);
	
	other.Format("%d",m_Object.mStopTime.nMinutes);
	sel=0;
	for( i = 0; i < 60; ++i)
	{
		str.Format("%d",i);
		m_cb2Minute.InsertString(i,str);
		if(other==str) sel = i;
	}
	m_cb2Minute.SetCurSel(sel);

	other.Format("%d",m_Object.mStopTime.nHours);
	sel=0;
	m_cb2Hours.InsertString(0,"任意");
	for( i = 0; i < 24; ++i)
	{
		str.Format("%d",i);
		m_cb2Hours.InsertString(i+1,str);
		if(other==str) sel = i+1;
	}
	m_cb2Hours.SetCurSel(sel);
	
	other.Format("%d",m_Object.mStopTime.nDay);
	sel=0;
	m_cb2Day.InsertString(0,"任意");
	for( i = 1; i < 32; ++i)
	{
		str.Format("%d",i);
		m_cb2Day.InsertString(i,str);
		if(other==str) sel = i;
	}
	m_cb2Day.SetCurSel(sel);
	//1

	if(m_Object.mActiveTime.nYear==-1) sel = 0;
	else sel = m_Object.mActiveTime.nYear - 2005;
	
	for(y = 0; y < sizeof(szYears)/4; ++y)
		m_cb1Year.InsertString(y,szYears[y]);
	m_cb1Year.SetCurSel(sel);

	other = szWeek[m_Object.mActiveTime.nWeek+1];
	for(i =0; i < 8; ++i)
	{
		str.Format(szWeek[i]);
		m_cb1Week.InsertString(i,str);
		if(other==str) sel = i;
	}
	m_cb1Week.SetCurSel(sel);
	
	other.Format("%d",m_Object.mActiveTime.nMouth + 1);
	sel=0;
	m_cb1Mouth.InsertString(0,"任意");
	for( i = 0; i < 12; ++i)
	{
		str.Format("%d",i+1);
		m_cb1Mouth.InsertString(i+1,str);
		if(other==str) sel = i+1;
	}
	m_cb1Mouth.SetCurSel(sel);
	
	other.Format("%d",m_Object.mActiveTime.nMinutes);
	sel=0;
	for( i = 0; i < 60; ++i)
	{
		str.Format("%d",i);
		m_cb1Minute.InsertString(i,str);
		if(other==str) sel = i;
	}
	m_cb1Minute.SetCurSel(sel);

	
	other.Format("%d",m_Object.mActiveTime.nHours);
	sel=0;
	m_cb1Hours.InsertString(0,"任意");
	for( i = 0; i < 24; ++i)
	{
		str.Format("%d",i);
		m_cb1Hours.InsertString(i+1,str);
		if(other==str) sel = i+1;
	}
	m_cb1Hours.SetCurSel(sel);
	
	other.Format("%d",m_Object.mActiveTime.nDay);
	sel=0;
	m_cb1Day.InsertString(0,"任意");
	for( i = 1; i < 32; ++i)
	{
		str.Format("%d",i);
		m_cb1Day.InsertString(i,str);
		if(other==str) sel = i;
	}
	m_cb1Day.SetCurSel(sel);
	
}

void CNpcControllerDlg::OnOK()
{
	UpdateData(true);
	
	m_Object.id = (unsigned int)atof(m_strID);
	strcpy(m_Object.szName, m_strName);
	
	
	m_Object.nStopTime			= m_nStopTime;
	m_Object.nWaitTime			= m_nActiveTime;
	m_Object.bActived			= m_bIsInitActive;
	m_Object.bRepeatActived     = m_bIsRepeatActive;
	m_Object.bActiveTimeInvalid = !m_bTime1Invailid;
	m_Object.bStopTimeInvalid	= !m_bTime2Invailid;
	m_Object.nControllerID      = m_nControllerID;
	m_Object.nActiveTimeRange   = m_nActiveTimeRange;

	CString txt;
	int sel = m_cb1Year.GetCurSel();
	m_cb1Year.GetLBText(sel,txt);
	if(txt=="任意") m_Object.mActiveTime.nYear = -1;
	else m_Object.mActiveTime.nYear = (int)atof(txt);

	sel = m_cb1Mouth.GetCurSel();
	m_cb1Mouth.GetLBText(sel,txt);
	if(txt=="任意") m_Object.mActiveTime.nMouth = -1;
	else m_Object.mActiveTime.nMouth = (int)atof(txt) - 1;

	sel = m_cb1Week.GetCurSel();
	m_cb1Week.GetLBText(sel,txt);
	if(txt=="任意") m_Object.mActiveTime.nWeek = -1;
	else m_Object.mActiveTime.nWeek = sel - 1;

	sel = m_cb1Day.GetCurSel();
	m_cb1Day.GetLBText(sel,txt);
	if(txt=="任意") m_Object.mActiveTime.nDay = -1;
	else m_Object.mActiveTime.nDay = (int)atof(txt);

	sel = m_cb1Hours.GetCurSel();
	m_cb1Hours.GetLBText(sel,txt);
	if(txt=="任意") m_Object.mActiveTime.nHours = -1;
	else m_Object.mActiveTime.nHours = (int)atof(txt);

	sel = m_cb1Minute.GetCurSel();
	m_cb1Minute.GetLBText(sel,txt);
	m_Object.mActiveTime.nMinutes = (int)atof(txt);

	//2
	sel = m_cb2Year.GetCurSel();
	m_cb2Year.GetLBText(sel,txt);
	if(txt=="任意") m_Object.mStopTime.nYear = -1;
	else m_Object.mStopTime.nYear = (int)atof(txt);

	sel = m_cb2Mouth.GetCurSel();
	m_cb2Mouth.GetLBText(sel,txt);
	if(txt=="任意") m_Object.mStopTime.nMouth = -1;
	else m_Object.mStopTime.nMouth = (int)atof(txt) - 1;

	sel = m_cb2Week.GetCurSel();
	m_cb2Week.GetLBText(sel,txt);
	if(txt=="任意") m_Object.mStopTime.nWeek = -1;
	else m_Object.mStopTime.nWeek = sel-1;

	sel = m_cb2Day.GetCurSel();
	m_cb2Day.GetLBText(sel,txt);
	if(txt=="任意") m_Object.mStopTime.nDay = -1;
	else m_Object.mStopTime.nDay = (int)atof(txt);

	sel = m_cb2Hours.GetCurSel();
	m_cb2Hours.GetLBText(sel,txt);
	if(txt=="任意") m_Object.mStopTime.nHours = -1;
	else m_Object.mStopTime.nHours = (int)atof(txt);

	sel = m_cb2Minute.GetCurSel();
	m_cb2Minute.GetLBText(sel,txt);
	m_Object.mStopTime.nMinutes = (int)atof(txt);

	if(m_Object.mStopTime.nWeek !=-1 && m_Object.mStopTime.nDay !=-1)
	{
		MessageBox("在停止激活时间中，周的天和月的天必须至少有一个是任意!");
		return;
	}

	if(m_Object.mActiveTime.nWeek !=-1 && m_Object.mActiveTime.nDay !=-1)
	{
		MessageBox("在激活时间中，周的天和月的天必须至少有一个是任意!");
		return;
	}

	if(m_Object.aTimeSeg.size() != 0 )
	{
		CTimeSegment * ptimeseg = CreateTimeSegCheck();

		abase::vector<CTimeSegment::DATETIMESEG> atimeseg;
		for(int i=0;i<m_Object.aTimeSeg.size(); i+=2)
		{
			CTimeSegment::DATETIMESEG pTime1;
			SetDateTime(&pTime1, &m_Object.aTimeSeg[i]);
			atimeseg.push_back(pTime1);

			SetDateTime(&pTime1, &m_Object.aTimeSeg[i+1]);
			atimeseg.push_back(pTime1);
		}

		int nRet = ptimeseg->CheckTimeSegAvailalbe(atimeseg, (m_Object.GetTimeSegLogic() == 1));
		if(nRet != 0)
		{
			CString strError;
			strError.Format("时间段范围配置错误，请检查编号为%d的时间段！", nRet);

			AfxMessageBox(strError);

			return;
		}
	}

	CDialog::OnOK();
}

void CNpcControllerDlg::OnCheck1Timeinvailid() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	if(m_bTime1Invailid) 
	{
		EnableCombox1(true);
		CWnd *pWnd = GetDlgItem(IDC_EDIT_TIME_RANGE);
		pWnd->EnableWindow(true);
	}
	else
	{
		EnableCombox1(false);
		CWnd *pWnd = GetDlgItem(IDC_EDIT_TIME_RANGE);
		pWnd->EnableWindow(false);
	}
	
}

void CNpcControllerDlg::OnCheck2Timeinvailid() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	if(m_bTime2Invailid) EnableCombox2(true);
	else EnableCombox2(false);
}

void CNpcControllerDlg::OnTimeRangeEdit() 
{
	POSITION pos = m_TimeRangeList.GetFirstSelectedItemPosition();
	if(pos == NULL)
		return;
	else
	{
 		int nItem = m_TimeRangeList.GetNextSelectedItem(pos);
	
		CONTROLLER_TIME timeStart = m_Object.aTimeSeg[nItem*2];
		CONTROLLER_TIME timeEnd = m_Object.aTimeSeg[nItem*2+1];

		CTimeRange dlg;
		dlg.m_pTimeStart = &timeStart;
		dlg.m_pTimeEnd = &timeEnd;
		dlg.m_bReadOnly = m_bReadOnly;
		if(dlg.DoModal() == IDOK)
		{
			if(m_bReadOnly)
				return;

			// 检查！
			CTimeSegment * ptimeseg = CreateTimeSegCheck();
			CTimeSegment::DATETIMESEG start_date;
			CTimeSegment::DATETIMESEG end_date;

			SetDateTime(&start_date, &timeStart);
			SetDateTime(&end_date, &timeEnd);


			// 检查日期
			CHECK_TIMESEG checkMsg = ptimeseg->CheckValidity(start_date, end_date);
			if(checkMsg == TS_SUCCESS)
			{
				m_Object.aTimeSeg[nItem*2] = timeStart;
				m_Object.aTimeSeg[nItem*2+1] = timeEnd;

				UpdateList();

				m_Object.bActived = false;
				CButton* pWnd = (CButton *)GetDlgItem(IDC_CHECK_ISACTIVEINIT);
				pWnd->SetCheck(false);
				pWnd->EnableWindow(false);
			}
			else
			{
				// 错误提示，无法修改
				CString errorMsg;
				switch(checkMsg)
				{
				case TS_ERR_YEAR:
					errorMsg = "年份设置有问题！";
					break;

				case TS_ERR_MONTH:
					errorMsg = "月份设置有问题！";
					break;

				case TS_ERR_MONTHDAY:
					errorMsg = "日设置有问题！";
					break;

				case TS_ERR_HOUR:
					errorMsg = "小时设置有问题！";
					break;

				case TS_ERR_MIN:
					errorMsg = "分钟设置有问题！";
					break;

				case TS_ERR_WEEKDAY:
					errorMsg = "周设置有问题！";
					break;

				default:
					break;
				}

				AfxMessageBox(errorMsg);
			}

		}
	}
}

void CNpcControllerDlg::OnTimeRangeDel() 
{
	if(m_bReadOnly)
		return;

	POSITION pos = m_TimeRangeList.GetFirstSelectedItemPosition();
	if(pos == NULL)
		return;
	else
	{
		CString msg;
		msg.Format("你确定要删除选中的时间段吗？");
		if(IDYES!=AfxMessageBox(msg,MB_YESNO|MB_ICONQUESTION))
			return;

 		int nItem = m_TimeRangeList.GetNextSelectedItem(pos);
		m_Object.aTimeSeg.erase(&m_Object.aTimeSeg[nItem*2+1]);
		m_Object.aTimeSeg.erase(&m_Object.aTimeSeg[nItem*2]);

		m_Object.nTimeSegNum--;

		UpdateList();

		if(m_Object.GetTimeSegNum() == 0)
		{
			m_Object.bActived = false;

			CButton* pWnd = (CButton *)GetDlgItem(IDC_CHECK_ISACTIVEINIT);
			pWnd->SetCheck(false);
			pWnd->EnableWindow(true);
		}
	}	
}

void CNpcControllerDlg::OnTimeRangeAdd3() 
{
	if(m_bReadOnly)
		return;

	CONTROLLER_TIME timeStart;
	CONTROLLER_TIME timeEnd;

	CTimeRange dlg;
	dlg.m_pTimeStart = &timeStart;
	dlg.m_pTimeEnd = &timeEnd;
	dlg.m_bReadOnly = m_bReadOnly;
	if(dlg.DoModal() == IDOK)
	{
		// 检查！
		CTimeSegment * ptimeseg = CreateTimeSegCheck();
		CTimeSegment::DATETIMESEG start_date;
		CTimeSegment::DATETIMESEG end_date;

		SetDateTime(&start_date, &timeStart);
		SetDateTime(&end_date, &timeEnd);


		// 检查日期
		CHECK_TIMESEG checkMsg = ptimeseg->CheckValidity(start_date, end_date);
		if(checkMsg == TS_SUCCESS)
		{
			m_Object.aTimeSeg.push_back(timeStart);
			m_Object.aTimeSeg.push_back(timeEnd);

			m_Object.nTimeSegNum++;

			UpdateList();

			m_Object.bActived = false;
			CButton* pWnd = (CButton *)GetDlgItem(IDC_CHECK_ISACTIVEINIT);
			pWnd->SetCheck(false);
			pWnd->EnableWindow(false);
		}
		else
		{
			// 错误提示，无法添加
			CString errorMsg;
			switch(checkMsg)
			{
			case TS_ERR_YEAR:
				errorMsg = "年份设置有问题！";
				break;

			case TS_ERR_MONTH:
				errorMsg = "月份设置有问题！";
				break;

			case TS_ERR_MONTHDAY:
				errorMsg = "日设置有问题！";
				break;

			case TS_ERR_HOUR:
				errorMsg = "小时设置有问题！";
				break;

			case TS_ERR_MIN:
				errorMsg = "分钟设置有问题！";
				break;

			case TS_ERR_WEEKDAY:
				errorMsg = "周设置有问题！";
				break;

			default:
				break;
			}

			AfxMessageBox(errorMsg);
		}
	}
}

void CNpcControllerDlg::OnDblclkListTimeRange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnTimeRangeEdit();

	*pResult = 0;
}

void CNpcControllerDlg::OnRadioUnion() 
{
	SetTimeRangeType(false);	
}

void CNpcControllerDlg::OnRadioAnd() 
{
	SetTimeRangeType(true);	
}

void  CNpcControllerDlg::SetTimeRangeType(bool bAnd)
{
	CButton *pButton = (CButton *)GetDlgItem(IDC_RADIO_UNION);
	if(pButton) 
		pButton->SetCheck(!bAnd);
	pButton = (CButton *)GetDlgItem(IDC_RADIO_AND);
	if(pButton)
		pButton->SetCheck(bAnd);

	m_Object.SetTimeSegLogic(bAnd?true:false);
}

void CNpcControllerDlg::OnTimeRangeCheck2() 
{
	// TODO: Add your control notification handler code here
	if(m_Object.aTimeSeg.size() ==0 )
		return;

	CTimeSegment * ptimeseg = CreateTimeSegCheck();

	abase::vector<CTimeSegment::DATETIMESEG> atimeseg;
	for(int i=0;i<m_Object.aTimeSeg.size(); i+=2)
	{
		CTimeSegment::DATETIMESEG pTime1;
		SetDateTime(&pTime1, &m_Object.aTimeSeg[i]);
		atimeseg.push_back(pTime1);

		SetDateTime(&pTime1, &m_Object.aTimeSeg[i+1]);
		atimeseg.push_back(pTime1);
	}

	int nRet = ptimeseg->CheckTimeSegAvailalbe(atimeseg, (m_Object.GetTimeSegLogic() == 1));
	if(nRet == 0)
	{
		ptimeseg->AnalogueCalculation(atimeseg, (m_Object.GetTimeSegLogic() == 1));
		ptimeseg->OpenAnalogueFile();
	}
	else
	{
		CString strError;
		strError.Format("时间段范围配置错误，请检查编号为%d的时间段！", nRet);

		AfxMessageBox(strError);
	}
	
}
