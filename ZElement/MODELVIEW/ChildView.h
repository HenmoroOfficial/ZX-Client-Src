// ChildView.h : interface of the CChildView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDVIEW_H__78AE8967_0776_4A59_A8F8_8A4F7FAF0A6A__INCLUDED_)
#define AFX_CHILDVIEW_H__78AE8967_0776_4A59_A8F8_8A4F7FAF0A6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CChildView window
class A3DCamera;
class A3DViewport;

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL DestroyWindow();
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChildView();
	bool Init();
	void Render();
	void MouseWheel(int delta);
	void SetCamaraPos( A3DVECTOR3 vPos );
	// Generated message map functions
protected:
	void     DrawCenter();

	A3DCamera*	m_pA3DCamera;
	A3DViewport*	m_pA3DViewport;
	int		m_iWidth;		//	Viewport size
	int		m_iHeight;
	bool    m_bLDrag;
	bool    m_bRDrag;
	CPoint  m_ptLast;

	
	//{{AFX_MSG(CChildView)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSetRestoreCam();
	afx_msg void OnSetShowAabb();
	afx_msg void OnUpdateSetShowAabb(CCmdUI* pCmdUI);
	afx_msg void OnSetShowGrid();
	afx_msg void OnUpdateSetShowGrid(CCmdUI* pCmdUI);
	afx_msg void OnSetShowWire();
	afx_msg void OnUpdateSetShowWire(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSetAmbClr();
	afx_msg void OnSetClearClr();
	afx_msg void OnSetDirClr();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW_H__78AE8967_0776_4A59_A8F8_8A4F7FAF0A6A__INCLUDED_)
