#if !defined(AFX_TREASUREVALIDTIMEDLG_H__41C824F3_0512_4BF0_A109_A4D30F7E6CA3__INCLUDED_)
#define AFX_TREASUREVALIDTIMEDLG_H__41C824F3_0512_4BF0_A109_A4D30F7E6CA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreasureValidTimeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTreasureValidTimeDlg dialog

class CTreasureValidTimeDlg : public CDialog
{
// Construction
public:
	CTreasureValidTimeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTreasureValidTimeDlg)
	enum { IDD = IDD_DIALOG_TREASURE_VALID_TIME };
	int		m_nType;
	BOOL	m_bStartTimeForTimeInterval;
	BOOL	m_bEndTimeForTimeInterval;
	CTime	m_tmEndTimeForTimeInterval;
	CTime	m_tmBeginTimeForTimeInterval;
	CString	m_strBeginTimeForTimeInterval;
	CString	m_strEndTimeForTimeInterval;
	CString	m_strBeginTimeForWeek;
	CString	m_strEndTimeForWeek;
	CString	m_strBeginTimeForMonth;
	CString	m_strEndTimeForMonth;
	//}}AFX_DATA

	//-------------------------------------------------------------------------
	int		m_nParam;//参数掩码
	int		m_nStartTime;//开始时间
	int		m_nEndTime;//结束时间

	//
	void	SetAllValues( int nType, int nStartTime, int nEndTime, int nParam )
	{
		m_nType			= nType;
		m_nParam		= nParam;
		m_nStartTime	= nStartTime;
		m_nEndTime		= nEndTime;
	}

	void	UpdateFormatString( CString& strTime, int nFlag );
	int		GetTimeValue( CString strTime );
	CString	GetDigitString( const CString& strSource, int nStart = 0, int nSize = 0 );
	void	UpdateTimeString( CString& strTime );
	bool	CheckValidString( const CString& strSource );
	void	UpdateEditData( CString& strEdit );

	enum TYPE
	{
		TYPE_NOLIMIT = 0,	//无效，无限制
		TYPE_TIMEINTERVAL,	//一段时间有效
		TYPE_WEEK,			//每周有效
		TYPE_MONTH,			//每月有效
	};
	//-------------------------------------------------------------------------

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreasureValidTimeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTreasureValidTimeDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSpinStartTimeForTimeInterval(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinEndTimeForTimeInterval(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinStartTimeForWeek(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinEndTimeForWeek(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinStartTimeForMonth(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinEndTimeForMonth(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateEditForTimeInterval();
	afx_msg void OnUpdateEdit2();
	afx_msg void OnUpdateEdit3();
	afx_msg void OnUpdateEdit4();
	afx_msg void OnUpdateEdit5();
	afx_msg void OnUpdateEdit6();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREASUREVALIDTIMEDLG_H__41C824F3_0512_4BF0_A109_A4D30F7E6CA3__INCLUDED_)
