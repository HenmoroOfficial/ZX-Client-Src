#if !defined(AFX_CUSTOMACTIONDLG_H__DEBD9C34_44D0_4BBB_BBC2_F9C69FBFD180__INCLUDED_)
#define AFX_CUSTOMACTIONDLG_H__DEBD9C34_44D0_4BBB_BBC2_F9C69FBFD180__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CustomActionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCustomActionDlg dialog

class CCustomActionDlg : public CDialog
{
// Construction
public:
	CCustomActionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCustomActionDlg)
	enum { IDD = IDD_DLG_CUSTOM_ACTION };
	CButton	m_btnBind;
	CButton	m_btnAdd;
	CComboBox	m_comboGender;
	CComboBox	m_comboSect;
	CComboBox	m_comboBindAction;
	CComboBox	m_comboAction;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomActionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCustomActionDlg)
	afx_msg void OnEditchangeComboCustomAction();
	afx_msg void OnSelchangeComboCustomAction();
	afx_msg void OnSelchangeComboCustomActionBind();
	afx_msg void OnSelchangeComboCustomActionSect();
	afx_msg void OnButtonCustomActionAdd();
	afx_msg void OnButtonCustomActionBind();
	afx_msg void OnButtonCustomActionCancel();
	afx_msg void OnButtonCustomActionOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboCustomActionGender();
	afx_msg void OnEditchangeComboCustomActionBind();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void UpdateComboBindAction();
	void UpdateComboBindActionSel();


	int m_iCommonName;
	int m_iRealName;
	int m_iGender;
	int m_iSect;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMACTIONDLG_H__DEBD9C34_44D0_4BBB_BBC2_F9C69FBFD180__INCLUDED_)
