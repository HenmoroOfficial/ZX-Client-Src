#if !defined(AFX_GENERALDATAITEMDLG_H__75BBB694_B698_4B23_AACA_60EBBE0CA0EA__INCLUDED_)
#define AFX_GENERALDATAITEMDLG_H__75BBB694_B698_4B23_AACA_60EBBE0CA0EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GeneralDataItemDlg.h : header file
//

//=============================================================================
#include "AObject.h"
class CGeneralDataUIWrapper;
class CPropertyList;
//=============================================================================
/////////////////////////////////////////////////////////////////////////////
// CGeneralDataItemDlg dialog

class CGeneralDataItemDlg : public CDialog
{
// Construction
public:
	CGeneralDataItemDlg(CWnd* pParent = NULL);   // standard constructor

	//-------------------------------------------------------------------------
	void Init( CGeneralDataUIWrapper* pGeneralDataUIWrapper )
	{
		m_pGeneralDataUIWrapper = pGeneralDataUIWrapper;
	}

	void SetDataReadOnly( bool bDataReadOnly = false ) { m_bDataReadOnly = bDataReadOnly; }
	//-------------------------------------------------------------------------

// Dialog Data
	//{{AFX_DATA(CGeneralDataItemDlg)
	enum { IDD = IDD_DIALOG_TREASURE_ITEM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGeneralDataItemDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//-------------------------------------------------------------------------
	CPropertyList*			m_pList;
	CGeneralDataUIWrapper*	m_pGeneralDataUIWrapper;
	bool					m_bDataReadOnly;
	//-------------------------------------------------------------------------
	// Generated message map functions
	//{{AFX_MSG(CGeneralDataItemDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENERALDATAITEMDLG_H__75BBB694_B698_4B23_AACA_60EBBE0CA0EA__INCLUDED_)
