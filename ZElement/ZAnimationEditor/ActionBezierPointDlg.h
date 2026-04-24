#if !defined(AFX_ACTIONBEZIERPOINTDLG_H__97DA521C_8D1D_4D78_B58D_E92B6101E2D2__INCLUDED_)
#define AFX_ACTIONBEZIERPOINTDLG_H__97DA521C_8D1D_4D78_B58D_E92B6101E2D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActionBezierPointDlg.h : header file
//

class CAnmBezierPoint;
/////////////////////////////////////////////////////////////////////////////
// CActionBezierPointDlg dialog

class CActionBezierPointDlg : public CDialog
{
// Construction
public:
	CActionBezierPointDlg(CWnd* pParent, CAnmBezierPoint *pPoint);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CActionBezierPointDlg)
	enum { IDD = IDD_DLG_ACTION_BEZIER_POINT };
	UINT	m_uiPointID;
	int	    m_iTotalFrames;
	float	m_fFOV;
	float	m_fPosX;
	float	m_fPosY;
	float	m_fPosZ;
	float	m_fOriW;
	float	m_fOriX;
	float	m_fOriY;
	float	m_fOriZ;
	//}}AFX_DATA

private:
    CAnmBezierPoint *m_pBezierPoint;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActionBezierPointDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CActionBezierPointDlg)
	afx_msg void OnButtonActionOk();
	afx_msg void OnButtonActionCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONBEZIERPOINTDLG_H__97DA521C_8D1D_4D78_B58D_E92B6101E2D2__INCLUDED_)
