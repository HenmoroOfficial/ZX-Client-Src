// BezierAddOperation.cpp: implementation of the CBezierAddOperation class.
//
//////////////////////////////////////////////////////////////////////

#include "global.h"
#include "elementeditor.h"
#include "BezierAddOperation.h"
#include "SceneObjectManager.h"
#include "SceneLightObject.h"
#include "Render.h"
#include "a3d.h"
#include "BezierUndoAction.h"
#include "UndoMan.h"
#include "UndoLightAddAction.h"
#include "MainFrm.h"
#include "SceneObjectMultiSelectOperation.h"
#include "Box3D.h"
#include "InputID.h"

//#define new A_DEBUG_NEW

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBezierAddOperation::CBezierAddOperation()
{
	m_nPointID = 0;
	m_nBezierID = 0;
	m_pBezier = NULL; 
}

CBezierAddOperation::~CBezierAddOperation()
{
	m_tempBezier.Release();
}

bool CBezierAddOperation::OnLButtonDown(int x, int y, DWORD dwFlags)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		pMap->SetModifiedFlag(true);
		CSceneObjectManager *pSManager = pMap->GetSceneObjectMan();
		LIGHTDATA lightData;
		
		A3DVECTOR3 vPos((float)x, (float)y, 1.0f);
		A3DCamera*pCamera = (A3DCamera*)(g_Render.GetA3DEngine())->GetActiveCamera();
		A3DViewport* pViewport =g_Render.GetA3DEngine()->GetActiveViewport();
		A3DVECTOR3 vStart((float)x, (float)y, 0.0f);
		pViewport->InvTransform(vStart,vStart);
		pViewport->InvTransform(vPos,vPos);
		
		RAYTRACERT TraceRt;	//	Used to store trace result
		if(pSManager->GetTracePos(vStart,vPos,TraceRt.vPoint))
		{	
			if(m_pBezier==NULL)
			{
				
				AUX_GetMainFrame()->GetToolTabWnd()->UpdatePropertyData(true);//读属性数据
				m_tempBezier.UpdateProperty(true);
				m_pBezier = pSManager->CreateBezier(m_tempBezier.GetObjectName());
				if(m_pBezier)
				{
					DWORD dwColor;
					m_tempBezier.GetProperty(dwColor);
					m_pBezier->SetProperty(dwColor);
					m_pBezier->SetBezier(m_tempBezier.IsBezier());

					if(g_Configs.iIsOverseaVersion == 1)
					{
						CInputID dlgID;
						dlgID.m_bShowGlobalID = true;
						dlgID.DoModal();

						m_pBezier->SetObjectID(dlgID.m_nID);
						m_pBezier->SetGlobalID(dlgID.m_nGlobalID);
					}
					else
					{
						m_pBezier->SetObjectID(pSManager->GenerateObjectID());
						int nGlobalID = GenerateGlobalID();
						if(nGlobalID==-1) 
						{
							pSManager->DeleteSceneObjectForever(m_pBezier);
							m_pBezier->Release();
							delete m_pBezier;
							m_pBezier = NULL;
							return false;
						}
						m_pBezier->SetGlobalID(nGlobalID);
					}

					m_pBezier->CreatePoint(PointAlign(TraceRt.vPoint),m_nBezierID,m_nPointID++);
					AUX_GetMainFrame()->GetToolTabWnd()->ShowPropertyPannel(m_pBezier->m_pProperty);//把灯光的属性给对象属性表
					m_pBezier->UpdateProperty(false);
					AUX_GetMainFrame()->GetToolTabWnd()->UpdatePropertyData(false);//输出属性数据
					AUX_GetMainFrame()->GetToolTabWnd()->UpdateSceneObjectList();
					//for undo
					CUndoSceneObjectAddAction *pUndo = new CUndoSceneObjectAddAction(pMap->GetSceneObjectMan());
					pUndo->GetSeletedListData();
					g_UndoMan.AddUndoAction(pUndo);
					ReleaseCapture();
				}else g_Log.Log("CBezierAddOperation::OnLButtonDown, Failed to create bezier");
			}else
			{
				CEditerBezierPoint *pNewPt = m_pBezier->CreatePoint(PointAlign(TraceRt.vPoint),m_nBezierID,m_nPointID++);
				if(m_pBezier->IsBezier()) m_pBezier->OptimizeSmooth();
				m_pBezier->ReCalculateLength();
				if(m_pBezier->m_list_points.GetCount()>1)
					AUX_GetMainFrame()->EnableBezierOperation(true);
			}
		}
		
	}
	return true;
}

bool CBezierAddOperation::OnRButtonUp( int x, int y, DWORD dwFlags)
{
	return true;
}

bool CBezierAddOperation::OnMouseMove(int x,int y,DWORD dwFlags)
{
	SetCursor(g_hAddObjectCursor);
	return true;
}

void CBezierAddOperation::CreateBezier()
{
	InitBezierCreateState();
	int m_nBezierID = 0;
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		AString name;
		CSceneObjectManager *pSManager = pMap->GetSceneObjectMan();
		pSManager->ClearSelectedList();
		while(1)
		{
			name.Format("Bezier_%d",m_nBezierID);
			if(pSManager->FindSceneObject(name)!=NULL)
			{
				m_nBezierID++;
			}else break; 
			
		}
		DWORD color;
		int r = (int)(255*(float)rand()/(float)RAND_MAX);
		int g = (int)(255*(float)rand()/(float)RAND_MAX);
		int b = (int)(255*(float)rand()/(float)RAND_MAX);
		color = A3DCOLORRGB(r,g,b);
		m_tempBezier.SetProperty(color);
		m_tempBezier.SetObjectName(name);
		m_tempBezier.UpdateProperty(false);
		AUX_GetMainFrame()->GetToolTabWnd()->ShowPropertyPannel(m_tempBezier.m_pProperty);//把灯光的属性给对象属性表
	
	}
}

A3DVECTOR3 CBezierAddOperation::PointAlign(A3DVECTOR3 v)
{
	if(!g_bBezierPtSnap) return v;
	float grip = 5.0f;
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CTerrain *pTerrain = pMap->GetTerrain();
		ARectF rc = pTerrain->GetTerrainArea();
		
		if(fabs(rc.left - v.x) < grip)
			v.x = rc.left;
		if(fabs(rc.right - v.x) < grip)
			v.x = rc.right;
		if(fabs(rc.top - v.z) < grip)
			v.z = rc.top;
		if(fabs(rc.bottom - v.z) < grip)
			v.z = rc.bottom;
	}
	
	float mindis = 99999.0f;
	A3DVECTOR3 vs;	
	APtrList<CSceneObject *>* bList = pMap->GetSceneObjectMan()->GetSortObjectList(CSceneObject::SO_TYPE_BEZIER);
	ALISTPOSITION pos = bList->GetHeadPosition();
	while(pos)
	{
		CEditerBezier* pBezier = (CEditerBezier*)bList->GetNext(pos);																																																										
		if(pBezier->IsDeleted() && m_pBezier == pBezier) continue;
		POSITION pos2 = pBezier->m_list_points.GetHeadPosition();
		while( pos2 )
		{
 			CEditerBezierPoint *pt = (CEditerBezierPoint *)pBezier->m_list_points.GetNext(pos2);
			A3DVECTOR3 vPos = pt->GetPos();
			float dis = (v - vPos).Magnitude();
			if(dis < mindis)
			{
				mindis = dis;
				vs = vPos;
			}
		}
	}

	if(mindis < grip) v = vs;
	
	return v;
}

void CBezierAddOperation::InitBezierCreateState()
{
	m_nBezierID++;
	m_nPointID = 0;
	m_pBezier = NULL;
}


CBezierDragOperation::CBezierDragOperation()
{
	m_bHasHit = false;
	m_bMouseDown = false;
	m_nDirectFlag = 0;
	m_pUndo = NULL;
	m_vDelta = A3DVECTOR3(0.0f);
}

CBezierDragOperation::~CBezierDragOperation()
{

}

void CBezierDragOperation::SelectPoint(bool bNext)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pSManager = pMap->GetSceneObjectMan();
		if(pSManager->m_listSelectedObject.GetCount()==1)
		{
			CSceneObject *pObj = pSManager->m_listSelectedObject.GetTail();
			if(pObj->GetObjectType()==CSceneObject::SO_TYPE_BEZIER)
			{	
				if(((CEditerBezier*)pObj)->GetHitFlag()==1)
				{
					CEditerBezierSegment *pSegHead = NULL,*pSegTail = NULL;
					CEditerBezierPoint *pt = ((CEditerBezier*)pObj)->GetCurPoint(&pSegHead,&pSegTail);
					if(pt)
					{
						if(pSegHead && !bNext) ((CEditerBezier*)pObj)->SelectedPoint(pSegHead->GetHeadID());
						if(pSegTail && bNext) ((CEditerBezier*)pObj)->SelectedPoint(pSegTail->GetTailID());
						
						pt = ((CEditerBezier*)pObj)->GetCurPoint(&pSegHead,&pSegTail);
						AUX_GetMainFrame()->ShowBezierPointPro((CEditerBezier*)pObj, pt);
						if(g_bLockCam && pt) 
						{
							g_Render.GetA3DEngine()->GetActiveCamera()->SetPos(pt->GetPos());
							g_Render.GetA3DEngine()->GetActiveCamera()->SetDirAndUp(A3DVECTOR3(pt->GetDirection().x,pt->GetDirection().y,pt->GetDirection().z),A3DVECTOR3(0,1.0f,0));
						}
					}
				}
			}
		}
	}
}

bool CBezierDragOperation::OnLButtonDown(int x, int y, DWORD dwFlags)
{
	m_nOldX = x;
	m_nOldY = y;
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap && !g_bLockCam)
	{
		CSceneObjectManager *pSManager = pMap->GetSceneObjectMan();
		if(pSManager->m_listSelectedObject.GetCount()==1)
		{
			CSceneObject *pObj = pSManager->m_listSelectedObject.GetTail();
			if(pObj)
			{
				CEditerBezierSegment *pSegHead = NULL,*pSegTail = NULL;
				CEditerBezierPoint *pt = NULL;
				if(pObj->GetObjectType()==CSceneObject::SO_TYPE_BEZIER)
				{
					if(m_bHasHit)
					{
						m_bMouseDown = true;
						bool bStart = false;
						
						int nFlag = 0;
						A3DVECTOR3 vDirPos;
						
						//先看是有有显示的坐标可以被点中
						nFlag = ((CEditerBezier*)pObj)->GetHitFlag();
						switch(nFlag)
						{
						case 0:
							m_bHasHit = false;
							break;
						case 1://命中点
							pt = ((CEditerBezier*)pObj)->GetCurPoint(&pSegHead,&pSegTail);
							if(pt) vDirPos = pt->GetPos();
							break;
						case 2://命中段
							pSegHead = ((CEditerBezier*)pObj)->GetCurSegment(vDirPos);
							if(((CEditerBezier*)pObj)->GetTailSeg() != pSegHead)
							{
								if(pSegHead) vDirPos = pSegHead->GetAnchor1();
							}else 
							{
								if(pSegHead) vDirPos = pSegHead->GetAnchor2();
							}
							break;
						case 3://命中控制点
							pSegHead = ((CEditerBezier*)pObj)->GetCurCtrl(bStart);
							if(pSegHead)
							{
								if(bStart) vDirPos = pSegHead->GetAnchor1();
								else vDirPos = pSegHead->GetAnchor2();
								
							}
							break;
						}

						A3DMATRIX4 matR; matR.Identity();
						pSManager->m_coordinateDirection.SetSelectedFlags(0);
						pSManager->m_coordinateDirection.SetRotate(matR);
						pSManager->m_coordinateDirection.SetPos(vDirPos);
						pSManager->m_coordinateDirection.Update();
						pSManager->m_coordinateDirection.OnLButtonDown(x,y,dwFlags);
						m_nDirectFlag = pSManager->m_coordinateDirection.GetSelectedFlags();
						if(m_nDirectFlag==0) m_bHasHit = false;
						else
						{
							CPoint pt(x,y);
							float dot[3];
							SceneObjectMultiSelectOperation::ScreenTraceAxis(vDirPos,pt,&m_vXY,&m_vXZ,&m_vYZ,dot);
							m_vXY = m_vXY - vDirPos;
							m_vXZ = m_vXZ - vDirPos;
							m_vYZ = m_vYZ - vDirPos;
							
							m_pUndo = new CBezierUndoAction();
							m_pUndo->StartRecord();
							return true;
						}
						
					}
					m_bMouseDown = false;
					A3DVECTOR3 vPos((float)x, (float)y, 1.0f);
					A3DCamera*pCamera = (A3DCamera*)(g_Render.GetA3DEngine())->GetActiveCamera();
					A3DViewport* pViewport =g_Render.GetA3DEngine()->GetActiveViewport();
					A3DVECTOR3 vStart((float)x, (float)y, 0.0f);
					pViewport->InvTransform(vStart,vStart);
					pViewport->InvTransform(vPos,vPos);
					int nFlag;
					m_bHasHit = ((CEditerBezier*)pObj)->PickControl(vStart, vPos,nFlag);
					if(m_bHasHit && nFlag==1)
					{
						pt = ((CEditerBezier*)pObj)->GetCurPoint(&pSegHead,&pSegTail);
						AUX_GetMainFrame()->ShowBezierPointPro((CEditerBezier*)pObj, pt);
					}else AUX_GetMainFrame()->HideBezierPointPro();
				}
			}
		}
	}
	return true;
}

bool CBezierDragOperation::OnLButtonUp(int x,int y, DWORD dwFlags)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pSManager = pMap->GetSceneObjectMan();
		if(m_vDelta!=A3DVECTOR3(0.0f))
		{	
			if(m_pUndo)
			{
				g_UndoMan.AddUndoAction(m_pUndo);
				m_pUndo = NULL;
			}
		}else
		{
			if(m_pUndo)
			{
				m_pUndo->Release();
				delete m_pUndo;
				m_pUndo = NULL;
			}
		}
		pSManager->m_coordinateDirection.OnLButtonUp(x,y,dwFlags);
	}
	m_bMouseDown = false;
	return true;
}

bool CBezierDragOperation::OnMouseMove(int x,int y,DWORD dwFlags)
{
	
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pSManager = pMap->GetSceneObjectMan();
		if(pSManager->m_listSelectedObject.GetCount()==1)
		{
			CSceneObject *pObj = pSManager->m_listSelectedObject.GetTail();
			if(m_bMouseDown && m_nDirectFlag!=0)
			{
				SetCursor(g_hMoveCursor);
				CEditerBezierSegment *pSeg = NULL;
				CEditerBezierSegment *pSegHead = NULL,*pSegTail = NULL;
				CEditerBezierPoint *pPoint = NULL;
				A3DVECTOR3 vTarget;
				bool bStartCtrl;
				int nFlag = ((CEditerBezier*)pObj)->GetHitFlag();
				switch(nFlag) 
				{
				case 1:
					pPoint = ((CEditerBezier*)pObj)->GetCurPoint(&pSegTail,&pSegHead);
					if(pPoint)
					{//对点的处理
						A3DVECTOR3 vBezierTrans = GetDeltaVector(pPoint->GetPos(),x,y);
						A3DVECTOR3 vPos = pPoint->GetPos();
						vPos = vPos + vBezierTrans;
						if(!((CEditerBezier*)pObj)->IsBezier())
						{	
							vPos.y = pMap->GetTerrain()->GetPosHeight(vPos) + 0.1f;
						}
						pPoint->SetPos(vPos);
						m_vDelta +=vBezierTrans;
						if(pSegTail)
						{
							if(((CEditerBezier*)pObj)->IsBezier())
							{	
								vPos = pSegTail->GetAnchor2();
								vPos = vPos + vBezierTrans;
							}
							A3DVECTOR3 a2 = pSegTail->GetAnchor2();
							A3DVECTOR3 tail = pSegTail->GetTail();
							
							pSegTail->SetAnchor2(vPos);
							pSegTail->SetTail(pPoint->GetPos());
						}
						if(pSegHead)
						{
							if(((CEditerBezier*)pObj)->IsBezier())
							{
								vPos = pSegHead->GetAnchor1();
								vPos = vPos + vBezierTrans;
							}
							
							A3DVECTOR3 a1 = pSegHead->GetAnchor1();
							A3DVECTOR3 head = pSegHead->GetHead();
							pSegHead->SetAnchor1(vPos);
							pSegHead->SetHead(pPoint->GetPos());
						}
					}
					
					break;
				case 2:
					pSeg = ((CEditerBezier*)pObj)->GetCurSegment(vTarget);
					if(pSeg && ((CEditerBezier*)pObj)->IsBezier())
					{
						if(((CEditerBezier*)pObj)->GetTailSeg() != pSeg)
							vTarget = pSeg->GetAnchor1();
						else vTarget = pSeg->GetAnchor2();
						A3DVECTOR3 vBezierTrans = GetDeltaVector(vTarget,x,y);
						A3DVECTOR3 v1 = pSeg->GetAnchor1();
						A3DVECTOR3 v2 = pSeg->GetAnchor2();
						m_vDelta +=vBezierTrans;
						v1 = v1 + vBezierTrans;
						v2 = v2 + vBezierTrans;
						pSeg->SetAnchor1(v1);
						pSeg->SetAnchor2(v2);
					}
					break;
				case 3:
					pSeg = ((CEditerBezier*)pObj)->GetCurCtrl(bStartCtrl);
					if(pSeg && ((CEditerBezier*)pObj)->IsBezier())
					{
						if(bStartCtrl)
						{
							A3DVECTOR3 v1 = pSeg->GetAnchor1();
							A3DVECTOR3 vBezierTrans = GetDeltaVector(v1,x,y);
							v1 = v1 + vBezierTrans;
							m_vDelta +=vBezierTrans;
							pSeg->SetAnchor1(v1);
						}else
						{
							A3DVECTOR3 v2 = pSeg->GetAnchor2();
							A3DVECTOR3 vBezierTrans = GetDeltaVector(v2,x,y);
							v2 = v2 + vBezierTrans;
							m_vDelta +=vBezierTrans;
							pSeg->SetAnchor2(v2);
						}
					}
					break;
				}
			}else
			{
				CEditerBezierSegment *pSegHead = NULL,*pSegTail = NULL;
				CEditerBezierPoint *pt = NULL;
				bool bStart = false;
				
				int nFlag = 0;
				A3DVECTOR3 vDirPos;
				
				//先看是否有显示的坐标可以被点中
				nFlag = ((CEditerBezier*)pObj)->GetHitFlag();
				switch(nFlag)
				{
				case 1://命中点
					pt = ((CEditerBezier*)pObj)->GetCurPoint(&pSegHead,&pSegTail);
					if(pt)
					{
						vDirPos = pt->GetPos();
					}
					break;
				case 2://命中段
					pSegHead = ((CEditerBezier*)pObj)->GetCurSegment(vDirPos);
					if(pSegHead != ((CEditerBezier*)pObj)->GetTailSeg())
					{
						if(pSegHead) vDirPos = pSegHead->GetAnchor1();
					}else 
					{
						if(pSegHead) vDirPos = pSegHead->GetAnchor2();
					}
					break;
				case 3://命中控制点
					pSegHead = ((CEditerBezier*)pObj)->GetCurCtrl(bStart);
					if(pSegHead)
					{
						if(bStart) vDirPos = pSegHead->GetAnchor1();
						else vDirPos = pSegHead->GetAnchor2();
						
					}
					break;
				}
				A3DMATRIX4 mat;
				mat.Identity();
				pSManager->m_coordinateDirection.SetSelectedFlags(0);
				pSManager->m_coordinateDirection.SetRotate(mat);
				pSManager->m_coordinateDirection.SetPos(vDirPos);
				pSManager->m_coordinateDirection.Update();
				pSManager->m_coordinateDirection.OnMouseMove(x,y,dwFlags);
				m_nDirectFlag = pSManager->m_coordinateDirection.GetSelectedFlags();
				//if(m_nDirectFlag!=0) SetCursor(g_hMoveCursor);
			}
		}
	}
	m_nOldX = x;
	m_nOldY = y;
	return true;
}

void CBezierDragOperation::Render(A3DViewport* pA3DViewport)
{
	//绘制每个对象的坐标指示
	A3DMATRIX4 mat;
	mat.Identity();
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(pManager->m_listSelectedObject.GetCount()==1)
		{
			
			CSceneObject *ptemp = pManager->m_listSelectedObject.GetTail();
			if(ptemp->GetObjectType()!=CSceneObject::SO_TYPE_BEZIER) return;
			CEditerBezierSegment *pSegHead = NULL,*pSegTail = NULL;
			CEditerBezierPoint *pt = NULL;
			bool bStart = false;
			
			int nFlag = 0;
			A3DVECTOR3 vDirPos = A3DVECTOR3(0.0f,0.0f,0.0f);
			nFlag = ((CEditerBezier*)ptemp)->GetHitFlag();
			if(nFlag>0)
			{
				switch(nFlag)
				{
				case 1://命中点
					pt = ((CEditerBezier*)ptemp)->GetCurPoint(&pSegHead,&pSegTail);
					if(pt)
					{
						vDirPos = pt->GetPos();
					}
					break;
				case 2://命中段
					if(!((CEditerBezier*)ptemp)->IsBezier()) return;
					pSegHead = ((CEditerBezier*)ptemp)->GetCurSegment(vDirPos);
					if(pSegHead != ((CEditerBezier*)ptemp)->GetTailSeg())
					{
						if(pSegHead) vDirPos = pSegHead->GetAnchor1();
					}else 
					{
						if(pSegHead) vDirPos = pSegHead->GetAnchor2();
					}
					break;
				case 3://命中控制点
					if(!((CEditerBezier*)ptemp)->IsBezier()) return;
					pSegHead = ((CEditerBezier*)ptemp)->GetCurCtrl(bStart);
					if(pSegHead)
					{
						if(bStart) vDirPos = pSegHead->GetAnchor1();
						else vDirPos = pSegHead->GetAnchor2();
					}
					break;
				}
				pManager->m_coordinateDirection.SetSelectedFlags(m_nDirectFlag);
				pManager->m_coordinateDirection.SetPos(vDirPos);
				pManager->m_coordinateDirection.Update();
				pManager->m_coordinateDirection.SetRotate(mat);
				pManager->m_coordinateDirection.Draw();
			}
		}
	}
}

A3DVECTOR3 CBezierDragOperation::GetDeltaVector(A3DVECTOR3 pos,int x,int y)
{
	A3DVECTOR3 vDelta = A3DVECTOR3(0.0f);
	CPoint pt(x,y);
	
	float dot[3];
	A3DVECTOR3 xy(pos),xz(pos),yz(pos);
	SceneObjectMultiSelectOperation::ScreenTraceAxis(pos,pt,&xy,&xz,&yz,dot);
	switch(m_nDirectFlag)
	{
	case  CCoordinateDirection::AXIS_X: 
		if(dot[0] < dot[1])
		{
			vDelta.x = xz.x - pos.x - m_vXZ.x;
		}
		else 
		{
			vDelta.x = xy.x - pos.x - m_vXY.x;
		}
		break;
		
	case  CCoordinateDirection::AXIS_Y: 
		if(dot[0] < dot[2])
		{
			vDelta.y = yz.y - pos.y - m_vYZ.y;
		}
		else 
		{
			vDelta.y = xy.y - pos.y - m_vXY.y;
		}
		break;
		
	case  CCoordinateDirection::AXIS_Z: 
		if(dot[1] < dot[2])
		{
			vDelta.z = yz.z - pos.z - m_vYZ.z;
		}
		else 
		{
			vDelta.z = xz.z - pos.z - m_vXZ.z;
		}
		break;
		
	case  CCoordinateDirection::PLANE_XY: 
		vDelta.x = xy.x - pos.x - m_vXY.x;
		vDelta.y = xy.y - pos.y - m_vXY.y;
		break;
		
	case  CCoordinateDirection::PLANE_XZ: 
		vDelta.x = xz.x - pos.x - m_vXZ.x;
		vDelta.z = xz.z - pos.z - m_vXZ.z;
		break;
		
	case  CCoordinateDirection::PLANE_YZ: 
		vDelta.y = yz.y - pos.y - m_vYZ.y;
		vDelta.z = yz.z - pos.z - m_vYZ.z;
		break; 
	}
	
	return vDelta;
}

//////////////////////////////////////////////////////////////////////////////
//class BezierTestOperation
//////////////////////////////////////////////////////////////////////////////

CBezierTestOperation::CBezierTestOperation()
{
	m_bStartPlay = false;
	m_pPointTail = 0;
	m_pPointHead = 0;
	m_fCurLength = 0;
	m_fSegLength = 0;
	m_dwEscapeTime = 0;
	m_fAcceleration = 0;
}

CBezierTestOperation::~CBezierTestOperation()
{
}

void CBezierTestOperation::Tick(DWORD dwTimeDelta)
{
	if(m_bStartPlay)
	{
		UpdateCamra(dwTimeDelta);
	}
}

void CBezierTestOperation::UpdateCamra(DWORD dwTimeDelta)
{
	if(m_dwEscapeTime >= (DWORD)m_fTotalTime)
	{
		int nLoop = m_pBezier->GetLoopType();
		
		if(m_bForward) m_pSeg = GetNextSeg();
		else m_pSeg = GetLastSeg();
		if(m_pSeg==NULL)
		{
			if(nLoop==2) m_bForward = !m_bForward;
			
			if(nLoop==1 && m_bForward)
			{
				m_SegPos = m_pBezier->m_list_segments.GetHeadPosition();
				m_pSeg = GetNextSeg();
				if(m_pSeg==NULL) return;
			}else if(nLoop==0) 
			{
				if(m_pBezier->GetNextID()==-1)
					Stop();
				else NextBezier(m_pBezier->GetNextID());
				return;
			}else
			{//反向
				m_SegPos = m_pBezier->m_list_segments.GetTailPosition();
				m_pSeg = GetLastSeg();
				if(m_pSeg==NULL) return;
			}
		}
		
		if(nLoop==2 && !m_bForward)
		{
			
			m_pPointHead = m_pBezier->GetPoint(m_pSeg->GetTailID());
			m_pPointTail = m_pBezier->GetPoint(m_pSeg->GetHeadID());
		}else
		{
			m_pPointTail = m_pBezier->GetPoint(m_pSeg->GetTailID());
			m_pPointHead = m_pBezier->GetPoint(m_pSeg->GetHeadID());
		}
		
		m_pSeg->ReCalculateLength();
		m_fSegLength = m_pSeg->GetLenght();
		m_fCurLength = 0;
		m_dwEscapeTime = dwTimeDelta;
		m_fTotalTime = 0;
		m_fAcceleration = 0;
		if(m_pPointHead->GetSpeed() + m_pPointTail->GetSpeed() != 0 ) 
		{
			m_fTotalTime = 2*m_fSegLength/(m_pPointHead->GetSpeed() + m_pPointTail->GetSpeed());
			m_fAcceleration = (m_pPointTail->GetSpeed() - m_pPointHead->GetSpeed())/m_fTotalTime;
			m_fTotalTime *= 1000;
		}else m_bStartPlay = FALSE;
	}
	
	m_dwEscapeTime += dwTimeDelta;
	float fEscape = m_dwEscapeTime/1000.0f;
	m_fCurLength = m_pPointHead->GetSpeed()*fEscape + 0.5f*m_fAcceleration*fEscape*fEscape;
	float u = m_fCurLength/m_fSegLength;
	if(u > 1.0f) u = 1.0f;
	A3DVECTOR3 pos;
	if(m_pBezier->GetLoopType()==2 && !m_bForward) pos = m_pSeg->Bezier(1.0f-u);
	else pos = m_pSeg->Bezier(u);
	
	A3DVECTOR3 dir = m_pPointHead->GetDirection() + (m_pPointTail->GetDirection() - m_pPointHead->GetDirection())*u;
	float      ffov = m_pPointHead->GetFov() + (m_pPointTail->GetFov() - m_pPointHead->GetFov())*u;
	float      fratio = m_pPointHead->GetRatio() + ( m_pPointTail->GetRatio() - m_pPointHead->GetRatio())*u;
	
	A3DCamera *pca = (A3DCamera*) g_Render.GetA3DEngine()->GetActiveCamera();
	if(pca)
	{
		float parm[4];
		pca->GetProjectionParam(&parm[0],&parm[1],&parm[2],&parm[3]);
		pca->SetProjectionParam(ffov/180.8*A3D_PI,parm[1],parm[2],fratio);
		
		pca->SetPos(pos);
		pca->SetDirAndUp(dir,A3DVECTOR3(0.0f,1.0f,0.0f));
	}
}

CEditerBezierSegment* CBezierTestOperation::GetNextSeg()
{
	if(m_SegPos)
		return (CEditerBezierSegment*)m_pBezier->m_list_segments.GetNext(m_SegPos);
	else return NULL;
}

CEditerBezierSegment* CBezierTestOperation::GetLastSeg()
{
	if(m_SegPos)
		return (CEditerBezierSegment*)m_pBezier->m_list_segments.GetPrev(m_SegPos);
	else return NULL;
}

void CBezierTestOperation::Play()
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(pManager->m_listSelectedObject.GetCount()==1)
		{
			CEditerBezier *pBezier = (CEditerBezier *)pManager->m_listSelectedObject.GetHead();
			if(pBezier==NULL) return;
			if(pBezier->GetObjectType()!=CSceneObject::SO_TYPE_BEZIER) return;
			m_pBezier = pBezier;
			m_SegPos = m_pBezier->m_list_segments.GetHeadPosition();
			Aux_CaptureCamera();
			m_pPointTail = 0;
			m_pPointHead = 0;
			m_fCurLength = 0;
			m_fSegLength = 0;
			m_dwEscapeTime = 0;
			m_fAcceleration = 0;
			m_fTotalTime = 0;
			m_bStartPlay = true;
			m_bForward = true;
		}
	}
	
}

void CBezierTestOperation::NextBezier(int id)
{
	CEditerBezier *pBezier = (CEditerBezier *)(AUX_GetMainFrame()->GetMap()->GetSceneObjectMan()->FindBezierByGlobalID(id));
	if(pBezier==NULL)
	{
		Stop();
		return;
	};

	m_pBezier = pBezier;
	m_SegPos = m_pBezier->m_list_segments.GetHeadPosition();
	Aux_CaptureCamera();
	m_pPointTail = 0;
	m_pPointHead = 0;
	m_fCurLength = 0;
	m_fSegLength = 0;
	m_dwEscapeTime = 0;
	m_fAcceleration = 0;
	m_fTotalTime = 0;
	m_bStartPlay = true;
	m_bForward = true;
}

void CBezierTestOperation::Stop()
{
	m_bStartPlay = false;
	Aux_RestoreCamera();
}

bool CBezierTestOperation::OnLButtonDown(int x, int y, DWORD dwFlags)
{
	//if(m_bStartPlay) Stop();
	//else Play();
	return false;
}


