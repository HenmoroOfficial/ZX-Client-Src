#if !defined(AFX_SELECTEDOBJECT_H__2948AB59_EFE7_4CEA_8598_2D4F281BBB04__INCLUDED_)
#define AFX_SELECTEDOBJECT_H__2948AB59_EFE7_4CEA_8598_2D4F281BBB04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectedObject.h : header file
//

//#include "Animator/AnmObject.h"
class CAnmObject;

/////////////////////////////////////////////////////////////////////////////
// CSelectedObjectDlg dialog

class CSelectedObjectDlg : public CDialog
{
// Construction
public:
	CSelectedObjectDlg(CWnd* pParent, CAnmObject *pObject);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelectedObjectDlg)
	enum { IDD = IDD_DLG_OBJECT };
	UINT	m_uiObjectID;
	CString	m_sObjectType;
	CString	m_sObjectDisplayName;
	CString	m_sObjectFileName;
	float	m_fOffsetX;
	float	m_fOffsetY;
	float	m_fOffsetZ;
	//}}AFX_DATA

private:
    CAnmObject *m_pObject;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectedObjectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectedObjectDlg)
	afx_msg void OnButtonObjectImport();
	afx_msg void OnButtonObjectOk();
	afx_msg void OnButtonObjectCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTEDOBJECT_H__2948AB59_EFE7_4CEA_8598_2D4F281BBB04__INCLUDED_)
