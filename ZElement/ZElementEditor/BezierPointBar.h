#if !defined(AFX_BEZIERPOINTBAR_H__34CE6633_51FB_4A16_BE0A_A511367F261D__INCLUDED_)
#define AFX_BEZIERPOINTBAR_H__34CE6633_51FB_4A16_BE0A_A511367F261D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BezierPointBar.h : header file
//
class CEditerBezier;
class CEditerBezierPoint;

/////////////////////////////////////////////////////////////////////////////
// CBezierPointBar dialog

class CBezierPointBar : public CDialogBar
{
// Construction
public:
	CBezierPointBar(CWnd* pParent = NULL);   // standard constructor
	void Init( CEditerBezier* pObj, CEditerBezierPoint *pBezierPoint);
// Dialog Data
	//{{AFX_DATA(CBezierPointBar)
	enum { IDD = IDD_DIALOG_EDIT_POINT };
	float	m_fCamFov;
	float	m_fCamSpeed;
	float	m_fCamX;
	float	m_fCamY;
	float	m_fCamZ;
	float	m_fCamRatio;
	float	m_fSpotRadius;
	//}}AFX_DATA

	void OnNextPointModified();
	void OnPrevPointModified();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBezierPointBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CEditerBezierPoint* m_pBezierPoint;
	CEditerBezier* m_pBezier;
	float m_fOldFov;
	float m_fOldRadio;
	// Generated message map functions
	//{{AFX_MSG(CBezierPointBar)
	afx_msg void OnCheckChangeCam();
	afx_msg void OnBezierModified();
	afx_msg void OnUpdateBezierModified(CCmdUI *pCmdUI);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnNextPoint(CCmdUI *pCmdUI);
	afx_msg void OnPrevPoint(CCmdUI *pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BEZIERPOINTBAR_H__34CE6633_51FB_4A16_BE0A_A511367F261D__INCLUDED_)
