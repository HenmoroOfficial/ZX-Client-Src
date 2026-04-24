#if !defined(AFX_QUESTIONITEM_H__A884714E_5E23_4232_9391_79148CF7CC4F__INCLUDED_)
#define AFX_QUESTIONITEM_H__A884714E_5E23_4232_9391_79148CF7CC4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// QuestionItem.h : header file
//

class CQuestionData;

/////////////////////////////////////////////////////////////////////////////
// CQuestionItem dialog

class CQuestionItem : public CDialog
{
// Construction
public:
	CQuestionItem(CWnd* pParent = NULL);   // standard constructor
	void Init(CString strPath){ m_strPathName = strPath; }
// Dialog Data
	//{{AFX_DATA(CQuestionItem)
	enum { IDD = IDD_DIALOG_QUESTION_ITEM };
	CString	m_strQuestion;
	CString	m_strAnswerF;
	CString	m_strAnswerE;
	CString	m_strAnswerD;
	CString	m_strAnswerC;
	CString	m_strAnswerB;
	CString	m_strAnswerA;
	BOOL	m_bKeyA;
	BOOL	m_bKeyB;
	BOOL	m_bKeyC;
	BOOL	m_bKeyD;
	BOOL	m_bKeyE;
	BOOL	m_bKeyF;
	BOOL	m_bDeleted;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQuestionItem)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_strPathName;
	CQuestionData* m_pCurQuestion;
	int     m_nAnswer;
	bool    m_bReadOnly;

	bool LoadQuestion();
	void EnableItem(char c,BOOL bEnable);
	void DisableAllItem();
	void DisableInterface();
	void SetItemCheck(int n);

	bool IsChanged();

	// Generated message map functions
	//{{AFX_MSG(CQuestionItem)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg void OnRadio5();
	afx_msg void OnRadio6();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QUESTIONITEM_H__A884714E_5E23_4232_9391_79148CF7CC4F__INCLUDED_)
