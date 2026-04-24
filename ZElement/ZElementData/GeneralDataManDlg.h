#if !defined(AFX_GENERALDATAMANDLG_H__1F30BF8F_3041_485B_853B_8B72ACCBFE01__INCLUDED_)
#define AFX_GENERALDATAMANDLG_H__1F30BF8F_3041_485B_853B_8B72ACCBFE01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GeneralDataManDlg.h : header file
//
//=============================================================================
#include "DataEditTreeCtrl.h"
#include "GeneralDataMan.h"
//=============================================================================

/////////////////////////////////////////////////////////////////////////////
// CGeneralDataManDlg dialog

class CGeneralDataManDlg : public CDialog
{
// Construction
public:
	CGeneralDataManDlg(CWnd* pParent = NULL);   // standard constructor

	//-------------------------------------------------------------------------
	void Init( CString strDataFileName, CString strTemplIDFileName, CString strOrgPath, const char* szMainTypes[], int nMainTypesCount, int nGenenralDataTemplTypeID = 0 )
	{
		m_strDataFileName			= strDataFileName;
		m_strTemplIDFileName		= strTemplIDFileName;
		m_strOrgPath				= strOrgPath;
		m_szMainTypes				= szMainTypes;
		m_nMainTypesCount			= nMainTypesCount;
		m_nGeneralDataTemplTypeID	= nGenenralDataTemplTypeID;
	}
	void Release();
	//-------------------------------------------------------------------------

// Dialog Data
	//{{AFX_DATA(CGeneralDataManDlg)
	enum { IDD = IDD_DIALOG_TREASURE };
	CStatic	m_staticIcon;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGeneralDataManDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnCancel();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//-------------------------------------------------------------------------
	CDataEditTreeCtrl	m_treeDataMan;
	CImageList			m_ImageList;
	HICON				m_IconReadOnly;
	HICON				m_IconWriteable;
	CString				m_strDataFileName;
	CString				m_strTemplIDFileName;
	CString				m_strOrgPath;
	const char**		m_szMainTypes;
	int					m_nMainTypesCount;
	bool				m_bReadOnly;
	int					m_nGeneralDataTemplTypeID;

	//===============================
	CGeneralDataMan*	m_pGeneralDataMan;

	//===============================
	void RefreshTreeList( void );	//刷新整个general data manager的tree list
	//===============================
	//-------------------------------------------------------------------------

	// Generated message map functions
	//{{AFX_MSG(CGeneralDataManDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnExport();
	afx_msg void OnFreshGeneralData();
	afx_msg void OnCheckOut();
	afx_msg void OnCheckIn();
	afx_msg void OnMoveLast();
	afx_msg void OnMoveDown();
	afx_msg void OnExportAttribute();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENERALDATAMANDLG_H__1F30BF8F_3041_485B_853B_8B72ACCBFE01__INCLUDED_)
