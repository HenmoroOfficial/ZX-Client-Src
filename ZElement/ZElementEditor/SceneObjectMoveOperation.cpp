// SceneObjectMoveOperation.cpp: implementation of the CSceneObjectMoveOperation class.
//
//////////////////////////////////////////////////////////////////////


#include "Global.h"
#include "elementeditor.h"
#include "SceneObjectMoveOperation.h"
#include "SceneWaterObject.h"
#include "MainFrm.h"
#include "A3D.h"
#include "render.h"
#include "SceneObjectManager.h"
#include "UndoObjectMoveAction.h"
#include "BezierUndoAction.h"
#include "UndoMan.h"

//#define new A_DEBUG_NEW

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSceneObjectMoveOperation::CSceneObjectMoveOperation()
{
	m_bRotate = false;
	m_vDelta = A3DVECTOR3(0.0f,0.0f,0.0f);
	m_pUndo = NULL;
}

CSceneObjectMoveOperation::~CSceneObjectMoveOperation()
{

}

bool CSceneObjectMoveOperation::OnMouseMove(int x, int y, DWORD dwFlags)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	A3DVECTOR3 vTrans = A3DVECTOR3(0.0f,0.0f,0.0f);
	SceneObjectMultiSelectOperation::OnMouseMove(x,y,dwFlags);
	if(pMap && (!m_bDrawRect))
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(pManager==NULL) return true;
		pManager->m_coordinateDirection.SetCursor(g_hMoveCursor);
		if(m_bMouseDown==false) return true;
		
		if(m_pSelectedObject == NULL) return true;
		ALISTPOSITION pos = pManager->m_listSelectedObject.GetTailPosition();
		vTrans = GetDeltaVector(m_pSelectedObject->GetPos(),x,y);
		m_pSelectedObject->Translate(vTrans);
		m_pSelectedObject->UpdateProperty(false);
		while( pos )
		{
			CSceneObject* ptemp = pManager->m_listSelectedObject.GetPrev(pos);
			if(m_pSelectedObject==ptemp) continue;
			ptemp->Translate(vTrans);
			ptemp->UpdateProperty(false);
		}
		if(pManager->m_listSelectedObject.GetCount()==1)
		{
			CSceneObject* pobj = pManager->m_listSelectedObject.GetHead();
			A3DVECTOR3 vpos = pobj->GetPos();
			AUX_GetMainFrame()->SetXValue(vpos.x);
			AUX_GetMainFrame()->SetYValue(vpos.y);
			AUX_GetMainFrame()->SetZValue(vpos.z);
		}
		AUX_GetMainFrame()->GetToolTabWnd()->UpdatePropertyData(false);
		m_vDelta += vTrans;
		m_nOldX = x;
		m_nOldY = y;
	}
	return true;
}

A3DVECTOR3 CSceneObjectMoveOperation::GetDeltaVector(A3DVECTOR3 pos,int x,int y)
{
	A3DVECTOR3 vDelta = A3DVECTOR3(0.0f);
	CPoint pt(x,y);
	
	float dot[3];
	A3DVECTOR3 xy(pos),xz(pos),yz(pos);
	ScreenTraceAxis(pos,pt,&xy,&xz,&yz,dot);
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
void CSceneObjectMoveOperation::Render(A3DViewport* pA3DViewport)
{
	SceneObjectMultiSelectOperation::Render(pA3DViewport);
	//绘制每个对象的坐标指示
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		A3DMATRIX4 mat;
		mat.Identity();
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(pManager==NULL) return;
		ALISTPOSITION pos = pManager->m_listSelectedObject.GetTailPosition();
		while( pos )
		{
			CSceneObject* ptemp = pManager->m_listSelectedObject.GetPrev(pos);
			A3DVECTOR3 vCoorPos = ptemp->GetPos();
			pManager->m_coordinateDirection.SetSelectedFlags(m_nDirectFlag);
			pManager->m_coordinateDirection.SetPos(vCoorPos);
			pManager->m_coordinateDirection.Update();
			pManager->m_coordinateDirection.SetRotate(mat);
			pManager->m_coordinateDirection.Draw();
		}
	}
}

bool CSceneObjectMoveOperation::OnLButtonDown(int x, int y, DWORD dwFlags)
{
	m_vDelta = A3DVECTOR3(0.0f,0.0f,0.0f);
	
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		//if(!m_bDrawRect)
		//{	
			pMap->SetModifiedFlag(true);
			m_pUndo = new CUndoObjectMoveAction(pMap->GetSceneObjectMan());
			m_pUndo->GetSeletedListData();
		//}
	}
	return SceneObjectMultiSelectOperation::OnLButtonDown(x,y,dwFlags);
}

bool CSceneObjectMoveOperation::OnLButtonUp(int x, int y, DWORD dwFlags)
{
	SceneObjectMultiSelectOperation::OnLButtonUp(x,y,dwFlags);
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(m_vDelta != A3DVECTOR3(0.0f,0.0f,0.0f))
		{
			if(m_pUndo)
			{
				g_UndoMan.AddUndoAction(m_pUndo);
				m_pUndo = NULL;
			}
			//对水对象的特殊处理，鼠标弹起才更新数据
			ALISTPOSITION pos = pManager->m_listSelectedObject.GetTailPosition();
			while( pos )
			{
				CSceneObject* ptemp = pManager->m_listSelectedObject.GetPrev(pos);
				if(ptemp->GetObjectType()== CSceneObject::SO_TYPE_WATER)
					((CSceneWaterObject*)ptemp)->BuildRenderWater();
			}
		}else
		{
			if(m_pUndo)
			{//
				m_pUndo->Release();
				delete m_pUndo;
				m_pUndo = NULL;
			}
		}
	}

	m_vDelta = A3DVECTOR3(0.0f,0.0f,0.0f);
	return true;
}

void CSceneObjectMoveOperation::OnKeyDown(DWORD dwChar, DWORD dwRepCnt, DWORD dwFlags)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if( !pMap )
		return;
	CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
	if( !pManager || pManager->GetSceneObjectSelectedList()->GetCount()==0 )
		return;

	A3DVECTOR3 vTrans = A3DVECTOR3(0.0f,0.0f,0.0f);
	switch(dwChar)
	{
	case 'I':
		vTrans = A3DVECTOR3(0.0f,0.0f,0.02f) * (int)dwRepCnt;
		break;
	case 'K':
		vTrans = A3DVECTOR3(0.0f,0.0f,-0.02f) * (int)dwRepCnt;
		break;
	case 'J':
		vTrans = A3DVECTOR3(-0.02f,0.0f,0.0f) * (int)dwRepCnt;
		break;
	case 'L':
		vTrans = A3DVECTOR3(0.02f,0.0f,0.0f) * (int)dwRepCnt;
		break;
	case 'U':
		vTrans = A3DVECTOR3(0.0f,0.02f,0.0f) * (int)dwRepCnt;
		break;
	case 'O':
		vTrans = A3DVECTOR3(0.0f,-0.02f,0.0f) * (int)dwRepCnt;
		break;
	default:
		return;
	}

	APtrList<CSceneObject *>* pSelList = pManager->GetSceneObjectSelectedList();
	ALISTPOSITION pos = pSelList->GetHeadPosition();
	while(pos)
	{
		CSceneObject* pScnObject = pSelList->GetNext(pos);
		pScnObject->Translate(vTrans);
		pScnObject->UpdateProperty(false);
	}
}

void CSceneObjectMoveOperation::OnKeyUp(DWORD dwChar, DWORD dwRepCnt, DWORD dwFlags)
{

}
