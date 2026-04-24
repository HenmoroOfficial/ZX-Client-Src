#if !defined(AFX_QUESTIONSDLG_H__87CC01AD_E68E_4DFE_BF34_BF28807B28F1__INCLUDED_)
#define AFX_QUESTIONSDLG_H__87CC01AD_E68E_4DFE_BF34_BF28807B28F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// QuestionsDlg.h : header file
//
#include <vector.h>
class CQuestionData;
/////////////////////////////////////////////////////////////////////////////
// CQuestionsDlg dialog

class CQuestionsDlg : public CDialog
{
// Construction
public:
	CQuestionsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CQuestionsDlg)
	enum { IDD = IDD_DIALOG_QUESTION_EDITOR };
	CTreeCtrl	m_Tree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQuestionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList m_ImageList;
	FIXED FloatToFixed(double d);
    int mapFXY(FIXED fxy);
	bool LoadIDTemplate();
	bool LoadFolderTemplate();
	void FreshQuestions();
	void UpdateMenu(CMenu *pMenu);
	void AddQuestion( CString *pData, CString sFolder);
	bool IsValid(CQuestionData *pQs);
	bool WriteVectorString(FILE *pFile,CString str);
	void GetOutLine(int wType,int cpfx, POINTFX apfx[],abase::vector<CPoint>& listPath);
	void TranslateToByte(abase::vector<CPoint>& listPath, char* pBuffer);
	// Generated message map functions
	//{{AFX_MSG(CQuestionsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRclickTreeQuestions(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCreateQuestion();
	afx_msg void OnQuestionCheckIn();
	afx_msg void OnQuestionCheckOut();
	afx_msg void OnDblclkTreeQuestions(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonExport();
	afx_msg void OnButtonFresh();
	afx_msg void OnCreateFolder();
	afx_msg void OnItemexpandedTreeQuestions(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonImport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QUESTIONSDLG_H__87CC01AD_E68E_4DFE_BF34_BF28807B28F1__INCLUDED_)
