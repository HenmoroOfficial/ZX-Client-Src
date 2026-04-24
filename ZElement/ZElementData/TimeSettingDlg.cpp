// TimeSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "elementdata.h"
#include "TimeSettingDlg.h"
#include "AAssist.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//=============================================================================
static const char TimeFormat[] = "%02d：%02d：%02d";

BOOL CALLBACK TimeSetting::OnActivate(void)
{
	CTimeSettingDlg dlg;
	dlg.m_nCurrentTime = m_var;
	dlg.m_nFlag = m_nFlag;
	if ( dlg.DoModal() != IDOK ) 
	{
		return FALSE;
	}
	m_var = dlg.m_nCurrentTime;
	return TRUE;
}

LPCTSTR CALLBACK TimeSetting::OnGetShowString(void) const
{
	AString str;
	AVariant var = m_var;
	int nTime = var;
	switch ( m_nFlag )
	{
		case 0:	// 同时具有年月日和小时分秒格式
		{
			if ( nTime )
			{
				CTime tmTime = CTime::GetCurrentTime();	
#ifdef _ANGELICA22
				tm tm1;
				tm tm2;
				tm1 = *tmTime.GetGmtTm(&tm1);
				tm2 = *tmTime.GetLocalTm(&tm2);
#else
				tm tm1 = *tmTime.GetGmtTm();
				tm tm2 = *tmTime.GetLocalTm();
#endif // #ifdef _ANGELICA22
				int nZone = tm2.tm_hour - tm1.tm_hour;
				nTime -= nZone*3600;
				tmTime = nTime;
				str.Format( _T("%d年%d月%d日 %02d：%02d：%02d"), tmTime.GetYear(), tmTime.GetMonth(), tmTime.GetDay(),
							tmTime.GetHour(), tmTime.GetMinute(), tmTime.GetSecond());
			}
			break;
		}
		case 1:	// 只有年月日格式
		{
			if ( nTime )
			{
				CTime tmTime = CTime::GetCurrentTime();	
#ifdef _ANGELICA22
				tm tm1;
				tm tm2;
				tm1 = *tmTime.GetGmtTm(&tm1);
				tm2 = *tmTime.GetLocalTm(&tm2);
#else
				tm tm1 = *tmTime.GetGmtTm();
				tm tm2 = *tmTime.GetLocalTm();
#endif // #ifdef _ANGELICA22
				int nZone = tm2.tm_hour - tm1.tm_hour;
				nTime -= nZone*3600;
				tmTime = nTime;
				str.Format( _T("%d年%d月%d日"),  tmTime.GetYear(), tmTime.GetMonth(), tmTime.GetDay());
			}
			break;
		}
		case 2: // 只有小时分秒格式
		{
			int nHour = nTime / 3600;
			int nMin = (nTime % 3600) / 60;
			int nSecond = (nTime % 3600) % 60;
			str.Format( _T("每日 %2d：%2d：%2d"), nHour, nMin, nSecond );
			break;
		}
		default:
			break;
	}

	return str;
}

AVariant CALLBACK TimeSetting::OnGetValue(void) const
{
	return m_var;
}

void CALLBACK TimeSetting::OnSetValue(const AVariant& var)
{
	m_var = var;
}

//=============================================================================

/////////////////////////////////////////////////////////////////////////////
// CTimeSettingDlg dialog


CTimeSettingDlg::CTimeSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTimeSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTimeSettingDlg)
	m_tmDateTime = 0;
	m_strDayTime = _T("");
	//}}AFX_DATA_INIT
}


void CTimeSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTimeSettingDlg)
	DDX_DateTimeCtrl(pDX, IDC_DATE_TIME, m_tmDateTime);
	DDX_Text(pDX, IDC_DAY_TIME, m_strDayTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTimeSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CTimeSettingDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_DAY_TIME_INC, OnDeltaposDayTimeInc)
	ON_EN_UPDATE(IDC_DAY_TIME, OnUpdateDayTime)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimeSettingDlg message handlers

BOOL CTimeSettingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	((CEdit*)GetDlgItem( IDC_DAY_TIME ))->LimitText(10);	//设置最多能输入的字符个数

	int nTime = m_nCurrentTime;	
	//初始化时，根据flag设置哪些控件隐藏，哪些显示,同时设置相应的初始值
	switch ( m_nFlag )
	{
		case 0:		// 同时具有年月日和小时分秒格式
		{
			if ( nTime )
			{
				CTime tmTime = CTime::GetCurrentTime();	
#ifdef _ANGELICA22
				tm tm1;
				tm tm2;
				tm1 = *tmTime.GetGmtTm(&tm1);
				tm2 = *tmTime.GetLocalTm(&tm2);
#else
				tm tm1 = *tmTime.GetGmtTm();
				tm tm2 = *tmTime.GetLocalTm();
#endif // #ifdef _ANGELICA22
				int nZone = tm2.tm_hour - tm1.tm_hour;
				nTime -= nZone*3600;
				m_tmDateTime = nTime;
				FormatToTimeString( m_strDayTime, m_tmDateTime.GetHour(), m_tmDateTime.GetMinute(), m_tmDateTime.GetSecond() );
			}
			else
			{
				FormatToTimeString( m_strDayTime, 0, 0, 0);
			}
			break;
		}
		case 1:		// 只有年月日格式
		{
			((CWnd*)GetDlgItem( IDC_DAY_TIME ))->ShowWindow(SW_HIDE);
			((CWnd*)GetDlgItem( IDC_DAY_TIME_INC ))->ShowWindow(SW_HIDE);

			if ( nTime )
			{
				CTime tmTime = CTime::GetCurrentTime();	
#ifdef _ANGELICA22
				tm tm1;
				tm tm2;
				tm1 = *tmTime.GetGmtTm(&tm1);
				tm2 = *tmTime.GetLocalTm(&tm2);
#else
				tm tm1 = *tmTime.GetGmtTm();
				tm tm2 = *tmTime.GetLocalTm();
#endif // #ifdef _ANGELICA22
				int nZone = tm2.tm_hour - tm1.tm_hour;
				nTime -= nZone*3600;
				m_tmDateTime = nTime;
			}
			break;
		}
		case 2:		// 只有小时分秒格式
		{
			((CWnd*)GetDlgItem( IDC_DATE_TIME ))->ShowWindow(SW_HIDE);

			int nHour = nTime / 3600;
			int nMin = (nTime % 3600) / 60;
			int nSecond = (nTime % 3600) % 60;
			FormatToTimeString( m_strDayTime, nHour, nMin, nSecond );
			break;
		}
		default:
			break;
	}
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTimeSettingDlg::OnOK() 
{
	//刷新UI
	UpdateData( TRUE );
	m_nCurrentTime = 0;
	switch ( m_nFlag )
	{
		case 0:		// 同时具有年月日和小时分秒格式
		{
			CTime tmTime = CTime::GetCurrentTime();	
#ifdef _ANGELICA22
			tm tm1;
			tm tm2;
			tm1 = *tmTime.GetGmtTm(&tm1);
			tm2 = *tmTime.GetLocalTm(&tm2);
#else
			tm tm1 = *tmTime.GetGmtTm();
			tm tm2 = *tmTime.GetLocalTm();
#endif // #ifdef _ANGELICA22
			int nZone = tm2.tm_hour - tm1.tm_hour;
			int nTimeDelta = m_tmDateTime.GetHour()*3600 + m_tmDateTime.GetMinute()*60 + m_tmDateTime.GetSecond();
			m_nCurrentTime = m_tmDateTime.GetTime() - nTimeDelta + FormatFromTimeString( m_strDayTime );
			m_nCurrentTime += nZone*3600; //加上时区校正
			break;
		}
		case 1:		// 只有年月日格式
		{
			CTime tmTime = CTime::GetCurrentTime();	
#ifdef _ANGELICA22
			tm tm1;
			tm tm2;
			tm1 = *tmTime.GetGmtTm(&tm1);
			tm2 = *tmTime.GetLocalTm(&tm2);
#else
			tm tm1 = *tmTime.GetGmtTm();
			tm tm2 = *tmTime.GetLocalTm();
#endif // #ifdef _ANGELICA22
			int nZone = tm2.tm_hour - tm1.tm_hour;
			int nTimeDelta = m_tmDateTime.GetHour()*3600 + m_tmDateTime.GetMinute()*60 + m_tmDateTime.GetSecond();
			m_nCurrentTime = m_tmDateTime.GetTime() - nTimeDelta;
			m_nCurrentTime += nZone*3600; //加上时区校正
			break;
		}
		case 2:		// 只有小时分秒格式
		{
			m_nCurrentTime = FormatFromTimeString( m_strDayTime );
			break;
		}
		default:
			break;
	}

	CDialog::OnOK();
}

void CTimeSettingDlg::FormatToTimeString( CString& str, int nHour, int nMin, int nSecond )
{
	str.Format( TimeFormat, nHour, nMin, nSecond );
}

int CTimeSettingDlg::FormatFromTimeString( CString str )
{
	int nHour = 0, nMin = 0, nSecond = 0, nTotalTime = 0;
	sscanf( str, TimeFormat, &nHour, &nMin, &nSecond );
	if ( nHour > 23 )
	{
		nHour = 23;
	}
	if ( nHour < 0 )
	{
		nHour = 0;
	}
	if ( nMin > 59 )
	{
		nMin = 59;
	}
	if ( nMin < 0 )
	{
		nMin = 0;
	}
	if ( nSecond > 59 )
	{
		nSecond = 59;
	}
	if ( nSecond < 0 )
	{
		nSecond = 0;
	}

	nTotalTime = nHour*3600 + nMin*60 + nSecond;
	return nTotalTime;
}

void CTimeSettingDlg::OnDeltaposDayTimeInc(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	//处理点击spin后的消息
	UpdateData( TRUE );

	int nHour = 0, nMin = 0, nSecond = 0;
	sscanf( m_strDayTime, TimeFormat, &nHour, &nMin, &nSecond );
	if ( -1 == pNMUpDown->iDelta )	//向上
	{
		nSecond++;
		if ( nSecond > 59 )
		{
			nMin++;
			nSecond = 0;
		}
		if ( nMin > 59 )
		{
			nHour++;
			nMin = 0;
		}
		if ( nHour > 23 )
		{
			nHour = 0;
		}
	}
	else if ( 1 == pNMUpDown->iDelta ) //向下
	{
		nSecond--;
		if ( nSecond < 0 )
		{
			nMin--;
			nSecond = 59;
		}
		if ( nMin < 0 )
		{
			nHour--;
			nMin = 59;
		}
		if ( nHour < 0 )
		{
			nHour = 0;
		}
	}

	//
	m_strDayTime.Format( TimeFormat, nHour, nMin, nSecond );
	UpdateData( FALSE );

	*pResult = 0;
}

void CTimeSettingDlg::OnUpdateDayTime() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	//
	CString strOrg = m_strDayTime;
	UpdateData( TRUE );
	if ( !CheckValidString( m_strDayTime ) )
	{
		m_strDayTime = strOrg;
	}
	UpdateData( FALSE );
	
}

bool CTimeSettingDlg::CheckValidString( const CString& strSource )
{
	int nStrLength = strSource.GetLength();
	int nWStrLength = 2*(nStrLength+1);
	wchar_t* wszStr = new wchar_t[nWStrLength];
	if ( !wszStr )
	{
		return false;
	}
	memset( wszStr, 0, nWStrLength*sizeof(wchar_t) );

	//转换为宽字节
	a_MultiByteToWideChar( strSource, nStrLength, wszStr, nWStrLength );
	int nFormatCharCount = 0, nWStrLen = wcslen( wszStr );
	for ( int i = 0; i < nWStrLen; ++i )
	{
		if ( !iswdigit(wszStr[i]) )
		{
			if ( ( L'：' != wszStr[i] ) )
			{
				delete []wszStr;
				return false;
			}
			else
			{
				nFormatCharCount++;
			}
		}
	}

	if ( nFormatCharCount > 2 )
	{
		delete []wszStr;
		return false;
	}

	//
	delete []wszStr;
	return true;

}
