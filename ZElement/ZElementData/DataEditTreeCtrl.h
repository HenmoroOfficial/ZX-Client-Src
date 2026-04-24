#if !defined(AFX_DATAEDITTREECTRL_H__7518FCF0_8EF0_4CDB_A7DF_0716D696ABAE__INCLUDED_)
#define AFX_DATAEDITTREECTRL_H__7518FCF0_8EF0_4CDB_A7DF_0716D696ABAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataEditTreeCtrl.h : header file
//
//=============================================================================
class CGeneralDataMan;
//=============================================================================
/////////////////////////////////////////////////////////////////////////////
// CDataEditTreeCtrl window

class CDataEditTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CDataEditTreeCtrl();
	void SetDataMan( CGeneralDataMan* pGeneralDataMan ){ m_pGeneralDataMan = pGeneralDataMan; }

// Attributes
public:
	bool		m_bDataChanged;
	bool		m_bDataReadOnly;
	CString		m_strOrgPath;

// Operations
public:
	HTREEITEM CopyBranch(HTREEITEM htiBranch,HTREEITEM htiNewParent,HTREEITEM htiAfter);
	void MoveUp();
	void MoveDown();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataEditTreeCtrl)
	//}}AFX_VIRTUAL

protected:
	void UpdateMenu(CMenu *pMenu);
	int GetChildItemsCount(HTREEITEM hParent);
	HTREEITEM GetRejectItem();
	HTREEITEM CopyItem(HTREEITEM hItem, HTREEITEM htiNewParent, HTREEITEM htiAfter);

// Implementation
public:
	virtual ~CDataEditTreeCtrl();

	// Generated message map functions
protected:
	//-------------------------------------------------------------------------
	CGeneralDataMan*		m_pGeneralDataMan;
	//-------------------------------------------------------------------------
	UINT          m_TimerTicks;      //处理滚动的定时器所经过的时间
	UINT          m_nScrollTimerID;  //处理滚动的定时器
	CPoint        m_HoverPoint;      //鼠标位置
	UINT          m_nHoverTimerID;   //鼠标敏感定时器
	DWORD         m_dwDragStart;     //按下鼠标左键那一刻的时间
	BOOL          m_bDragging;       //标识是否正在拖动过程中
	BOOL          m_bEditting;       
	CImageList*   m_pDragImage;      //拖动时显示的图象列表
	HTREEITEM     m_hItemDragS;      //被拖动的标签
	HTREEITEM     m_hItemDragD;      //接受拖动的标签
	//-------------------------------------------------------------------------

	//{{AFX_MSG(CDataEditTreeCtrl)
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuAddsub();
	afx_msg void OnMenuAddDataItem();
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuDelDataItem();
	afx_msg void OnMenuDoffDataItem();
	afx_msg void OnMenuDelsub();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAEDITTREECTRL_H__7518FCF0_8EF0_4CDB_A7DF_0716D696ABAE__INCLUDED_)
