#if !defined(AFX_TIMESETTINGDLG_H__F2EB3A84_1FBA_44A1_A379_00FF6A50B74E__INCLUDED_)
#define AFX_TIMESETTINGDLG_H__F2EB3A84_1FBA_44A1_A379_00FF6A50B74E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TimeSettingDlg.h : header file
//

//-----------------------------------------------------------------------------
#include "AObject.h"

struct TimeSetting : public CUSTOM_FUNCS
{
	~TimeSetting() {}
	virtual BOOL CALLBACK OnActivate(void);
	virtual LPCTSTR CALLBACK OnGetShowString(void) const;
	virtual AVariant CALLBACK OnGetValue(void) const;
	virtual void CALLBACK OnSetValue(const AVariant& var);
	AVariant m_var;
	int m_nFlag;
};

//-----------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
// CTimeSettingDlg dialog

class CTimeSettingDlg : public CDialog
{
// Construction
public:
	CTimeSettingDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTimeSettingDlg)
	enum { IDD = IDD_DIALOG_TIME_SETTING };
	CTime	m_tmDateTime;
	CString	m_strDayTime;
	//}}AFX_DATA

	//-------------------------------------------------------------------------
	int		m_nCurrentTime;	//当前设置的时间，换算成秒之后的值。同时，这个值进行了时区校正
	int		m_nFlag;		//处理标志，0：年月日和小时分秒都有，1：只有年月日，2：只有小时分秒

	void	FormatToTimeString( CString& str, int nHour, int nMin, int nSecond );
	int		FormatFromTimeString( CString str );
	bool	CheckValidString( const CString& strSource );
	//-------------------------------------------------------------------------


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTimeSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTimeSettingDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDeltaposDayTimeInc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateDayTime();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMESETTINGDLG_H__F2EB3A84_1FBA_44A1_A379_00FF6A50B74E__INCLUDED_)
