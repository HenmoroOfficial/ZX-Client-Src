// AnimationEditorView.h : interface of the CAnimationEditorView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANIMATIONEDITORVIEW_H__C010687E_F8BD_4B43_9462_478F6866E1AC__INCLUDED_)
#define AFX_ANIMATIONEDITORVIEW_H__C010687E_F8BD_4B43_9462_478F6866E1AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OperationRecorder.h"

class CAnmObject;
class CAnmBezier;
class CAnmAxes;

/////////////////////////////////////////////////////////////////////////////
// CAnimationEditorView window

class CAnimationEditorView : public CWnd
{
public:
    enum EditorState
    {
        STATE_SELECTION = 0,
        STATE_TRANSLATION,
        STATE_TRANSLATION_TERRAIN,
        STATE_ROTATION,
        STATE_ROTATION_TERRAIN,
        STATE_ADD_POINT,
        STATE_MOVE_BEZIER,
    };
// Construction
public:
    CAnimationEditorView();

    void KeyboardMouseMoveOperation();

    void ClearOperation();

private:
    void ObjectOperation(CAnmObject *pObject, CPoint point);

    void BezierOperation(CAnmBezier *pBezier, CPoint point);

    void SetOperation(Operation *op);

// Attributes
public:

private:
    COperationRecorder m_Recorder;
    bool m_bLButtonPressed;
    bool m_bRButtonPressed;
    bool m_bOperating;
    CPoint m_ptMouseLast;
    EditorState m_eEditorState;
    bool m_bBezierEditor;
    CAnmAxes *m_pAxes;
    float m_fLastHitFractor;

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAnimationEditorView)
	protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CAnimationEditorView();

    // Generated message map functions
protected:
public:
    void SelectObject(const unsigned int uiID);
    void SelectBezierPoint(const unsigned int uiID);
    void SelectBezierSegment(const unsigned int uiID, const int anchor);
    //{{AFX_MSG(CAnimationEditorView)
    afx_msg void OnPaint();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnSelection();
	afx_msg void OnUpdateSelection(CCmdUI* pCmdUI);
	afx_msg void OnTranslation();
    afx_msg void OnTranslationTerrain();
	afx_msg void OnRotation();
	afx_msg void OnUpdateTranslation(CCmdUI* pCmdUI);
    afx_msg void OnUpdateTranslationTerrain(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRotation(CCmdUI* pCmdUI);
	afx_msg void OnAddBezierPoint();
	afx_msg void OnRemoveBezierPoint();
	afx_msg void OnUpdateAddBezierPoint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRemoveBezierPoint(CCmdUI* pCmdUI);
	afx_msg void OnBezierState();
	afx_msg void OnUpdateBezierState(CCmdUI* pCmdUI);
	afx_msg void OnAddBezierPointWithNowCamera();
	afx_msg void OnAddBezierPointWithNowModel();
	afx_msg void OnUpdateAddBezierPointWithNowCamera(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddBezierPointWithNowModel(CCmdUI* pCmdUI);
	afx_msg void OnFixBezierPointWithNowCamera();
	afx_msg void OnUpdateFixBezierPointWithNowCamera(CCmdUI* pCmdUI);
	afx_msg void OnDisableCameraAnimation();
	afx_msg void OnUpdateDisableCameraAnimation(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnModelToCamera();
	afx_msg void OnUpdateModelToCamera(CCmdUI* pCmdUI);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnMoveBezier();
	afx_msg void OnUpdateMoveBezier(CCmdUI* pCmdUI);
	afx_msg void OnBindCameraToObject();
	afx_msg void OnUpdateBindCameraToObject(CCmdUI* pCmdUI);
	afx_msg void OnBindCameraOffset();
	afx_msg void OnUpdateBindCameraOffset(CCmdUI* pCmdUI);
	afx_msg void OnRotationTerrain();
	afx_msg void OnUpdateRotationTerrain(CCmdUI* pCmdUI);
	afx_msg void OnFixBezierPointWithNowModel();
	afx_msg void OnUpdateFixBezierPointWithNowModel(CCmdUI* pCmdUI);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANIMATIONEDITORVIEW_H__C010687E_F8BD_4B43_9462_478F6866E1AC__INCLUDED_)
