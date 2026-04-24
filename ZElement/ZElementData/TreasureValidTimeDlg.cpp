// TreasureValidTimeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "elementdata.h"
#include "TreasureValidTimeDlg.h"
#include "AAssist.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreasureValidTimeDlg dialog

//-----------------------------------------------------------------------------
static const char TimeFormat[] = "%02d：%02d";
//-----------------------------------------------------------------------------

CTreasureValidTimeDlg::CTreasureValidTimeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTreasureValidTimeDlg::IDD, pParent), m_nParam(0), m_nStartTime(0), m_nEndTime(0)
{
	//{{AFX_DATA_INIT(CTreasureValidTimeDlg)
	m_nType = -1;
	m_bStartTimeForTimeInterval = FALSE;
	m_bEndTimeForTimeInterval = FALSE;
	m_tmEndTimeForTimeInterval = 0;
	m_tmBeginTimeForTimeInterval = 0;
	m_strBeginTimeForTimeInterval = _T("");
	m_strEndTimeForTimeInterval = _T("");
	m_strBeginTimeForWeek = _T("");
	m_strEndTimeForWeek = _T("");
	m_strBeginTimeForMonth = _T("");
	m_strEndTimeForMonth = _T("");
	//}}AFX_DATA_INIT
}


void CTreasureValidTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTreasureValidTimeDlg)
	DDX_Radio(pDX, IDC_RADIO1, m_nType);
	DDX_Check(pDX, IDC_CHECK1, m_bStartTimeForTimeInterval);
	DDX_Check(pDX, IDC_CHECK2, m_bEndTimeForTimeInterval);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_tmEndTimeForTimeInterval);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_tmBeginTimeForTimeInterval);
	DDX_Text(pDX, IDC_EDIT1, m_strBeginTimeForTimeInterval);
	DDX_Text(pDX, IDC_EDIT2, m_strEndTimeForTimeInterval);
	DDX_Text(pDX, IDC_EDIT3, m_strBeginTimeForWeek);
	DDX_Text(pDX, IDC_EDIT4, m_strEndTimeForWeek);
	DDX_Text(pDX, IDC_EDIT5, m_strBeginTimeForMonth);
	DDX_Text(pDX, IDC_EDIT6, m_strEndTimeForMonth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTreasureValidTimeDlg, CDialog)
	//{{AFX_MSG_MAP(CTreasureValidTimeDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_START_TIME_FOR_TIME_INTERVAL, OnDeltaposSpinStartTimeForTimeInterval)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_END_TIME_FOR_TIME_INTERVAL, OnDeltaposSpinEndTimeForTimeInterval)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_START_TIME_FOR_WEEK, OnDeltaposSpinStartTimeForWeek)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_END_TIME_FOR_WEEK, OnDeltaposSpinEndTimeForWeek)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_START_TIME_FOR_MONTH, OnDeltaposSpinStartTimeForMonth)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_END_TIME_FOR_MONTH, OnDeltaposSpinEndTimeForMonth)
	ON_EN_UPDATE(IDC_EDIT1, OnUpdateEditForTimeInterval)
	ON_EN_UPDATE(IDC_EDIT2, OnUpdateEdit2)
	ON_EN_UPDATE(IDC_EDIT3, OnUpdateEdit3)
	ON_EN_UPDATE(IDC_EDIT4, OnUpdateEdit4)
	ON_EN_UPDATE(IDC_EDIT5, OnUpdateEdit5)
	ON_EN_UPDATE(IDC_EDIT6, OnUpdateEdit6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreasureValidTimeDlg message handlers

void CTreasureValidTimeDlg::OnOK() 
{
	//刷新数据
	UpdateData();
	m_nParam = 0; //清除之前的值
	switch ( m_nType )
	{
		case TYPE_TIMEINTERVAL:	//时间段内有效
		{
			//处理开始时间和结束时间的掩码
			if ( m_bStartTimeForTimeInterval )
			{
				m_nParam |= 0x00000001;
				//更新开始时间
				int nTimeDelta = m_tmBeginTimeForTimeInterval.GetHour()*3600 + m_tmBeginTimeForTimeInterval.GetMinute()*60 + m_tmBeginTimeForTimeInterval.GetSecond();
				m_nStartTime = m_tmBeginTimeForTimeInterval.GetTime() - nTimeDelta + GetTimeValue( m_strBeginTimeForTimeInterval );
			}
			if ( m_bEndTimeForTimeInterval )
			{
				m_nParam |= 0x00000002;
				//更新结束时间
				int nTimeDelta = m_tmEndTimeForTimeInterval.GetHour()*3600 + m_tmEndTimeForTimeInterval.GetMinute()*60 + m_tmEndTimeForTimeInterval.GetSecond();
				m_nEndTime = m_tmEndTimeForTimeInterval.GetTime()  - nTimeDelta + GetTimeValue( m_strEndTimeForTimeInterval );
			}
			break;
		}
		case TYPE_WEEK: //每周有效
		{
			for ( int i = 0; i < 7; ++i )
			{
				CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK3 + i);
				if ( pButton && (pButton->GetCheck() == BST_CHECKED) )
				{
					if ( i != 6 )
					{
						m_nParam |= (1 << (i+1));//周一：2；周二：4；...周日：1
					}
					else
					{
						m_nParam |= 0x00000001;
					}
				}
			}
			//更新开始时间和结束时间
			m_nStartTime = GetTimeValue( m_strBeginTimeForWeek );
			m_nEndTime = GetTimeValue( m_strEndTimeForWeek );
			break;
		}
		case TYPE_MONTH: //每月有效
		{
			for ( int i = 0; i < 31; ++i )
			{
				CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK10 + i);
				if ( pButton && (pButton->GetCheck() == BST_CHECKED) )
				{
					m_nParam |= (1 << (i+1));
				}
			}
			//更新开始时间和结束时间
			m_nStartTime = GetTimeValue( m_strBeginTimeForMonth );
			m_nEndTime = GetTimeValue( m_strEndTimeForMonth );
			break;
		}
		default:
			break;
	}

	CDialog::OnOK();
}

BOOL CTreasureValidTimeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//将初始值设置到UI控件中
	if ( m_nType < 0 || m_nType >= 4 )
	{
		m_nType = TYPE_NOLIMIT;//无效的操作类型
	}
	
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO1 + m_nType);
	if ( pButton )
	{
		pButton->SetCheck( BST_CHECKED );
	}

	//无论如何，先初始化这些格式化显示
	m_strBeginTimeForTimeInterval.Format( TimeFormat, 0, 0 );
	m_strEndTimeForTimeInterval.Format( TimeFormat, 0, 0 );
	m_strBeginTimeForWeek.Format( TimeFormat, 0, 0 );
	m_strEndTimeForWeek.Format( TimeFormat, 0, 0 );
	m_strBeginTimeForMonth.Format( TimeFormat, 0, 0 );
	m_strEndTimeForMonth.Format( TimeFormat, 0, 0 );
	//设置edit的字数最大长度
	for ( int i = 0; i < 6; ++i )
	{
		((CEdit*)GetDlgItem(IDC_EDIT1 + i))->LimitText(6);
	}

	//
	switch ( m_nType )
	{
		case TYPE_TIMEINTERVAL:	//按时间段内有效
		{
			if ( m_nParam & 0x00000001 )
			{
				m_bStartTimeForTimeInterval = TRUE;
				//初始化开始时间
				m_tmBeginTimeForTimeInterval	= m_nStartTime;
				m_strBeginTimeForTimeInterval.Format( TimeFormat, m_tmBeginTimeForTimeInterval.GetHour(), m_tmBeginTimeForTimeInterval.GetMinute() );
			}
			if ( m_nParam & 0x00000002 )
			{
				m_bEndTimeForTimeInterval = TRUE;
				//初始化结束时间
				m_tmEndTimeForTimeInterval		= m_nEndTime;
				m_strEndTimeForTimeInterval.Format( TimeFormat, m_tmEndTimeForTimeInterval.GetHour(), m_tmEndTimeForTimeInterval.GetMinute() );
			}
			break;
		}
		case TYPE_WEEK:	//按每周内有效
		{
			//初始化周的掩码
			for ( int i = 0; i < 6; ++i )
			{
				if ( m_nParam & (1 << (i + 1)) )
				{
					CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK3 + i);
					if ( pButton )
					{
						pButton->SetCheck( BST_CHECKED );
					}
				}
			}
			if ( m_nParam & 0x00000001 )
			{
				CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK3 + 6);
				if ( pButton )
				{
					pButton->SetCheck( BST_CHECKED );
				}
			}
			//初始化开启和结束时间
			int nBeginHour = m_nStartTime / 3600;
			int nBeginMin  = (m_nStartTime % 3600) / 60;
			int nEndHour   = m_nEndTime / 3600;
			int nEndMin	   = (m_nEndTime % 3600) / 60;
			m_strBeginTimeForWeek.Format( TimeFormat, nBeginHour, nBeginMin );
			m_strEndTimeForWeek.Format( TimeFormat, nEndHour, nEndMin );
			break;
		}
		case TYPE_MONTH:	//按每月内有效
		{
			//初始化月的掩码
			for ( int i = 0; i < 31; ++i )
			{
				if ( m_nParam & (1 << (i + 1)) )
				{
					CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK10 + i);
					if ( pButton )
					{
						pButton->SetCheck( BST_CHECKED );
					}
				}
			}
			//初始化开启和结束时间
			int nBeginHour = m_nStartTime / 3600;
			int nBeginMin  = (m_nStartTime % 3600) / 60;
			int nEndHour   = m_nEndTime / 3600;
			int nEndMin	   = (m_nEndTime % 3600) / 60;
			m_strBeginTimeForMonth.Format( TimeFormat, nBeginHour, nBeginMin );
			m_strEndTimeForMonth.Format( TimeFormat, nEndHour, nEndMin );
			break;
		}
	}
	UpdateData(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTreasureValidTimeDlg::OnDeltaposSpinStartTimeForTimeInterval(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	//处理在点击spin后的消息
	UpdateFormatString( m_strBeginTimeForTimeInterval, pNMUpDown->iDelta );

	*pResult = 0;
}

void CTreasureValidTimeDlg::UpdateFormatString( CString& strTime, int nFlag )
{
	UpdateData( TRUE );

	int nHour = 0, nMin = 0;
	sscanf( strTime, TimeFormat, &nHour, &nMin );
	if ( -1 == nFlag )//向上
	{
		if ( nHour == 23 && nMin == 59 )
		{
			return;
		}
		nMin++;
		if ( nMin > 59 )
		{
			nMin = 0;
			nHour++;
			if ( nHour > 23 )
			{
				nHour = 23;
			}
		}
	}
	else if ( 1 == nFlag )//向下
	{
		if ( nHour == 0 && nMin == 0 )
		{
			return;
		}
		if ( nMin == 0 )
		{
			nHour--;
			if ( nHour < 0 )
			{
				nHour = 0;
			}
			nMin = 59;
		}
		else
		{
			nMin--;
		}
	}

	//
	strTime.Format( TimeFormat, nHour, nMin );
	UpdateData(FALSE);
}



void CTreasureValidTimeDlg::OnDeltaposSpinEndTimeForTimeInterval(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	//处理在点击spin后的消息
	UpdateFormatString( m_strEndTimeForTimeInterval, pNMUpDown->iDelta );

	*pResult = 0;
}

void CTreasureValidTimeDlg::OnDeltaposSpinStartTimeForWeek(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	//处理在点击spin后的消息
	UpdateFormatString( m_strBeginTimeForWeek, pNMUpDown->iDelta );
	
	*pResult = 0;
}

void CTreasureValidTimeDlg::OnDeltaposSpinEndTimeForWeek(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	//处理在点击spin后的消息
	UpdateFormatString( m_strEndTimeForWeek, pNMUpDown->iDelta );	

	*pResult = 0;
}

void CTreasureValidTimeDlg::OnDeltaposSpinStartTimeForMonth(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	//处理在点击spin后的消息
	UpdateFormatString( m_strBeginTimeForMonth, pNMUpDown->iDelta );		

	*pResult = 0;
}

void CTreasureValidTimeDlg::OnDeltaposSpinEndTimeForMonth(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	//处理在点击spin后的消息
	UpdateFormatString( m_strEndTimeForMonth, pNMUpDown->iDelta );	

	*pResult = 0;
}

int CTreasureValidTimeDlg::GetTimeValue( CString strTime )
{
	int nHour = 0, nMin = 0, nTotalTime = 0;
	sscanf( strTime, TimeFormat, &nHour, &nMin );
	nTotalTime = nHour*3600 + nMin*60;
	return nTotalTime;
}

void CTreasureValidTimeDlg::OnUpdateEditForTimeInterval() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
/*	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
	if ( !pEdit )
	{
		return;
	}
	int nOldPos = LOWORD(pEdit->GetSel());
*/
	UpdateEditData( m_strBeginTimeForTimeInterval );
	
}

CString	CTreasureValidTimeDlg::GetDigitString( const CString& strSource, int nStart, int nSize )
{
	CString str = "";
	int nStrLength = strSource.GetLength();
	if ( (nStart < 0) || (nSize <= 0) || (nStart >= nStrLength) )
	{
		return str;
	}

	//计算正确字符串长度
	if ( (nStart + nSize) > nStrLength )
	{
		nSize = nStrLength - nStart;
	}

	//获得合法的数字字符串
	int nCount = 0;
	int nEndPos = nStart + nSize;
	for ( int i = nStart; (i < nEndPos && nCount < 2); ++i )
	{
		if ( isdigit(strSource[i]) )
		{
			str += strSource[i];
			nCount++;
		}
	}

	//
	return str;
}

void CTreasureValidTimeDlg::UpdateTimeString( CString& strTime )
{
	int nHour = 0, nMin = 0;
	CString strValue = GetDigitString( strTime, 0, 2 );//计算小时
	sscanf( strValue, "%d", &nHour );
	strValue = GetDigitString( strTime, 2, 4 );//计算分钟数
	sscanf( strValue, "%d", &nMin );

	//判断合法性
	if ( nHour < 0 )
	{
		nHour = 0;
	}
	if ( nHour > 23 )
	{
		nHour = 23;
	}
	if ( nMin < 0 )
	{
		nMin = 0;
	}
	if ( nMin > 59 )
	{
		nMin = 59;
	}

	//修改最终值
	strTime.Format( TimeFormat, nHour, nMin );
}

bool CTreasureValidTimeDlg::CheckValidString( const CString& strSource )
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
	int nCount = 0, nFlag = 0, nWStrLen = wcslen( wszStr );
	for ( int i = 0; i < nWStrLen; ++i )
	{
		if ( !iswdigit(wszStr[i]) )
		{
			nFlag++;
			if ( 0 == i )
			{
				delete []wszStr;
				return false;
			}
			if ( L'：' == wszStr[i] )
			{
				nCount++;
				if ( nCount >= 2 )
				{
					delete []wszStr;
					return false;
				}
			}
		}
	}
	
	//最后验证
	if ( (!nCount && nFlag) || (nCount && nFlag >= 2 ) )
	{
		delete []wszStr;
		return false;
	}

	//
	delete []wszStr;
	return true;
}

void CTreasureValidTimeDlg::UpdateEditData( CString& strEdit )
{
	CString strOld = strEdit;
	UpdateData(TRUE);

	if ( CheckValidString( strEdit ) )
	{
		UpdateTimeString( strEdit );
	}
	else
	{
		strEdit = strOld;
	}
	UpdateData(FALSE); 
}

void CTreasureValidTimeDlg::OnUpdateEdit2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// 
	UpdateEditData( m_strEndTimeForTimeInterval );	

}

void CTreasureValidTimeDlg::OnUpdateEdit3() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	//
	UpdateEditData( m_strBeginTimeForWeek );	
	
}

void CTreasureValidTimeDlg::OnUpdateEdit4() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	//
	UpdateEditData( m_strEndTimeForWeek );	
}

void CTreasureValidTimeDlg::OnUpdateEdit5() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	//
	UpdateEditData( m_strBeginTimeForMonth );
}

void CTreasureValidTimeDlg::OnUpdateEdit6() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	//
	UpdateEditData( m_strEndTimeForMonth );
	
}
