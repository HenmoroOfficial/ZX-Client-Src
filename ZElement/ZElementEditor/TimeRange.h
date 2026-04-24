#if !defined(AFX_TIMERANGE_H__5D3A3EB9_4E17_4267_8F4E_175BA4E4ECC1__INCLUDED_)
#define AFX_TIMERANGE_H__5D3A3EB9_4E17_4267_8F4E_175BA4E4ECC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TimeRange.h : header file
//

#include "NpcControllerDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CTimeRange dialog

class CTimeRange : public CDialog
{
// Construction
public:
	CTimeRange(CWnd* pParent = NULL);   // standard constructor

	CONTROLLER_TIME* m_pTimeStart;
	CONTROLLER_TIME* m_pTimeEnd;
// Dialog Data
	//{{AFX_DATA(CTimeRange)
	enum { IDD = IDD_TIME_RANGE_EDIT };
	CComboBox	m_cbStartYear;
	CComboBox	m_cbStartWeek;
	CComboBox	m_cbStartMonth;
	CComboBox	m_cbStartMinute;
	CComboBox	m_cbStartHour;
	CComboBox	m_cbStartDay;
	CComboBox	m_cbEndYear;
	CComboBox	m_cbEndWeek;
	CComboBox	m_cbEndMonth;
	CComboBox	m_cbEndMinute;
	CComboBox	m_cbEndHour;
	CComboBox	m_cbEndDay;
	//}}AFX_DATA

	bool	m_bReadOnly;

	bool IsValidTime();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTimeRange)
	protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void InitCombox();
	// Generated message map functions
	//{{AFX_MSG(CTimeRange)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMERANGE_H__5D3A3EB9_4E17_4267_8F4E_175BA4E4ECC1__INCLUDED_)
