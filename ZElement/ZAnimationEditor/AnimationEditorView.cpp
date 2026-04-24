// AnimationEditorView.cpp : implementation of the CAnimationEditorView class
//

#include "stdafx.h"
#include "EC_Game.h"
#include "EC_Global.h"
#include "EC_Configs.h"
#include "A3DGfxExMan.h"
#include "AnimationEditor.h"
#include "AnimationEditorView.h"
#include "AnmSystem.h"
#include "Animator.h"
#include "AnmRayTrace.h"
#include "AnmBezierManager.h"
#include "AnmObjectManager.h"
#include "AnmAxes.h"
#include "BindCameraDlg.h"

#ifdef BACK_VERSION
#include "EC_Viewport.h"
#else
#include "ECViewport.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnimationEditorView

CAnimationEditorView::CAnimationEditorView()
{
    m_bLButtonPressed = false;
    m_bRButtonPressed = false;
    m_bOperating = false;
    m_eEditorState = STATE_SELECTION;
    m_bBezierEditor = false;
    m_pAxes = 0;
}

void CAnimationEditorView::SelectObject(const unsigned int uiID)
{
    m_pAxes = g_Animator.GetAxes();
    g_Animator.GetObjectManager()->SelectObject(uiID);
    CAnmObject *pObject = g_Animator.GetObjectManager()->GetSelectedObject();
    if (pObject)
    {
        m_pAxes->SetVisable(true);
        const A3DAABB objectAABB = pObject->GetAABB();
        m_pAxes->SetPos(pObject->GetPos()+A3DVECTOR3(0.0f, objectAABB.Maxs.y-objectAABB.Mins.y, 0.0f));
    }
}

void CAnimationEditorView::SelectBezierPoint(const unsigned int uiID)
{
    m_pAxes = g_Animator.GetAxes();
    CAnmBezier *pBezier = g_Animator.GetBezierManager()->GetSelectedBezier();
    if (pBezier)
    {
        pBezier->SelectPoint(uiID);
        CAnmBezierPoint *pPoint = pBezier->GetSelectedPoint();
        if (pPoint)
        {
            m_pAxes->SetVisable(true);
            m_pAxes->SetPos(pPoint->GetPos());
        }
    }
}

void CAnimationEditorView::SelectBezierSegment(const unsigned int uiID, const int anchor)
{
    m_pAxes = g_Animator.GetAxes();
    CAnmBezier *pBezier = g_Animator.GetBezierManager()->GetSelectedBezier();
    if (pBezier)
    {
        pBezier->SelectSegment(uiID);
        pBezier->SelectSegmentAnchor(anchor);
        CAnmBezierSegment *pSegment = pBezier->GetSelectedSegment();
        if (pSegment)
        {
            m_pAxes->SetVisable(true);
            if (anchor == 1)
            {
                m_pAxes->SetPos(pSegment->GetAnchorA());
            }
            else if (anchor == 2)
            {
                m_pAxes->SetPos(pSegment->GetAnchorB());
            }
        }
    }
}

CAnimationEditorView::~CAnimationEditorView()
{
}


BEGIN_MESSAGE_MAP(CAnimationEditorView, CWnd)
    //{{AFX_MSG_MAP(CAnimationEditorView)
    ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
    ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
    ON_COMMAND(ID_SELECTION, OnSelection)
    ON_UPDATE_COMMAND_UI(ID_SELECTION, OnUpdateSelection)
	ON_COMMAND(ID_TRANSLATION, OnTranslation)
    ON_COMMAND(ID_TRANSLATION_TERRAIN, OnTranslationTerrain)
	ON_COMMAND(ID_ROTATION, OnRotation)
	ON_UPDATE_COMMAND_UI(ID_TRANSLATION, OnUpdateTranslation)
    ON_UPDATE_COMMAND_UI(ID_TRANSLATION_TERRAIN, OnUpdateTranslationTerrain)
    ON_UPDATE_COMMAND_UI(ID_ROTATION, OnUpdateRotation)
	ON_COMMAND(ID_ADD_BEZIER_POINT, OnAddBezierPoint)
	ON_COMMAND(ID_REMOVE_BEZIER_POINT, OnRemoveBezierPoint)
	ON_UPDATE_COMMAND_UI(ID_ADD_BEZIER_POINT, OnUpdateAddBezierPoint)
	ON_UPDATE_COMMAND_UI(ID_REMOVE_BEZIER_POINT, OnUpdateRemoveBezierPoint)
	ON_COMMAND(ID_BEZIER_STATE, OnBezierState)
	ON_UPDATE_COMMAND_UI(ID_BEZIER_STATE, OnUpdateBezierState)
	ON_COMMAND(ID_ADD_BEZIER_POINT_WITH_NOW_CAMERA, OnAddBezierPointWithNowCamera)
	ON_COMMAND(ID_ADD_BEZIER_POINT_WITH_NOW_MODEL, OnAddBezierPointWithNowModel)
	ON_UPDATE_COMMAND_UI(ID_ADD_BEZIER_POINT_WITH_NOW_CAMERA, OnUpdateAddBezierPointWithNowCamera)
	ON_UPDATE_COMMAND_UI(ID_ADD_BEZIER_POINT_WITH_NOW_MODEL, OnUpdateAddBezierPointWithNowModel)
	ON_COMMAND(ID_FIX_BEZIER_POINT_WITH_NOW_CAMERA, OnFixBezierPointWithNowCamera)
	ON_UPDATE_COMMAND_UI(ID_FIX_BEZIER_POINT_WITH_NOW_CAMERA, OnUpdateFixBezierPointWithNowCamera)
	ON_COMMAND(ID_DISABLE_CAMERA_ANIMATION, OnDisableCameraAnimation)
	ON_UPDATE_COMMAND_UI(ID_DISABLE_CAMERA_ANIMATION, OnUpdateDisableCameraAnimation)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_MODEL_TO_CAMERA, OnModelToCamera)
	ON_UPDATE_COMMAND_UI(ID_MODEL_TO_CAMERA, OnUpdateModelToCamera)
	ON_WM_WINDOWPOSCHANGED()
	ON_COMMAND(ID_MOVE_BEZIER, OnMoveBezier)
	ON_UPDATE_COMMAND_UI(ID_MOVE_BEZIER, OnUpdateMoveBezier)
	ON_COMMAND(ID_BIND_CAMERA_TO_OBJECT, OnBindCameraToObject)
	ON_UPDATE_COMMAND_UI(ID_BIND_CAMERA_TO_OBJECT, OnUpdateBindCameraToObject)
	ON_COMMAND(ID_BIND_CAMERA_OFFSET, OnBindCameraOffset)
	ON_UPDATE_COMMAND_UI(ID_BIND_CAMERA_OFFSET, OnUpdateBindCameraOffset)
	ON_COMMAND(ID_ROTATION_TERRAIN, OnRotationTerrain)
	ON_UPDATE_COMMAND_UI(ID_ROTATION_TERRAIN, OnUpdateRotationTerrain)
	ON_COMMAND(ID_FIX_BEZIER_POINT_WITH_NOW_MODEL, OnFixBezierPointWithNowModel)
	ON_UPDATE_COMMAND_UI(ID_FIX_BEZIER_POINT_WITH_NOW_MODEL, OnUpdateFixBezierPointWithNowModel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAnimationEditorView message handlers

BOOL CAnimationEditorView::PreCreateWindow(CREATESTRUCT& cs) 
{
    if (!CWnd::PreCreateWindow(cs))
        return FALSE;

    cs.dwExStyle |= WS_EX_CLIENTEDGE;
    cs.style &= ~WS_BORDER;
    cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
    ::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

    return TRUE;
}

void CAnimationEditorView::OnPaint() 
{
    CPaintDC dc(this); // device context for painting
    
    // TODO: Add your message handler code here
    
    // Do not call CWnd::OnPaint() for painting messages
}

void CAnimationEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_bLButtonPressed)
    {
        if (m_bBezierEditor)
        {
            CAnmBezier *pBezier = g_Animator.GetBezierManager()->GetSelectedBezier();
            if (pBezier)
            {
                BezierOperation(pBezier, point);
            }
        }
        else
        {
            CAnmObject *pObject = g_Animator.GetObjectManager()->GetSelectedObject();
            if (pObject)
            {
                ObjectOperation(pObject, point);
            }
        }
    }

    if (m_bRButtonPressed)
    {
        float dX = 0.2f * float(point.x - m_ptMouseLast.x);
        float dY = -0.2f * float(point.y - m_ptMouseLast.y);
        if (g_Animator.IsBindCamera())
        {
            CAnmObject *pObject = g_Animator.GetObjectManager()->GetSelectedObject();
            if (pObject && pObject->GetObjectID())
            {
                CAnmObject *pCamera = g_Animator.GetObjectManager()->GetObject(0);
                A3DVECTOR3 vPos = pObject->GetPos() + g_Animator.GetBindCameraOffset();
                A3DVECTOR3 vDis = pCamera->GetPos() - vPos;
                A3DQUATERNION qX = A3DQUATERNION(A3DVECTOR3(0.0f, 1.0f, 0.0f), 0.01f * dX);
                vDis = qX ^ vDis;
                A3DVECTOR3 vA = CrossProduct(vDis, A3DVECTOR3(0.0f, 1.0f, 0.0f));
                vA.Normalize();
                A3DQUATERNION qY = A3DQUATERNION(vA, -0.01f * dY);
                A3DVECTOR3 vDir = qY ^ vDis;
                if (DotProduct(vA, CrossProduct(vDir, A3DVECTOR3(0.0f, 1.0f, 0.0f))) >= 0.0f)
                {
                    pCamera->SetPos(vPos + vDir);
                }
                else
                {
                    pCamera->SetPos(vPos + vDis);
                }
                pCamera->SetDirAndUp(vPos - pCamera->GetPos());
            }
        }
        else
        {
            if (dX)
            {
                g_Animator.DegDelta(dX);
            }
            if (dY)
            {
                g_Animator.PitchDelta(dY);
            }
        }
    }
    m_ptMouseLast = point;
}

void CAnimationEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_bLButtonPressed = true;
    CAnmRayTrace *pRayTrace = g_Animator.GetRayTrace();
    m_pAxes = g_Animator.GetAxes();
    if (m_bBezierEditor)
    {
        SelectObject(-1);
        if (m_eEditorState == STATE_ADD_POINT)
        {
            if (pRayTrace->RayTrace(point.x, point.y, CAnmRayTrace::FILTER_TERRAIN))
            {
                g_Animator.AddBezierPoint(pRayTrace->GetHitPos()+A3DVECTOR3(0.0f, 0.5f, 0.0f));
            }
            m_pAxes->SetVisable(false);
        }
        else if (m_eEditorState == STATE_MOVE_BEZIER)
        {
            if (pRayTrace->RayTrace(point.x, point.y, CAnmRayTrace::FILTER_BEZIER))
            {
                if (pRayTrace->GetTargetType() == CAnmRayTrace::TYPE_AXES)
                {
                    m_pAxes->SetVisable(true);
                    if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_X)
                    {
                        m_fLastHitFractor = pRayTrace->GetHitPos().x;
                    }
                    else if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_Y)
                    {
                        m_fLastHitFractor = pRayTrace->GetHitPos().y;
                    }
                    else if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_Z)
                    {
                        m_fLastHitFractor = pRayTrace->GetHitPos().z;
                    }
                }
            }
        }
        else
        {
            if (pRayTrace->RayTrace(point.x, point.y, CAnmRayTrace::FILTER_BEZIER))
            {
                if (pRayTrace->GetTargetType() == CAnmRayTrace::TYPE_BEZIER_POINT)
                {
                    unsigned int uiID = pRayTrace->GetTargetID();
                    SelectBezierPoint(uiID);
                    SelectBezierSegment(-1, 0);
                }
                else if (pRayTrace->GetTargetType() == CAnmRayTrace::TYPE_BEZIER_SEGMENT_AHCHOR_A)
                {
                    unsigned int uiID = pRayTrace->GetTargetID();
                    SelectBezierSegment(uiID, 1);
                    SelectBezierPoint(-1);
                }
                else if (pRayTrace->GetTargetType() == CAnmRayTrace::TYPE_BEZIER_SEGMENT_AHCHOR_B)
                {
                    unsigned int uiID = pRayTrace->GetTargetID();
                    SelectBezierSegment(uiID, 2);
                    SelectBezierPoint(-1);
                }
                else if (pRayTrace->GetTargetType() == CAnmRayTrace::TYPE_AXES)
                {
                    m_pAxes->SetVisable(true);
                    if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_X)
                    {
                        m_fLastHitFractor = pRayTrace->GetHitPos().x;
                    }
                    else if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_Y)
                    {
                        m_fLastHitFractor = pRayTrace->GetHitPos().y;
                    }
                    else if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_Z)
                    {
                        m_fLastHitFractor = pRayTrace->GetHitPos().z;
                    }
                }
                else
                {
                    SelectBezierPoint(-1);
                    SelectBezierSegment(-1, 0);
                    m_pAxes->SetVisable(false);
                }
            }
            else
            {
                SelectBezierPoint(-1);
                SelectBezierSegment(-1, 0);
                m_pAxes->SetVisable(false);
            }
        }
    }
    else
    {
        if (pRayTrace->RayTrace(point.x, point.y, CAnmRayTrace::FILTER_OBJECT))
        {
            if (pRayTrace->GetTargetType() == CAnmRayTrace::TYPE_OBJECT)
            {
                unsigned int uiID = pRayTrace->GetTargetID();
                SelectObject(uiID);
            }
            else if (pRayTrace->GetTargetType() == CAnmRayTrace::TYPE_AXES)
            {
                m_pAxes->SetVisable(true);
                if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_X)
                {
                    m_fLastHitFractor = pRayTrace->GetHitPos().x;
                }
                else if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_Y)
                {
                    m_fLastHitFractor = pRayTrace->GetHitPos().y;
                }
                else if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_Z)
                {
                    m_fLastHitFractor = pRayTrace->GetHitPos().z;
                }
            }
            else
            {
                SelectObject(-1);
                m_pAxes->SetVisable(false);
            }
        }
        else
        {
            SelectObject(-1);
            m_pAxes->SetVisable(false);
        }
    }
    ::SetCapture(m_hWnd);
    theApp.UpdateList();

}

void CAnimationEditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
    m_bLButtonPressed = false;
    m_bOperating = false;
    ::ReleaseCapture();
}

void CAnimationEditorView::OnRButtonDown(UINT nFlags, CPoint point)
{
    m_bRButtonPressed = true;
    ::SetCapture(m_hWnd);
}

void CAnimationEditorView::OnRButtonUp(UINT nFlags, CPoint point)
{
    m_bRButtonPressed = false;
    ::ReleaseCapture();
}

void CAnimationEditorView::OnTranslation() 
{
	// TODO: Add your command handler code here
	m_eEditorState = STATE_TRANSLATION;
}

void CAnimationEditorView::OnTranslationTerrain() 
{
    // TODO: Add your command handler code here
    m_eEditorState = STATE_TRANSLATION_TERRAIN;
}

void CAnimationEditorView::OnRotation() 
{
	// TODO: Add your command handler code here
	m_eEditorState = STATE_ROTATION;
}

void CAnimationEditorView::OnUpdateTranslation(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (m_eEditorState == STATE_TRANSLATION)
    {
        pCmdUI->SetCheck(1);
    }
    else
    {
        pCmdUI->SetCheck(0);
    }
}

void CAnimationEditorView::OnUpdateTranslationTerrain(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    if (m_eEditorState == STATE_TRANSLATION_TERRAIN)
    {
        pCmdUI->SetCheck(1);
    }
    else
    {
        pCmdUI->SetCheck(0);
    }
}

void CAnimationEditorView::OnUpdateRotation(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_eEditorState == STATE_ROTATION)
    {
        pCmdUI->SetCheck(1);
    }
    else
    {
        pCmdUI->SetCheck(0);
    }
}

void CAnimationEditorView::KeyboardMouseMoveOperation()
{
    m_bLButtonPressed = true;
    OnMouseMove(0, m_ptMouseLast);
    m_bLButtonPressed = false;
}

void CAnimationEditorView::ObjectOperation(CAnmObject *pObject, CPoint point)
{
    m_pAxes = g_Animator.GetAxes();
    CAnmRayTrace *pRayTrace = g_Animator.GetRayTrace();
    const A3DAABB objectAABB = pObject->GetAABB();
    float objectHeight = objectAABB.Maxs.y - objectAABB.Mins.y;
    if (m_eEditorState == STATE_TRANSLATION && !g_Animator.IsBindCamera())
    {
        A3DVECTOR3 vPos = m_pAxes->GetPos();
        A3DVECTOR3 vResult = pObject->GetPos();
        A3DVECTOR3 vCameraPos = g_Animator.GetA3DCamera()->GetPos();
        A3DVECTOR3 vTo(float(point.x), float(point.y), 1.0f);
        A3DViewport* pViewport = g_Animator.GetA3DViewport();
        pViewport->InvTransform(vTo, vTo);
        A3DVECTOR3 vDir = vTo - vCameraPos;
        A3DVECTOR3 vNormal = vCameraPos - vPos;
        if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_X)
        {
            vNormal.x = 0.0f;
        }
        else if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_Y)
        {
            vNormal.y = 0.0f;
        }
        else if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_Z)
        {
            vNormal.z = 0.0f;
        }
        A3DVECTOR3 vHitPos;
        float fDist = DotProduct(vPos, vNormal);
        float fFraction;
        if (CLS_RayToPlane(vCameraPos, vDir, vNormal, fDist, true, vHitPos, &fFraction))
        {
            if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_X)
            {
                vResult.x = vHitPos.x-1;
            }
            else if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_Y)
            {
                vResult.y = vHitPos.y-1-objectHeight;
            }
            else if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_Z)
            {
                vResult.z = vHitPos.z-1;
            }
            if (!m_bOperating)
            {
                m_bOperating = true;
                m_Recorder.RecordObject(pObject->GetObjectID(),
                                        pObject->GetPos(),
                                        pObject->GetDir(),
                                        pObject->GetUp());
            }
            pObject->SetPos(vResult);
            m_pAxes->SetPos(vResult+A3DVECTOR3(0, objectHeight, 0));
        }
    }
    else if (m_eEditorState == STATE_TRANSLATION_TERRAIN && !g_Animator.IsBindCamera())
    {
        if (pRayTrace->RayTrace(point.x, point.y, CAnmRayTrace::FILTER_TERRAIN))
        {
            if (!m_bOperating)
            {
                m_bOperating = true;
                m_Recorder.RecordObject(pObject->GetObjectID(),
                    pObject->GetPos(),
                    pObject->GetDir(),
                    pObject->GetUp());
            }
            pObject->SetPos(pRayTrace->GetHitPos());
            m_pAxes->SetPos(pRayTrace->GetHitPos()+A3DVECTOR3(0, objectHeight, 0));
        }
    }
    else if (m_eEditorState == STATE_ROTATION)
    {
        A3DVECTOR3 vPos = m_pAxes->GetPos();
        A3DVECTOR3 vCameraPos = g_Animator.GetA3DCamera()->GetPos();
        A3DVECTOR3 vTo(float(point.x), float(point.y), 1.0f);
        A3DViewport* pViewport = g_Animator.GetA3DViewport();
        pViewport->InvTransform(vTo, vTo);
        A3DVECTOR3 vDir = vTo - vCameraPos;
        A3DVECTOR3 vNormal = vCameraPos - vPos;
        if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_X)
        {
            vNormal.x = 0.0f;
        }
        else if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_Y)
        {
            vNormal.y = 0.0f;
        }
        else if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_Z)
        {
            vNormal.z = 0.0f;
        }
        A3DVECTOR3 vHitPos;
        float fDist = DotProduct(vPos, vNormal);
        float fFraction;
        if (CLS_RayToPlane(vCameraPos, vDir, vNormal, fDist, true, vHitPos, &fFraction))
        {
            A3DVECTOR3 vDir = pObject->GetDir();
            vDir.Normalize();
            A3DVECTOR3 vUp = pObject->GetUp();
            vUp.Normalize();
            A3DVECTOR3 vRight = CrossProduct(vUp, vDir);
            if (!m_bOperating)
            {
                m_bOperating = true;
                m_Recorder.RecordObject(pObject->GetObjectID(), pObject->GetPos(), vDir, vUp);
            }
            if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_X)
            {
                float fD = (vHitPos.x-m_fLastHitFractor);
                A3DQUATERNION q;
                q.ConvertFromAxisAngle(vRight, fD);
                vDir = q ^ vDir;
                vUp  = q ^ vUp;
                pObject->SetDirAndUp(vDir, vUp);
                m_fLastHitFractor = vHitPos.x;
            }
            else if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_Y)
            {
                float fD = (vHitPos.y-m_fLastHitFractor);
                A3DQUATERNION q;
                q.ConvertFromAxisAngle(vUp, fD);
                vDir = q ^ vDir;
                vUp  = q ^ vUp;
                pObject->SetDirAndUp(vDir, vUp);
                m_fLastHitFractor = vHitPos.y;
            }
            else if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_Z)
            {
                float fD = (vHitPos.z-m_fLastHitFractor);
                A3DQUATERNION q;
                q.ConvertFromAxisAngle(vDir, fD);
                vDir = q ^ vDir;
                vUp  = q ^ vUp;
                pObject->SetDirAndUp(vDir, vUp);
                m_fLastHitFractor = vHitPos.z;
            }
        }
    }
    else if (m_eEditorState == STATE_ROTATION_TERRAIN)
    {
        A3DVECTOR3 vPos = pObject->GetPos();
        A3DVECTOR3 vCameraPos = g_Animator.GetA3DCamera()->GetPos();
        A3DVECTOR3 vTo(float(point.x), float(point.y), 1.0f);
        A3DViewport* pViewport = g_Animator.GetA3DViewport();
        pViewport->InvTransform(vTo, vTo);
        A3DVECTOR3 vDir = vTo - vCameraPos;
        A3DVECTOR3 vNormal(0.0f, 1.0f, 0.0f);
        A3DVECTOR3 vHitPos;
        float fDist = DotProduct(vPos, vNormal);
        float fFraction;
        if (CLS_RayToPlane(vCameraPos, vDir, vNormal, fDist, true, vHitPos, &fFraction))
        {
            A3DVECTOR3 pos = vHitPos - pObject->GetPos();
            pos.y = 0.0f;
            if (!m_bOperating)
            {
                m_bOperating = true;
                m_Recorder.RecordObject(pObject->GetObjectID(),
                    pObject->GetPos(),
                    pObject->GetDir(),
                    pObject->GetUp());
            }
            pObject->SetDirAndUp(pos);
        }
    }
	
}

void CAnimationEditorView::BezierOperation(CAnmBezier *pBezier, CPoint point)
{
	
    m_pAxes = g_Animator.GetAxes();
    CAnmBezierPoint *pBezierPoint = pBezier->GetSelectedPoint();
    CAnmBezierSegment *pBezierSegment = pBezier->GetSelectedSegment();
    if (pBezier || pBezierPoint || pBezierSegment)
    {
        CAnmRayTrace *pRayTrace = g_Animator.GetRayTrace();
        if (m_eEditorState == STATE_TRANSLATION)
        {
            A3DVECTOR3 vPos = m_pAxes->GetPos();
            A3DVECTOR3 vResult;
            if (pBezierPoint)
            {
                vResult = pBezierPoint->GetPos();
            }
            else if (pBezierSegment)
            {
                if (pBezier->GetSelectedSegmentAnchor() == 1)
                {
                    vResult = pBezierSegment->GetAnchorA();
                }
                else if (pBezier->GetSelectedSegmentAnchor() == 2)
                {
                    vResult = pBezierSegment->GetAnchorB();
                }
            }
            A3DVECTOR3 vCameraPos = g_Animator.GetA3DCamera()->GetPos();
            A3DVECTOR3 vTo(float(point.x), float(point.y), 1.0f);
            A3DViewport* pViewport = g_Animator.GetA3DViewport();
            pViewport->InvTransform(vTo, vTo);
            A3DVECTOR3 vDir = vTo - vCameraPos;
            A3DVECTOR3 vNormal = vCameraPos - vPos;
            if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_X)
            {
                vNormal.x = 0.0f;
            }
            else if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_Y)
            {
                vNormal.y = 0.0f;
            }
            else if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_Z)
            {
                vNormal.z = 0.0f;
            }
            A3DVECTOR3 vHitPos;
            float fDist = DotProduct(vPos, vNormal);
            float fFraction;
            if (CLS_RayToPlane(vCameraPos, vDir, vNormal, fDist, true, vHitPos, &fFraction))
            {
                if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_X)
                {
                    vResult.x = vHitPos.x-1;
                }
                else if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_Y)
                {
                    vResult.y = vHitPos.y-1;
                }
                else if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_Z)
                {
                    vResult.z = vHitPos.z-1;
                }

                if (pBezierPoint)
                {
                    if (!m_bOperating)
                    {
                        m_bOperating = true;
                        m_Recorder.RecordBezierPoint(pBezier->GetID(),
                            pBezierPoint->GetID(),
                            pBezierPoint->GetPos(),
                            pBezierPoint->GetOrientation());
                    }
                    pBezier->SetPointPos(pBezierPoint, vResult);
                }
                else if (pBezierSegment)
                {
                    if (!m_bOperating)
                    {
                        m_bOperating = true;
                        int anchor = pBezier->GetSelectedSegmentAnchor();
                        A3DVECTOR3 pos;
                        if (anchor == 1)
                        {
                            pos = pBezierSegment->GetAnchorA();
                        }
                        else if (anchor == 2)
                        {
                            pos = pBezierSegment->GetAnchorB();
                        }
                        m_Recorder.RecordBezierAnchor(pBezier->GetID(),
                            pBezierSegment->GetID(), anchor, pos);
                    }
                    pBezier->SetSegmentAnchorPos(pBezierSegment, vResult,
                            pBezier->GetSelectedSegmentAnchor());
                }
                m_pAxes->SetPos(vResult);
            }
        }
        else if (m_eEditorState == STATE_TRANSLATION_TERRAIN)
        {
            if (pRayTrace->RayTrace(point.x, point.y, CAnmRayTrace::FILTER_TERRAIN))
            {
                A3DVECTOR3 vPos = pRayTrace->GetHitPos()+A3DVECTOR3(0.0f, 0.5f, 0.0f);
                if (pBezierPoint)
                {
                    if (!m_bOperating)
                    {
                        m_bOperating = true;
                        m_Recorder.RecordBezierPoint(pBezier->GetID(),
                            pBezierPoint->GetID(),
                            pBezierPoint->GetPos(),
                            pBezierPoint->GetOrientation());
                    }
                    pBezier->SetPointPos(pBezierPoint, vPos);
                }
                else if (pBezierSegment)
                {
                    if (!m_bOperating)
                    {
                        m_bOperating = true;
                        int anchor = pBezier->GetSelectedSegmentAnchor();
                        A3DVECTOR3 pos;
                        if (anchor == 1)
                        {
                            pos = pBezierSegment->GetAnchorA();
                        }
                        else if (anchor == 2)
                        {
                            pos = pBezierSegment->GetAnchorB();
                        }
                        m_Recorder.RecordBezierAnchor(pBezier->GetID(),
                            pBezierSegment->GetID(), anchor, pos);
                    }
                    pBezier->SetSegmentAnchorPos(pBezierSegment, vPos,
                            pBezier->GetSelectedSegmentAnchor());
                }
                m_pAxes->SetPos(vPos);
            }
        }
        else if (m_eEditorState == STATE_ROTATION)
        {
            if (pBezierPoint)
            {
                A3DVECTOR3 vPos = m_pAxes->GetPos();
                A3DVECTOR3 vCameraPos = g_Animator.GetA3DCamera()->GetPos();
                A3DVECTOR3 vTo(float(point.x), float(point.y), 1.0f);
                A3DViewport* pViewport = g_Animator.GetA3DViewport();
                pViewport->InvTransform(vTo, vTo);
                A3DVECTOR3 vDir = vTo - vCameraPos;
                A3DVECTOR3 vNormal = vCameraPos - vPos;
                if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_X)
                {
                    vNormal.x = 0.0f;
                }
                else if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_Y)
                {
                    vNormal.y = 0.0f;
                }
                else if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_Z)
                {
                    vNormal.z = 0.0f;
                }
                A3DVECTOR3 vHitPos;
                float fDist = DotProduct(vPos, vNormal);
                float fFraction;
                if (CLS_RayToPlane(vCameraPos, vDir, vNormal, fDist, true, vHitPos, &fFraction))
                {
                    if (!m_bOperating)
                    {
                        m_bOperating = true;
                        m_Recorder.RecordBezierPoint(pBezier->GetID(),
                        pBezierPoint->GetID(),
                        pBezierPoint->GetPos(),
                        pBezierPoint->GetOrientation());
                    }
                    if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_X)
                    {
                        pBezierPoint->YawDelta(vHitPos.x-m_fLastHitFractor);
                        m_fLastHitFractor = vHitPos.x;
                    }
                    else if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_Y)
                    {
                        pBezierPoint->PitchDelta(vHitPos.y-m_fLastHitFractor);
                        m_fLastHitFractor = vHitPos.y;
                    }
                    else if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_Z)
                    {
                        pBezierPoint->RollDelta(vHitPos.z-m_fLastHitFractor);
                        m_fLastHitFractor = vHitPos.z;
                    }
                }
            }
        }
        else if (m_eEditorState == STATE_MOVE_BEZIER)
        {
            A3DVECTOR3 vPos = m_pAxes->GetPos();
            A3DVECTOR3 vResult = vPos;
            A3DVECTOR3 vCameraPos = g_Animator.GetA3DCamera()->GetPos();
            A3DVECTOR3 vTo(float(point.x), float(point.y), 1.0f);
            A3DViewport* pViewport = g_Animator.GetA3DViewport();
            pViewport->InvTransform(vTo, vTo);
            A3DVECTOR3 vDir = vTo - vCameraPos;
            A3DVECTOR3 vNormal = vCameraPos - vPos;
            if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_X)
            {
                vNormal.x = 0.0f;
            }
            else if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_Y)
            {
                vNormal.y = 0.0f;
            }
            else if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_Z)
            {
                vNormal.z = 0.0f;
            }
            A3DVECTOR3 vHitPos;
            float fDist = DotProduct(vPos, vNormal);
            float fFraction;
            if (CLS_RayToPlane(vCameraPos, vDir, vNormal, fDist, true, vHitPos, &fFraction))
            {
                if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_X)
                {
                    vResult.x = vHitPos.x-1;
                }
                else if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_Y)
                {
                    vResult.y = vHitPos.y-1;
                }
                else if (m_pAxes->GetSelectedAxis() == CAnmAxes::TYPE_Z)
                {
                    vResult.z = vHitPos.z-1;
                }
                
                if (pBezier)
                {
                    pBezier->Move(vResult - vPos);
                }
                m_pAxes->SetPos(vResult);
            }
        }
    }
	
}

void CAnimationEditorView::SetOperation(Operation *op)
{
	
    if (op)
    {
        m_pAxes = g_Animator.GetAxes();
        if (op->target == OT_OBJECT)
        {
            CAnmObject *pObject = g_Animator.GetObjectManager()->GetObject(op->objectID);
            if (pObject)
            {
                pObject->SetPos(op->position);
                pObject->SetDirAndUp(op->direction, op->up);
                const A3DAABB objectAABB = pObject->GetAABB();
                float objectHeight = objectAABB.Maxs.y - objectAABB.Mins.y;
                m_pAxes->SetPos(op->position + A3DVECTOR3(0, objectHeight, 0));
            }
        }
        else if (op->target == OT_BEZIER_POINT)
        {
            CAnmBezier *pBezier = g_Animator.GetBezierManager()->GetBezier(op->bezierID);
            if (pBezier)
            {
                CAnmBezierPoint *pBezierPoint = pBezier->GetPoint(op->bezierPointID);
                if (pBezierPoint)
                {
                    pBezier->SetPointPos(pBezierPoint, op->position);
                    pBezierPoint->SetOrientation(op->orientation);
                    m_pAxes->SetPos(op->position);
                }
            }
        }
        else if (op->target == OT_BEZIER_ANCHOR)
        {
            CAnmBezier *pBezier = g_Animator.GetBezierManager()->GetBezier(op->bezierID);
            if (pBezier)
            {
                CAnmBezierSegment *pBezierSegment = pBezier->GetSegment(op->bezierSegmentID);
                if (pBezierSegment)
                {
                    pBezier->SetSegmentAnchorPos(pBezierSegment, op->position, op->bezierAnchor);
                    m_pAxes->SetPos(op->position);
                }
            }
        }
    }
	
}

void CAnimationEditorView::ClearOperation()
{
    m_Recorder.ClearAllRecords();
}

void CAnimationEditorView::OnAddBezierPoint() 
{
	// TODO: Add your command handler code here
	m_eEditorState = STATE_ADD_POINT;
}

void CAnimationEditorView::OnRemoveBezierPoint() 
{
	// TODO: Add your command handler code here
    if (m_bBezierEditor)
    {
	    CAnmBezierManager *pBezierManager = g_Animator.GetBezierManager();
        CAnmBezier *pBezier = pBezierManager->GetSelectedBezier();
        if (pBezier)
        {
            pBezier->RemovePoint();
            theApp.UpdateList();
        }
    }
}

void CAnimationEditorView::OnUpdateAddBezierPoint(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_eEditorState == STATE_ADD_POINT)
    {
        pCmdUI->SetCheck(1);
    }
    else
    {
        pCmdUI->SetCheck(0);
    }

    if (m_bBezierEditor)
    {
        CAnmBezier *pBezier = g_Animator.GetBezierManager()->GetSelectedBezier();
        if (pBezier)
        {
            pCmdUI->Enable(true);
            return;
        }
    }
    pCmdUI->Enable(false);
}

void CAnimationEditorView::OnUpdateRemoveBezierPoint(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
    if (m_bBezierEditor)
    {
        CAnmBezier *pBezier = g_Animator.GetBezierManager()->GetSelectedBezier();
        if (pBezier && pBezier->GetPointAmount())
        {
            pCmdUI->Enable(true);
            return;
        }
    }
    pCmdUI->Enable(false);
}

void CAnimationEditorView::OnBezierState()
{
	// TODO: Add your command handler code here
	if (m_bBezierEditor)
    {
        m_bBezierEditor = false;
    }
    else
    {
        m_bBezierEditor = true;
    }
}

void CAnimationEditorView::OnUpdateBezierState(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (m_bBezierEditor)
    {
        pCmdUI->SetCheck(1);
    }
    else
    {
        pCmdUI->SetCheck(0);
    }
}

void CAnimationEditorView::OnSelection() 
{
	// TODO: Add your command handler code here
	m_eEditorState = STATE_SELECTION;
}

void CAnimationEditorView::OnUpdateSelection(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_eEditorState == STATE_SELECTION)
    {
        pCmdUI->SetCheck(1);
    }
    else
    {
        pCmdUI->SetCheck(0);
    }
}


void CAnimationEditorView::OnAddBezierPointWithNowCamera() 
{
	// TODO: Add your command handler code here
    A3DVECTOR3 pos = g_Animator.GetA3DCamera()->GetPos();
	g_Animator.AddBezierPoint(pos);
    theApp.UpdateList();
    CAnmBezier *pBezier = g_Animator.GetBezierManager()->GetSelectedBezier();
    if (pBezier)
    {
        std::vector<unsigned int> vList;
        pBezier->GetPointIDList(vList);
        int p = vList.size() - 1;
        if (p >= 0)
        {
            CAnmBezierPoint *pBezierPoint = pBezier->GetPoint(vList[p]);
            CAnmObject *pObject = g_Animator.GetObjectManager()->GetObject(0);
            if (pBezierPoint && pObject)
            {
                pObject->UpdateOrientation();
                pBezierPoint->SetOrientation(pObject->GetOrientation());
            }
        }
    }
}

void CAnimationEditorView::OnAddBezierPointWithNowModel() 
{
	// TODO: Add your command handler code here
    CAnmObject *pObject = g_Animator.GetObjectManager()->GetSelectedObject();
    if (pObject)
    {
        A3DVECTOR3 pos = pObject->GetPos();
        g_Animator.AddBezierPoint(pos);
        theApp.UpdateList();
        CAnmBezier *pBezier = g_Animator.GetBezierManager()->GetSelectedBezier();
        if (pBezier)
        {
            std::vector<unsigned int> vList;
            pBezier->GetPointIDList(vList);
            int p = vList.size() - 1;
            if (p >= 0)
            {
                CAnmBezierPoint *pBezierPoint = pBezier->GetPoint(vList[p]);
                if (pBezierPoint)
                {
                    pObject->UpdateOrientation();
                    pBezierPoint->SetOrientation(pObject->GetOrientation());
                }
            }
        }
    }
}

void CAnimationEditorView::OnUpdateAddBezierPointWithNowCamera(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_bBezierEditor)
    {
        CAnmBezier *pBezier = g_Animator.GetBezierManager()->GetSelectedBezier();
        if (pBezier)
        {
            pCmdUI->Enable(true);
            return;
        }
    }
    pCmdUI->Enable(false);
}

void CAnimationEditorView::OnUpdateAddBezierPointWithNowModel(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    if (m_bBezierEditor)
    {
        CAnmObject *pObject = g_Animator.GetObjectManager()->GetSelectedObject();
        CAnmBezier *pBezier = g_Animator.GetBezierManager()->GetSelectedBezier();
        if (pObject && pBezier)
        {
            pCmdUI->Enable(true);
            return;
        }
    }
    pCmdUI->Enable(false);
}

void CAnimationEditorView::OnFixBezierPointWithNowCamera() 
{
	// TODO: Add your command handler code here

    CAnmBezier *pBezier = g_Animator.GetBezierManager()->GetSelectedBezier();
    if (pBezier)
    {
        CAnmObject *pObject = g_Animator.GetObjectManager()->GetObject(0);
        CAnmBezierPoint *pBezierPoint = pBezier->GetSelectedPoint();
        if (pBezierPoint && pObject)
        {
            if (!m_bOperating)
            {
                m_bOperating = true;
                m_Recorder.RecordBezierPoint(pBezier->GetID(),
                    pBezierPoint->GetID(),
                    pBezierPoint->GetPos(),
                    pBezierPoint->GetOrientation());
            }
            pBezier->SetPointPos(pBezierPoint, pObject->GetPos());
            pObject->UpdateOrientation();
            pBezierPoint->SetOrientation(pObject->GetOrientation());
            return;
        }

        CAnmBezierSegment *pBezierSegment = pBezier->GetSelectedSegment();
        if (pBezierSegment && pObject)
        {
            if (!m_bOperating)
            {
                m_bOperating = true;
                int anchor = pBezier->GetSelectedSegmentAnchor();
                A3DVECTOR3 pos;
                if (anchor == 1)
                {
                    pos = pBezierSegment->GetAnchorA();
                }
                else if (anchor == 2)
                {
                    pos = pBezierSegment->GetAnchorB();
                }
                m_Recorder.RecordBezierAnchor(pBezier->GetID(), pBezierSegment->GetID(), anchor, pos);
            }
            pBezier->SetSegmentAnchorPos(pBezierSegment, pObject->GetPos(), pBezier->GetSelectedSegmentAnchor());
        }
    }
	
}

void CAnimationEditorView::OnUpdateFixBezierPointWithNowCamera(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_bBezierEditor)
    {
        CAnmBezier *pBezier = g_Animator.GetBezierManager()->GetSelectedBezier();
        if (pBezier)
        {
            CAnmBezierPoint *pBezierPoint = pBezier->GetSelectedPoint();
            CAnmBezierSegment *pBezierSegment = pBezier->GetSelectedSegment();
            if (pBezierPoint || pBezierSegment)
            {
                pCmdUI->Enable(true);
                return;
            }
        }
    }
    pCmdUI->Enable(false);
}

void CAnimationEditorView::OnDisableCameraAnimation() 
{
	// TODO: Add your command handler code here
    if (g_Animator.IsDisabledCamera())
    {
        g_Animator.DisableCamera(false);
    }
    else
    {
        g_Animator.DisableCamera(true);
    }
}

void CAnimationEditorView::OnUpdateDisableCameraAnimation(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (g_Animator.IsDisabledCamera())
    {
        pCmdUI->SetCheck(1);
    }
    else
    {
        pCmdUI->SetCheck(0);
    }
}

void CAnimationEditorView::OnEditUndo() 
{
	// TODO: Add your command handler code here
    if (m_Recorder.CanUnDo())
    {
        SetOperation(m_Recorder.UnDo());
    }
}

void CAnimationEditorView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_Recorder.CanUnDo())
    {
        pCmdUI->Enable(true);
    }
    else
    {
        pCmdUI->Enable(false);
    }
}

void CAnimationEditorView::OnEditRedo() 
{
	// TODO: Add your command handler code here
    if (m_Recorder.CanReDo())
    {
        SetOperation(m_Recorder.ReDo());
    }
}

void CAnimationEditorView::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_Recorder.CanReDo())
    {
        pCmdUI->Enable(true);
    }
    else
    {
        pCmdUI->Enable(false);
    }
}

void CAnimationEditorView::OnModelToCamera() 
{
	// TODO: Add your command handler code here
    CAnmObject *pObject = g_Animator.GetObjectManager()->GetSelectedObject();
    if (pObject)
    {
        A3DVECTOR3 pos = g_Animator.GetA3DCamera()->GetPos();
        pObject->SetPos(pos + g_Animator.GetA3DCamera()->GetDir());
    }
}

void CAnimationEditorView::OnUpdateModelToCamera(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    CAnmObjectManager *pManager = g_Animator.GetObjectManager();
    if (pManager)
    {
        CAnmObject *pObject = pManager->GetSelectedObject();
        if (pObject)
        {
            pCmdUI->Enable(true);
            return;
        }
    }
    pCmdUI->Enable(false);
}

void CAnimationEditorView::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CWnd::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
//	g_AnmSystem.NotifyWndSizeChanged(lpwndpos->cx, lpwndpos->cy);

	if( g_pGame->GetConfigs() && !g_pGame->GetConfigs()->IsAdjusting() && g_pGame->GetA3DDevice() )
	{		
		EC_SYSTEM_SETTING ss = g_pGame->GetConfigs()->GetSystemSettings();
		ss.iRndWidth = lpwndpos->cx;
		ss.iRndHeight = lpwndpos->cy;
		g_pGame->GetA3DEngine()->SetDisplayMode(ss.iRndWidth, ss.iRndHeight, A3DFMT_UNKNOWN, !ss.bFullScreen, ss.bVSync, SDM_WIDTH | SDM_HEIGHT | SDM_FORMAT | SDM_WINDOW | SDM_VSYNC);

#ifdef BACK_VERSION
		CECViewport* pViewport = g_pGame->GetViewport();
#else
		ECViewport* pViewport = g_pGame->GetViewport();
#endif
		pViewport->Move(0,0,ss.iRndWidth,ss.iRndHeight,false);	

		AfxGetGFXExMan()->Resize2DViewport();
	}
}

void CAnimationEditorView::OnMoveBezier() 
{
	// TODO: Add your command handler code here
    m_pAxes = g_Animator.GetAxes();
    CAnmBezier *pBezier = g_Animator.GetBezierManager()->GetSelectedBezier();
    if (pBezier && pBezier->GetPointAmount())
    {
        m_eEditorState = STATE_MOVE_BEZIER;
        m_pAxes->SetVisable(true);
        m_pAxes->SetPos(pBezier->GetFirstPoint()->GetPos());
    }
    else
    {
        m_pAxes->SetVisable(false);
    }
}

void CAnimationEditorView::OnUpdateMoveBezier(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_eEditorState == STATE_MOVE_BEZIER)
    {
        pCmdUI->SetCheck(1);
    }
    else
    {
        pCmdUI->SetCheck(0);
    }
    if (m_bBezierEditor)
    {
        CAnmBezier *pBezier = g_Animator.GetBezierManager()->GetSelectedBezier();
        if (pBezier && pBezier->GetPointAmount())
        {
            pCmdUI->Enable(true);
            return;
        }
    }
    pCmdUI->Enable(false);
    if (m_eEditorState == STATE_MOVE_BEZIER)
    {
        pCmdUI->SetCheck(0);
        m_eEditorState = STATE_SELECTION;
    }
}

void CAnimationEditorView::OnBindCameraToObject()
{
    // TODO: Add your command handler code here
    if (g_Animator.IsBindCamera())
    {
        g_Animator.BindCamera(false);
    }
    else
    {
        g_Animator.BindCamera(true);
        if (g_Animator.IsBindCamera())
        {
            CAnmObjectManager *pObjectManager = g_Animator.GetObjectManager();
            if (pObjectManager)
            {
                CAnmObject *pObject = pObjectManager->GetSelectedObject();
                CAnmObject *pCamera = g_Animator.GetObjectManager()->GetObject(0);
                if (pObject && pObject->GetObjectID() && pCamera)
                {
                    A3DVECTOR3 vPos = pObject->GetPos() + g_Animator.GetBindCameraOffset();
                    A3DVECTOR3 vCam = pCamera->GetPos();
                    pCamera->SetDirAndUp(vPos - vCam);
                }
            }
        }
    }
}

void CAnimationEditorView::OnUpdateBindCameraToObject(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    CAnmObjectManager *pObjectManager = g_Animator.GetObjectManager();
    if (pObjectManager)
    {
        CAnmObject *pObject = pObjectManager->GetSelectedObject();
        if (pObject && pObject->GetObjectID())
        {
            pCmdUI->Enable(true);
            if (g_Animator.IsBindCamera())
            {
                pCmdUI->SetCheck(1);
            }
            else
            {
                pCmdUI->SetCheck(0);
            }
            return;
        }
    }
    pCmdUI->Enable(false);
    pCmdUI->SetCheck(0);
}


void CAnimationEditorView::OnBindCameraOffset() 
{
	// TODO: Add your command handler code here
    if (g_Animator.IsBindCamera())
    {
        CBindCameraDlg dlg(this);
        dlg.DoModal();
        CAnmObjectManager *pObjectManager = g_Animator.GetObjectManager();
        if (pObjectManager)
        {
            CAnmObject *pObject = pObjectManager->GetSelectedObject();
            CAnmObject *pCamera = g_Animator.GetObjectManager()->GetObject(0);
            if (pObject && pObject->GetObjectID() && pCamera)
            {
                A3DVECTOR3 vPos = pObject->GetPos() + g_Animator.GetBindCameraOffset();
                A3DVECTOR3 vCam = pCamera->GetPos();
                pCamera->SetDirAndUp(vPos - vCam);
            }
        }
    }
}

void CAnimationEditorView::OnUpdateBindCameraOffset(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (g_Animator.IsBindCamera())
    {
        pCmdUI->Enable(true);
    }
    else
    {
        pCmdUI->Enable(false);
    }
}

void CAnimationEditorView::OnRotationTerrain() 
{
	// TODO: Add your command handler code here
	m_eEditorState = STATE_ROTATION_TERRAIN;
}

void CAnimationEditorView::OnUpdateRotationTerrain(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_eEditorState == STATE_ROTATION_TERRAIN)
    {
        pCmdUI->SetCheck(1);
    }
    else
    {
        pCmdUI->SetCheck(0);
    }
}

void CAnimationEditorView::OnFixBezierPointWithNowModel() 
{
	// TODO: Add your command handler code here
    CAnmObject *pObject = g_Animator.GetObjectManager()->GetSelectedObject();
    CAnmBezier *pBezier = g_Animator.GetBezierManager()->GetSelectedBezier();
    if (pObject && pBezier)
    {
        CAnmBezierPoint *pBezierPoint = pBezier->GetSelectedPoint();
        if (pBezierPoint)
        {
            if (!m_bOperating)
            {
                m_bOperating = true;
                m_Recorder.RecordBezierPoint(pBezier->GetID(),
                    pBezierPoint->GetID(),
                    pBezierPoint->GetPos(),
                    pBezierPoint->GetOrientation());
            }
            pBezier->SetPointPos(pBezierPoint, pObject->GetPos());
            pObject->UpdateOrientation();
            pBezierPoint->SetOrientation(pObject->GetOrientation());
            return;
        }
        
        CAnmBezierSegment *pBezierSegment = pBezier->GetSelectedSegment();
        if (pBezierSegment)
        {
            if (!m_bOperating)
            {
                m_bOperating = true;
                int anchor = pBezier->GetSelectedSegmentAnchor();
                A3DVECTOR3 pos;
                if (anchor == 1)
                {
                    pos = pBezierSegment->GetAnchorA();
                }
                else if (anchor == 2)
                {
                    pos = pBezierSegment->GetAnchorB();
                }
                m_Recorder.RecordBezierAnchor(pBezier->GetID(), pBezierSegment->GetID(), anchor, pos);
            }
            pBezier->SetSegmentAnchorPos(pBezierSegment, pObject->GetPos(), pBezier->GetSelectedSegmentAnchor());
        }
    }
	
}

void CAnimationEditorView::OnUpdateFixBezierPointWithNowModel(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    // TODO: Add your command update UI handler code here
    
	if (m_bBezierEditor)
    {
        CAnmObject *pObject = g_Animator.GetObjectManager()->GetSelectedObject();
        CAnmBezier *pBezier = g_Animator.GetBezierManager()->GetSelectedBezier();
        if (pObject && pBezier)
        {
            CAnmBezierPoint *pBezierPoint = pBezier->GetSelectedPoint();
            CAnmBezierSegment *pBezierSegment = pBezier->GetSelectedSegment();
            if (pBezierPoint || pBezierSegment)
            {
                pCmdUI->Enable(true);
                return;
            }
        }
    }
	
    pCmdUI->Enable(false);
}

