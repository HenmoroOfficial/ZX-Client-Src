#if !defined(AFX_EXTRAAREAMANDLG_H__7D14F3EB_2621_419A_904C_C64521999238__INCLUDED_)
#define AFX_EXTRAAREAMANDLG_H__7D14F3EB_2621_419A_904C_C64521999238__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExtraAreaManDlg.h : header file
//
class CSceneObject;
/////////////////////////////////////////////////////////////////////////////
// CExtraAreaManDlg dialog

class CExtraAreaManDlg : public CDialog
{
// Construction
public:
	CExtraAreaManDlg(CWnd* pParent = NULL);   // standard constructor
    void Init( CSceneObject *pObj , int nObjType ){ m_pSceneAiObject = pObj; m_nObjectType = nObjType; }
// Dialog Data
	//{{AFX_DATA(CExtraAreaManDlg)
	enum { IDD = IDD_DIALOG_EXTRA_AREA_MAN };
	CListBox	m_listExtra;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtraAreaManDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateList();
	
	CSceneObject *m_pSceneAiObject;
	int m_nObjectType;
	// Generated message map functions
	//{{AFX_MSG(CExtraAreaManDlg)
	afx_msg void OnAddExtra();
	afx_msg void OnDelExtra();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXTRAAREAMANDLG_H__7D14F3EB_2621_419A_904C_C64521999238__INCLUDED_)
