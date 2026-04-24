#if !defined(AFX_OCCUPATIONMASKDLG_H__3B722B4C_A1F3_4622_9DD0_E21F633B5C57__INCLUDED_)
#define AFX_OCCUPATIONMASKDLG_H__3B722B4C_A1F3_4622_9DD0_E21F633B5C57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OccupationMaskDlg.h : header file
//

#include "AObject.h"

struct OccupationMask : public CUSTOM_FUNCS
{
	~OccupationMask() {}
	virtual BOOL CALLBACK OnActivate(void);
	virtual LPCTSTR CALLBACK OnGetShowString(void) const;
	virtual AVariant CALLBACK OnGetValue(void) const;
	virtual void CALLBACK OnSetValue(const AVariant& var);
	AVariant m_var;
	LPCTSTR* m_pMaskNames;
	int m_nNameCount;
};
/////////////////////////////////////////////////////////////////////////////
// COccupationMaskDlg dialog

class COccupationMaskDlg : public CDialog
{
// Construction
public:
	COccupationMaskDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COccupationMaskDlg)
	enum { IDD = IDD_OCCUPATION_MASK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	unsigned __int64 m_ulOccupationMask;
	LPCTSTR* m_pMaskNames;
	int m_nNameCount;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COccupationMaskDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COccupationMaskDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnButtonSelectAll();
	afx_msg void OnButtonUnselectAll();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OCCUPATIONMASKDLG_H__3B722B4C_A1F3_4622_9DD0_E21F633B5C57__INCLUDED_)
