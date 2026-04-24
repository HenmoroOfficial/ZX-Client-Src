// SceneAIGenerator.cpp: implementation of the CSceneAIGenerator class.
//
//////////////////////////////////////////////////////////////////////

#include "global.h"
#include "elementeditor.h"

#include "SceneObjectManager.h"
#include "Box3D.h"
#include "Render.h"
#include "A3D.h"
#include "MainFrm.h"
#include "NpcSelDlg.h"
#include "FixedNpcSelDlg.h"
#include "EC_Model.h"

#include "SceneAIGenerator.h"
#include "elementdataman.h"
#include "RenderWnd.h"
#include "BezierSelDlg.h"
#include "NpcControllerSelDlg.h"
#include "NpcControllerManDlg.h"
#include "ExtraAreaManDlg.h"
#include "..\ZElementData\DataPathMan.h"




//#define new A_DEBUG_NEW


void init_ai_data( AI_DATA &data)
{
	data.dwAreaType = 0;
	data.fEdgeHei = 20.0f;
	data.fEdgeLen = 20.0f;
	data.fEdgeWth = 20.0f;
	data.nMonsterType   = 0;
	data.bPoint   = false;
	data.nControllerId = 0;
	data.nMounsterNum = 0;
	data.nMounsterLife = 0;
	
	data.bGenMounsterInit = true;//怪物是否起始时生成
	data.iReviveType = 1;//区域怪物是否自动重生
	data.bValidAtOneTime = false;//区域是否同时生效一次
	data.dwGenID = 0;//区域制定编号
}

void Init_npc_property( NPC_PROPERTY * data)
{
	data->strNpcName = "";		//怪物名字
	data->dwID = 0;				//怪物ID
	data->dwNum = 0;			//怪物数
	data->dwRefurbish = 30;		//刷新率
	data->dwDiedTimes = 50;		//产生新属性需死的次数
	data->dwInitiative = 0;		//主动被动攻击选择
	data->fOffsetWater = 0;		//相对于水的高度
	data->fOffsetTerrain = 0;	//相对与地的高度
	
	//new 
	data->dwMounsterFaction = 0;//怪物自己的类别
	data->dwMounsterFactionHelper = 0;//怪物寻求帮助的类别
	data->dwMounsterFactionAccept = 0;//怪物响应帮助的类别
	data->dwDisapearTime = 0;//怪物死亡后的消失时间（0-100S）
	data->bNeedHelp = false;//是否要求救
	
	data->bUseDefautFaction = true;//用怪物默认类别
	data->bUseDefautFactionHelper = true;//用怪物默认的帮助者
	data->bUseDefautFactionAccept = true;//怪物响应默认的求助者

	data->nPathID = -1;
	data->nLoopType = 1;       //循环方式
	data->strPathName = "";   //帮定路径名
	data->nSpeedFlags = 0;     //0,慢速，1,快速
}

void init_fixed_npc_data(FIXED_NPC_DATA &data)
{
	data.strNpcName = "";
    data.dwID = 0;				
	data.dwRefurbish = 0;		
	data.fRadius = 5.0f;
	data.dwNpcType = 1;
	data.cNpcInfo = 0;
	data.nPathID = -1;
	data.nLoopType = 1;
	data.nSpeedFlags = 0;
	data.dwDisapearTime = 0;

	data.nControllerId = 0;
	data.nMounsterNum = 0;
	data.nMounsterLife = 0;

	data.bGenMounsterInit = true;//怪物是否起始时生成
	data.iReviveType = 1;//区域怪物是否自动重生
	data.bValidAtOneTime = true;//区域是否同时生效一次
	data.dwGenID = 0;//区域制定编号
	data.bPatrol      = false;
	data.sPhase = 0;
}

BOOL CALLBACK AI_ID_FUNCS::OnActivate(void)
{
	CSceneAIGenerator* pObj = NULL;
	CString strID;
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(pManager->m_listSelectedObject.GetCount()==1)
		{
			pObj = (CSceneAIGenerator*)pManager->m_listSelectedObject.GetHead();
			if(pObj->GetObjectType()==CSceneObject::SO_TYPE_AIAREA)
			{
				CNpcSelDlg dlg;
				ASSERT(pObj);
				dlg.Init(pObj);
				dlg.DoModal();
				m_strText = "";
				int n = pObj->GetNPCNum();
				for( int i = 0; i < n ; i++)
				{
					NPC_PROPERTY *pro = pObj->GetNPCProperty(i);
					strID.Format("(ID:%d)",pro->dwID);
					m_strText += pro->strNpcName;
					m_strText += strID;
					if( i+1 < n) m_strText += ";";	
				}
			}
		}
	}
	
	return TRUE;
}

LPCTSTR CALLBACK AI_ID_FUNCS::OnGetShowString(void) const
{
	return m_strText;
}

AVariant CALLBACK AI_ID_FUNCS::OnGetValue(void) const
{
	return m_var;
}

void CALLBACK AI_ID_FUNCS::OnSetValue(const AVariant& var)
{
	m_strText = "";
	CString strID;
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(pManager->m_listSelectedObject.GetCount()==1)
		{
			CSceneAIGenerator* pObj = (CSceneAIGenerator*)pManager->m_listSelectedObject.GetHead();
			if(pObj->GetObjectType()==CSceneObject::SO_TYPE_AIAREA)
			{
				int n = pObj->GetNPCNum();
				for( int i = 0; i < n ; i++)
				{
					NPC_PROPERTY *pro = pObj->GetNPCProperty(i);
					strID.Format("(ID:%d)",pro->dwID);
					m_strText += pro->strNpcName;
					m_strText += strID;
					if( i+1 < n) m_strText += ";";	
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
BOOL CALLBACK AI_EXTRA_ID_FUNCS::OnActivate(void)
{
	CSceneAIGenerator* pObj = NULL;
	CString strID;
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(pManager->m_listSelectedObject.GetCount()==1)
		{
			pObj = (CSceneAIGenerator*)pManager->m_listSelectedObject.GetHead();
			if(pObj->GetObjectType()==CSceneObject::SO_TYPE_AIAREA && !pObj->IsExtraArea())
			{
				CExtraAreaManDlg dlg;
				ASSERT(pObj);
				dlg.Init(pObj,CSceneObject::SO_TYPE_AIAREA);
				dlg.DoModal();
				if(pObj->GetExtraAreaNum() > 0 ) m_strText = "...";
				else m_strText = "";
			}
		}
	}
	
	return TRUE;
}

LPCTSTR CALLBACK AI_EXTRA_ID_FUNCS::OnGetShowString(void) const
{
	return m_strText;
}

AVariant CALLBACK AI_EXTRA_ID_FUNCS::OnGetValue(void) const
{
	return m_var;
}

void CALLBACK AI_EXTRA_ID_FUNCS::OnSetValue(const AVariant& var)
{
	m_strText = "";
	CString strID;
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(pManager->m_listSelectedObject.GetCount()==1)
		{
			CSceneAIGenerator* pObj = (CSceneAIGenerator*)pManager->m_listSelectedObject.GetHead();
			if(pObj->GetObjectType()==CSceneObject::SO_TYPE_AIAREA)
			{
				if(pObj->GetExtraAreaNum() > 0 && !pObj->IsExtraArea()) 
					m_strText = "...";
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
BOOL CALLBACK NPC_EXTRA_ID_FUNCS::OnActivate(void)
{
	CSceneFixedNpcGenerator* pObj = NULL;
	CString strID;
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(pManager->m_listSelectedObject.GetCount()==1)
		{
			pObj = (CSceneFixedNpcGenerator*)pManager->m_listSelectedObject.GetHead();
			if(pObj->GetObjectType()==CSceneObject::SO_TYPE_FIXED_NPC && !pObj->IsExtraArea())
			{
				CExtraAreaManDlg dlg;
				ASSERT(pObj);
				dlg.Init(pObj,CSceneObject::SO_TYPE_FIXED_NPC);
				dlg.DoModal();
				if(pObj->GetExtraAreaNum()>0) m_strText = "...";
				else m_strText = "";
			}
		}
	}
	
	return TRUE;
}

LPCTSTR CALLBACK NPC_EXTRA_ID_FUNCS::OnGetShowString(void) const
{
	return m_strText;
}

AVariant CALLBACK NPC_EXTRA_ID_FUNCS::OnGetValue(void) const
{
	return m_var;
}

void CALLBACK NPC_EXTRA_ID_FUNCS::OnSetValue(const AVariant& var)
{
	m_strText = "";
	CString strID;
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(pManager->m_listSelectedObject.GetCount()==1)
		{
			CSceneFixedNpcGenerator* pObj = (CSceneFixedNpcGenerator*)pManager->m_listSelectedObject.GetHead();
			if(pObj->GetObjectType()==CSceneObject::SO_TYPE_FIXED_NPC)
			{
				if(pObj->GetExtraAreaNum() > 0 && !pObj->IsExtraArea()) 
					m_strText = "...";
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////
//class FIXED_NPC_ID_FUNCS
BOOL CALLBACK FIXED_NPC_ID_FUNCS::OnActivate(void)
{
	
	CSceneObject* pObj = NULL;
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	CString strID;
	if(pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(pManager->m_listSelectedObject.GetCount()==1)
		{
			pObj = pManager->m_listSelectedObject.GetHead();
			if(pObj->GetObjectType()==CSceneObject::SO_TYPE_FIXED_NPC)
			{
				CFixedNpcSelDlg dlg;
				CSceneFixedNpcGenerator *pObj1 = (CSceneFixedNpcGenerator*)pObj;
				FIXED_NPC_DATA prop = pObj1->GetProperty();
				
				dlg.m_dwSelId = prop.dwID;
				dlg.m_cType = prop.cNpcInfo;
				dlg.m_strName = CString(prop.strNpcName);
				if(dlg.DoModal()==IDOK)
				{
					prop.dwID = dlg.m_dwSelId;
					prop.cNpcInfo = dlg.m_cType;
					prop.strNpcName = AString(dlg.m_strName);
					pObj1->SetProperty(prop);
					strID.Format("(ID:%d)",prop.dwID);
					m_strText = prop.strNpcName;
					m_strText += strID;
					pObj1->UpdateProperty(false);
				}
			}
		}
	}
	
	return TRUE;
}

LPCTSTR CALLBACK FIXED_NPC_ID_FUNCS::OnGetShowString(void) const
{
	return m_strText;
}

AVariant CALLBACK FIXED_NPC_ID_FUNCS::OnGetValue(void) const
{
	return m_var;
}

void CALLBACK FIXED_NPC_ID_FUNCS::OnSetValue(const AVariant& var)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	CString strID;
	if(pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(pManager->m_listSelectedObject.GetCount()==1)
		{
			CSceneFixedNpcGenerator* pObj = (CSceneFixedNpcGenerator*)pManager->m_listSelectedObject.GetHead();
			if(pObj->GetObjectType()==CSceneObject::SO_TYPE_FIXED_NPC)
			{
				FIXED_NPC_DATA pro = pObj->GetProperty();
				strID.Format("(ID:%d)",pro.dwID);
				m_strText = pro.strNpcName;
				m_strText += strID;
			}
		}
	}
}

//Npc beizer select function
BOOL CALLBACK NPC_BEZIER_SEL_ID_FUNCS::OnActivate(void)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(pManager->m_listSelectedObject.GetCount()==1)
		{
			CSceneObject *pObj = (CSceneObject*)pManager->m_listSelectedObject.GetHead();
			if(pObj->GetObjectType()==CSceneObject::SO_TYPE_FIXED_NPC)
			{
				CBezierSelDlg dlg;
				FIXED_NPC_DATA data = ((CSceneFixedNpcGenerator*)pObj)->GetProperty();
				dlg.m_nBezierID = data.nPathID;
				ASSERT(pObj);
				if(dlg.DoModal()==IDOK)
				{
					m_strText = dlg.m_strBezierName;
					data.nPathID = dlg.m_nBezierID;
					data.strPathName = dlg.m_strBezierName;
					((CSceneFixedNpcGenerator*)pObj)->SetProperty(data);
				}
			}
		}
	}
	
	return TRUE;
}

LPCTSTR CALLBACK NPC_BEZIER_SEL_ID_FUNCS::OnGetShowString(void) const
{
	return m_strText;
}

AVariant CALLBACK NPC_BEZIER_SEL_ID_FUNCS::OnGetValue(void) const
{
	return m_var;
}

void CALLBACK NPC_BEZIER_SEL_ID_FUNCS::OnSetValue(const AVariant& var)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(pManager->m_listSelectedObject.GetCount()==1)
		{
			CSceneObject* pObj = (CSceneObject*)pManager->m_listSelectedObject.GetHead();
			if(pObj->GetObjectType()==CSceneObject::SO_TYPE_FIXED_NPC)
			{
				FIXED_NPC_DATA data = ((CSceneFixedNpcGenerator*)pObj)->GetProperty();
				m_strText = data.strPathName;
			}
		}
	}
}


//Controller select function
BOOL CALLBACK CONTROLLER_ID_FUNCS::OnActivate(void)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(pManager->m_listSelectedObject.GetCount()==1)
		{
			CSceneObject *pObj = (CSceneObject*)pManager->m_listSelectedObject.GetHead();
			if(pObj->GetObjectType()==CSceneObject::SO_TYPE_AIAREA)
			{
				CNpcContollerSelDlg dlg;
				AI_DATA data = ((CSceneAIGenerator*)pObj)->GetProperty();
				dlg.m_nDyObjID = data.nControllerId;
				if(dlg.DoModal()==IDOK)
				{
					data.nControllerId = dlg.m_nDyObjID;
					((CSceneAIGenerator*)pObj)->SetProperty(data);
					CONTROLLER_OBJECT obj;
					InitControllerObj(&obj);
					g_ControllerObjectMan.GetObjByID(data.nControllerId,obj);
					if(obj.id==0) m_strText = "";
					else m_strText.Format("[%d]%s",obj.id,obj.szName);
				}
			}
		}
	}
	
	return TRUE;
}

LPCTSTR CALLBACK CONTROLLER_ID_FUNCS::OnGetShowString(void) const
{
	return m_strText;
}

AVariant CALLBACK CONTROLLER_ID_FUNCS::OnGetValue(void) const
{
	return m_var;
}

void CALLBACK CONTROLLER_ID_FUNCS::OnSetValue(const AVariant& var)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(pManager->m_listSelectedObject.GetCount()==1)
		{
			CSceneObject *pObj = (CSceneObject*)pManager->m_listSelectedObject.GetHead();
			if(pObj->GetObjectType()==CSceneObject::SO_TYPE_AIAREA)
			{
				CNpcContollerSelDlg dlg;
				AI_DATA data = ((CSceneAIGenerator*)pObj)->GetProperty();
				CONTROLLER_OBJECT obj;
				InitControllerObj(&obj);
				g_ControllerObjectMan.GetObjByID(data.nControllerId,obj);
				if(obj.id!=0) m_strText.Format("[%d]%s",obj.id,obj.szName);
			}
		}
	}
}


//Controller NPC select function
BOOL CALLBACK CONTROLLER_NPC_ID_FUNCS::OnActivate(void)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(pManager->m_listSelectedObject.GetCount()==1)
		{
			CSceneObject *pObj = (CSceneObject*)pManager->m_listSelectedObject.GetHead();
			if(pObj->GetObjectType()==CSceneObject::SO_TYPE_FIXED_NPC)
			{
				CNpcContollerSelDlg dlg;
				FIXED_NPC_DATA data = ((CSceneFixedNpcGenerator*)pObj)->GetProperty();
				dlg.m_nDyObjID = data.nControllerId;
				if(dlg.DoModal()==IDOK)
				{
					data.nControllerId = dlg.m_nDyObjID;
					((CSceneFixedNpcGenerator*)pObj)->SetProperty(data);
					CONTROLLER_OBJECT obj;
					InitControllerObj(&obj);
					g_ControllerObjectMan.GetObjByID(data.nControllerId,obj);
					if(obj.id==0) m_strText = "";
					else m_strText.Format("[%d]%s",obj.id,obj.szName);
				}
			}
		}
	}
	
	return TRUE;
}

LPCTSTR CALLBACK CONTROLLER_NPC_ID_FUNCS::OnGetShowString(void) const
{
	return m_strText;
}

AVariant CALLBACK CONTROLLER_NPC_ID_FUNCS::OnGetValue(void) const
{
	return m_var;
}

void CALLBACK CONTROLLER_NPC_ID_FUNCS::OnSetValue(const AVariant& var)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(pManager->m_listSelectedObject.GetCount()==1)
		{
			CSceneObject *pObj = (CSceneObject*)pManager->m_listSelectedObject.GetHead();
			if(pObj->GetObjectType()==CSceneObject::SO_TYPE_FIXED_NPC)
			{
				CNpcContollerSelDlg dlg;
				FIXED_NPC_DATA data = ((CSceneFixedNpcGenerator*)pObj)->GetProperty();
				CONTROLLER_OBJECT obj;
				InitControllerObj(&obj);
				g_ControllerObjectMan.GetObjByID(data.nControllerId,obj);
				if(obj.id!=0) m_strText.Format("[%d]%s",obj.id,obj.szName);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSceneAIGenerator::CSceneAIGenerator()
{
	m_nObjectType = SO_TYPE_AIAREA;
	m_pIDSelFuncs = new AI_ID_FUNCS;
	m_pExtraIDSelFuncs = new AI_EXTRA_ID_FUNCS;
	m_pControllerSelFuncs = new CONTROLLER_ID_FUNCS;
	area_type.AddElement("随地形起伏",TYPE_TERRAIN_FOLLOW);
	area_type.AddElement("平面",TYPE_PLANE);
	area_type.AddElement("立方体",TYPE_BOX);

	group_type.AddElement("普通类型",0);
	group_type.AddElement("群体类型1",1);
	group_type.AddElement("群体类型2",2);

	revive_type.AddElement("不重生",0);
	revive_type.AddElement("消失后重生",1);
	revive_type.AddElement("开启后重生",2);
	m_nCopy = 0;
	m_bExtraArea = false;
	BuildProperty();
}

CSceneAIGenerator::~CSceneAIGenerator()
{

}

void CSceneAIGenerator::DelExtra( int id)
{
	int n = m_listExtraArea.size();
	for( int i = 0; i < n; ++i)
	{
		if(m_listExtraArea[i] == id)
		{
			m_listExtraArea.erase(&m_listExtraArea[i]);
			break;
		}
	}
}

void CSceneAIGenerator::Translate(const A3DVECTOR3& vDelta)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap==NULL) return;
	if(!pMap->GetTerrain()) return;

	m_vPos += vDelta;
	if(m_Data.dwAreaType==TYPE_TERRAIN_FOLLOW)
		m_vPos.y = pMap->GetTerrain()->GetPosHeight(m_vPos) + 0.1f;

	m_matTrans.SetRow(3,m_vPos);
	m_matAbs = m_matScale * m_matRotate * m_matTrans;

	POSITION pos = m_listModels.GetHeadPosition();
	while(pos)
	{
		CECModel *pModels = (CECModel *)m_listModels.GetNext(pos);
		A3DVECTOR3 old = pModels->GetA3DSkinModel()->GetPos();
		old = old + vDelta;
		if(m_Data.dwAreaType==TYPE_TERRAIN_FOLLOW)
			old.y = pMap->GetTerrain()->GetPosHeight(old);
		pModels->GetA3DSkinModel()->SetPos(old);
	}	
}

void CSceneAIGenerator::DrawRect()
{
	
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap==NULL) return;
	if(!pMap->GetTerrain()) return;	
	ARectF trc = pMap->GetTerrain()->GetTerrainArea();
	float fCenterX = m_vPos.x;
	float fCenterY = m_vPos.z;

	int lseg = (int)(m_Data.fEdgeLen/STEP_LEN);
	int wseg = (int)(m_Data.fEdgeWth/STEP_LEN);

	lseg += 2;
	wseg += 2;

	float flRadius = m_Data.fEdgeLen/2.0f;
	float fwRadius = m_Data.fEdgeWth/2.0f;
	float offset_y = 0.1f;

	A3DVECTOR3 vLeftBottom = A3DVECTOR3(fCenterX - flRadius ,0, fCenterY - fwRadius);
	A3DVECTOR3 vRightBottom = A3DVECTOR3(fCenterX + flRadius ,0, fCenterY -  fwRadius);
	A3DVECTOR3 vRightUp = A3DVECTOR3(fCenterX + flRadius ,0,fCenterY + fwRadius);
	A3DVECTOR3 vLeftUp = A3DVECTOR3(fCenterX - flRadius,0,fCenterY + fwRadius);
	
	A3DVECTOR3* lvertices = new A3DVECTOR3[lseg];
	A3DVECTOR3* wvertices = new A3DVECTOR3[wseg];
	DWORD clr; 
	if(m_bSelected)	clr = A3DCOLORRGB(255,255,255);
	else 
	{
		if(m_bExtraArea) clr = A3DCOLORRGB(0,0,255); 
		else 
		{
			if(m_listExtraArea.size() > 0) clr = A3DCOLORRGB(0,0,125);
			else clr = A3DCOLORRGB(255,0,255);
		}
	}
	int n;
	float temp;
	for(n = 0 ; n<lseg; n++)
	{
		temp = (float)n/(float)(lseg-1);
		lvertices[n] = vLeftBottom + (vRightBottom - vLeftBottom)*temp;
		lvertices[n].y = pMap->GetTerrain()->GetPosHeight(lvertices[n]) + offset_y;//0.1f
		//Get height from pass map
		if(pMap->GetPassMapGen()->GetPassMap())
		{
			if(wvertices[n].x > trc.right || wvertices[n].x < trc.left ||
				wvertices[n].z > trc.top || wvertices[n].z < trc.bottom) continue;
			float fDelta = pMap->GetPassMapGen()->GetPosDeltaHeight(lvertices[n]);
			lvertices[n].y += fDelta;
		}
	
	}
	DrawLine(lvertices,lseg,clr);

	for(n = 0 ; n<lseg; n++)
	{
		temp = (float)n/(float)(lseg-1);
		lvertices[n] = vLeftUp + (vRightUp - vLeftUp)*temp;
		lvertices[n].y = pMap->GetTerrain()->GetPosHeight(lvertices[n]) + offset_y;//0.1f
		//Get height from pass map
		if(pMap->GetPassMapGen()->GetPassMap())
		{
			if(wvertices[n].x > trc.right || wvertices[n].x < trc.left ||
				wvertices[n].z > trc.top || wvertices[n].z < trc.bottom) continue;
			float fDelta = pMap->GetPassMapGen()->GetPosDeltaHeight(lvertices[n]);
			lvertices[n].y += fDelta;
		}
	}
	DrawLine(lvertices,lseg,clr);

	for(n = 0 ; n < wseg; n++)
	{
		temp = (float)n/(float)(wseg-1);
		wvertices[n] = vRightUp + (vRightBottom - vRightUp)*temp;
		wvertices[n].y = pMap->GetTerrain()->GetPosHeight(wvertices[n]) + offset_y;//0.1f
		//Get height from pass map
		if(pMap->GetPassMapGen()->GetPassMap())
		{
			float fDelta = 0.0f; 
			if(wvertices[n].x > trc.right || wvertices[n].x < trc.left ||
				wvertices[n].z > trc.top || wvertices[n].z < trc.bottom) continue;
			
			pMap->GetPassMapGen()->GetPosDeltaHeight(wvertices[n]);
			wvertices[n].y += fDelta;
		}
	}
	DrawLine(wvertices,wseg,clr);

	for(n = 0 ; n < wseg; n++)
	{
		temp = (float)n/(float)(wseg-1);
		wvertices[n] = vLeftUp + (vLeftBottom - vLeftUp)*temp;
		wvertices[n].y = pMap->GetTerrain()->GetPosHeight(wvertices[n]) + offset_y;//0.1f
		//Get height from pass map
		if(pMap->GetPassMapGen()->GetPassMap())
		{
			float fDelta = pMap->GetPassMapGen()->GetPosDeltaHeight(wvertices[n]);
			wvertices[n].y += fDelta;
		}
	}
	DrawLine(wvertices,wseg,clr);

	delete []lvertices;
	delete []wvertices;
}

void CSceneAIGenerator::DrawLine(A3DVECTOR3 *pVertices,DWORD dwNum,DWORD clr)
{
	A3DWireCollector *pWireCollector = g_Render.GetA3DEngine()->GetA3DWireCollector();
	
	WORD* pIndices = new WORD[(dwNum - 1)*2];
	int n = 0;
	for(int i=0; i<(int)dwNum - 1; i++)
	{
		pIndices[n] = i;
		pIndices[n+1] = i+1;
		n += 2;
	}
	
	if(pWireCollector)
	{
		pWireCollector->AddRenderData_3D(pVertices,dwNum,pIndices,(dwNum-1)*2,clr);
	}
	delete []pIndices;
}

void CSceneAIGenerator::DrawMounsterName()
{
	A3DString str;
	HA3DFONT hA3dFont;
	hA3dFont = g_Render.GetA3DEngine()->GetA3DFontMan()->RegisterFontType("宋体",10,0,'w');
	float fCenterX = m_vPos.x;
	float fCenterY = m_vPos.z;
	float flRadius = m_Data.fEdgeLen/2.0f;
	float fwRadius = m_Data.fEdgeWth/2.0f;
	A3DVECTOR3 vLeftUp = A3DVECTOR3(fCenterX - flRadius,0,fCenterY + fwRadius);
	
	
	A3DVECTOR3 vScreenPos;
	if(g_Render.GetA3DEngine()->GetActiveCamera()->GetWorldFrustum()->PointInFrustum(vLeftUp))
	{
		g_Render.GetA3DEngine()->GetActiveViewport()->Transform(vLeftUp, vScreenPos);
		int n = GetNPCNum();
		for(int i = 0; i < n; i++ )
		{
			NPC_PROPERTY* pro = GetNPCProperty(i);
			str.Init(g_Render.GetA3DDevice(),pro->strNpcName,hA3dFont);	
			str.DrawText(vScreenPos.x, vScreenPos.y,A3DCOLORRGB(0,255,0));
			vScreenPos.y +=10;
			str.Release();
		}
	}
}

void CSceneAIGenerator::Render(A3DViewport* pA3DViewport)
{
	if(m_nCopy != g_Configs.nShowCopyNum) return;
	RenderModels(pA3DViewport);
	CSceneObject::Render(pA3DViewport);
}

void CSceneAIGenerator::PostRender(A3DViewport* pA3DViewport)
{
	if(m_nCopy != g_Configs.nShowCopyNum) return;
	DrawCenter();
	if(m_Data.dwAreaType==TYPE_BOX)
	{
		CBox3D renderBox;
		CBox3D renderWire;
		renderBox.Init(&g_Render,1.0f,true);
		renderWire.Init(&g_Render,1.0f,false);
		renderBox.SetSize(m_Data.fEdgeLen/2.0f,m_Data.fEdgeHei/2.0f,m_Data.fEdgeWth/2.0f);
		renderWire.SetSize(m_Data.fEdgeLen/2.0f,m_Data.fEdgeHei/2.0f,m_Data.fEdgeWth/2.0f);
		renderBox.SetTM(m_matTrans);
		renderWire.SetTM(m_matTrans);
		renderWire.SetColor(A3DCOLORRGBA(0,0,255,255));
		if(m_bSelected) renderBox.SetColor(A3DCOLORRGBA(255,0,0,128));
		else renderBox.SetColor(A3DCOLORRGBA(255,255,0,128));

		renderBox.Render();
		renderWire.Render();
	}else if(m_Data.dwAreaType==TYPE_PLANE)
	{
		CBox3D renderBox;
		renderBox.Init(&g_Render,1.0f,true);
		renderBox.SetSize(m_Data.fEdgeLen/2.0f,0.01f,m_Data.fEdgeWth/2.0f);
		renderBox.SetTM(m_matTrans);
		if(m_bSelected) renderBox.SetColor(A3DCOLORRGBA(255,0,0,128));
		else renderBox.SetColor(A3DCOLORRGBA(255,255,0,128));
		renderBox.Render();

	}else if(m_Data.dwAreaType==TYPE_TERRAIN_FOLLOW)
	{
		DrawRect();
	}

	CViewFrame *pFrame = AUX_GetMainFrame()->GetViewFrame();
	CRenderWnd *pRWnd = pFrame->GetRenderWnd();
	if(pRWnd->GetActiveViewport() == VIEW_XZ)
	{
		DrawMounsterName();
	}
	CSceneObject::PostRender(pA3DViewport);
}

void CSceneAIGenerator::Tick(DWORD timeDelta)
{
	if(m_nCopy != g_Configs.nShowCopyNum) return;
	POSITION pos = m_listModels.GetHeadPosition();
	while(pos)
	{
		CECModel *pModels = (CECModel *)m_listModels.GetNext(pos);
		pModels->Tick(timeDelta);
	}	
}

void CSceneAIGenerator::UpdateProperty(bool bGet)
{
	if(bGet)
	{
		AString temp = AString(m_pProperty->GetPropVal(0));
		CElementMap *pMap = AUX_GetMainFrame()->GetMap();
		if(pMap)
		{
			CSceneObjectManager *pSManager = pMap->GetSceneObjectMan();
			CSceneObject*pObj = pSManager->FindSceneObject(temp);
			if(pObj != NULL && pObj != this)
			{
				AfxMessageBox("该对象的名字已经存在，编辑器将恢复原对象名字！",NULL,MB_OK);
				m_pProperty->SetPropVal(0,m_strName);
				m_pProperty->SetPropVal(1,m_vPos);
				m_pProperty->SetPropVal(2,m_nCopy);
				m_pProperty->SetPropVal(3,m_Data.dwAreaType);
				m_pProperty->SetPropVal(4,m_Data.fEdgeLen);
				m_pProperty->SetPropVal(5,m_Data.fEdgeWth);
				m_pProperty->SetPropVal(6,m_Data.fEdgeHei);
				m_pProperty->SetPropVal(7,m_Data.fEdgeLen*m_Data.fEdgeWth);
				m_pProperty->SetPropVal(8,m_Data.nMonsterType);
				m_pProperty->SetPropVal(9,m_Data.bPoint);
				m_pProperty->SetPropVal(10,m_Data.iReviveType);
				m_pProperty->SetPropVal(11,m_Data.nMounsterNum);
				m_pProperty->SetPropVal(12,m_Data.nMounsterLife);
				m_pProperty->SetPropVal(15,m_bExtraArea);
				m_pControllerSelFuncs->OnSetValue(m_Data.nControllerId);
				m_pIDSelFuncs->OnSetValue(m_strNPCList);
				m_pExtraIDSelFuncs->OnSetValue("");
				m_pProperty->SetPropVal(17,m_nObjectID);
				AUX_GetMainFrame()->GetToolTabWnd()->UpdatePropertyData(false);
				return;
			}
		}
		m_strName	        = temp;
		m_vPos	            = m_pProperty->GetPropVal(1);
		m_nCopy             = m_pProperty->GetPropVal(2);
		m_Data.dwAreaType   = m_pProperty->GetPropVal(3);
		m_Data.fEdgeLen     = m_pProperty->GetPropVal(4);
		m_Data.fEdgeWth     = m_pProperty->GetPropVal(5);
		m_Data.fEdgeHei     = m_pProperty->GetPropVal(6);
		m_pProperty->SetPropVal(7,m_Data.fEdgeLen*m_Data.fEdgeWth);
		m_Data.nMonsterType = m_pProperty->GetPropVal(8);
		m_Data.bPoint       = m_pProperty->GetPropVal(9);
		
		m_Data.iReviveType = m_pProperty->GetPropVal(10);
		m_Data.nMounsterNum = m_pProperty->GetPropVal(11);
		m_Data.nMounsterLife = m_pProperty->GetPropVal(12);
		m_bExtraArea = m_pProperty->GetPropVal(15); 
		if(m_Data.dwGenID!=0 || !m_Data.bGenMounsterInit)
		{
			if(IDYES==AfxMessageBox("这个区域已经被你更新，由于是旧版数据，现需要完成版本转换，你确定要转换吗？",MB_YESNO|MB_ICONQUESTION))
			{
				m_Data.bGenMounsterInit = true;
				m_Data.dwGenID = 0;
			}
		}
		
		m_matTrans.Translate(m_vPos.x,m_vPos.y,m_vPos.z);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
		
		ReloadModels();
	}else
	{
		m_pProperty->SetPropVal(0,m_strName);
		m_pProperty->SetPropVal(1,m_vPos);
		m_pProperty->SetPropVal(2,m_nCopy);
		m_pProperty->SetPropVal(3,m_Data.dwAreaType);
		m_pProperty->SetPropVal(4,m_Data.fEdgeLen);
		m_pProperty->SetPropVal(5,m_Data.fEdgeWth);
		m_pProperty->SetPropVal(6,m_Data.fEdgeHei);
		m_pProperty->SetPropVal(7,m_Data.fEdgeLen*m_Data.fEdgeWth);
		m_pProperty->SetPropVal(8,m_Data.nMonsterType);
		m_pProperty->SetPropVal(9,m_Data.bPoint);
		m_pProperty->SetPropVal(10,m_Data.iReviveType);
		m_pProperty->SetPropVal(11,m_Data.nMounsterNum);
		m_pProperty->SetPropVal(12,m_Data.nMounsterLife);
		m_pProperty->SetPropVal(15,m_bExtraArea);
		m_pControllerSelFuncs->OnSetValue(m_Data.nControllerId);
		m_pIDSelFuncs->OnSetValue(m_strNPCList);
		m_pExtraIDSelFuncs->OnSetValue("");
		m_pProperty->SetPropVal(17,m_nObjectID);
	}
}

void CSceneAIGenerator::Release()
{
	POSITION pos = m_listNPC.GetHeadPosition();
	while(pos)
	{
		NPC_PROPERTY *pro = (NPC_PROPERTY *)m_listNPC.GetNext(pos);
		delete pro;
	}
	m_listNPC.RemoveAll();

	ReleaseModels();
	if(m_pIDSelFuncs) delete m_pIDSelFuncs;
	if(m_pControllerSelFuncs) delete m_pControllerSelFuncs;
	if(m_pExtraIDSelFuncs) delete m_pExtraIDSelFuncs;
	CSceneObject::Release();
}

void CSceneAIGenerator::EntryScene()
{

}

void CSceneAIGenerator::LeaveScene()
{

}

void CSceneAIGenerator::BuildProperty()
{
	if(m_pProperty)
	{
		m_pProperty->DynAddProperty(AVariant(m_strName), "名字");
		m_pProperty->DynAddProperty(AVariant(m_vPos), "位置坐标");
		m_pProperty->DynAddProperty(AVariant(m_nCopy), "第几套数据");
		m_pProperty->DynAddProperty(AVariant(m_Data.dwAreaType), "区域类型", &area_type);
		m_pProperty->DynAddProperty(AVariant(m_Data.fEdgeLen), "长(x)");
		m_pProperty->DynAddProperty(AVariant(m_Data.fEdgeWth), "宽(z)");
		m_pProperty->DynAddProperty(AVariant(m_Data.fEdgeHei), "高(y,立方体有效)");
		m_pProperty->DynAddProperty(AVariant(m_fArea), "面积(x,z平面)",NULL,NULL,WAY_READONLY);
		m_pProperty->DynAddProperty(AVariant(m_Data.nMonsterType), "群怪类型",&group_type);
		m_pProperty->DynAddProperty(AVariant(m_Data.bPoint), "是产生点");
		
		//m_pProperty->DynAddProperty(AVariant(m_Data.bGenMounsterInit), "是否起始时生成");
		m_pProperty->DynAddProperty(AVariant(m_Data.iReviveType), "重生类型", &revive_type);
		m_pProperty->DynAddProperty(AVariant(m_Data.nMounsterNum), "生成对象的最大数");
		m_pProperty->DynAddProperty(AVariant(m_Data.nMounsterLife), " 生成对象的寿命");
		m_pProperty->DynAddProperty(AVariant(m_Data.nControllerId), "关联怪物控制器", (ASet*)m_pControllerSelFuncs, NULL, WAY_CUSTOM);
		m_pProperty->DynAddProperty(AVariant(m_strNPCList), "怪物列表", (ASet*)m_pIDSelFuncs, NULL, WAY_CUSTOM);
		m_pProperty->DynAddProperty(AVariant(m_bExtraArea), "是否为附属区域");
		m_pProperty->DynAddProperty(AVariant(""), "附属区域列表", (ASet*)m_pExtraIDSelFuncs, NULL, WAY_CUSTOM);
		m_pProperty->DynAddProperty(AVariant(m_nObjectID), "场景对象ID（海外版专用）", NULL,NULL,WAY_READONLY);

	}
}

CSceneObject* CSceneAIGenerator::CopyObject()
{
	
	CSceneAIGenerator *pNewObject = new CSceneAIGenerator();
	if(pNewObject==NULL) return NULL;
	int Num = 0;
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		AString name;
		CSceneObjectManager *pSManager = pMap->GetSceneObjectMan();
		pSManager->ClearSelectedList();
		while(1)
		{
			name.Format("AiGenerator_%d",Num);
			if(pSManager->FindSceneObject(name)!=NULL)
			{
				Num++;
			}else 
			{
				Num++;
				break;
			}
		}
		pNewObject->m_nObjectID = pSManager->GenerateObjectID();
		pSManager->InsertSceneObject(pNewObject);
		pSManager->AddObjectPtrToSelected(pNewObject);
		pNewObject->SetObjectName(name);
		pNewObject->SetPos(m_vPos +	A3DVECTOR3(0.1f,0.0f,0.1f));
		pNewObject->SetProperty(GetProperty());
		pNewObject->SetCopyFlags(GetCopyFlags());
		unsigned int i;
		for(i = 0; i < GetNPCNum(); i++)
		{
			NPC_PROPERTY *pro = new NPC_PROPERTY;
			*pro = *(GetNPCProperty(i));
			pNewObject->AddNPC( pro);
		}
		
		pNewObject->SetExtraArea(IsExtraArea());
		for( i = 0; i < m_listExtraArea.size(); ++i)
			pNewObject->AddExtra(m_listExtraArea[i]);

		pNewObject->ReloadModels();
		pNewObject->UpdateProperty(false);
		AUX_GetMainFrame()->GetToolTabWnd()->ShowPropertyPannel(pNewObject->m_pProperty);//把灯光的属性给对象属性表
		AUX_GetMainFrame()->GetToolTabWnd()->UpdatePropertyData(false);//输出属性数据
	}
	return pNewObject;
}

DWORD CSceneAIGenerator::GetNPCNum()
{
	return m_listNPC.GetCount();
}

NPC_PROPERTY* CSceneAIGenerator::GetNPCProperty(int index)
{
	return (NPC_PROPERTY*)m_listNPC.GetAt(m_listNPC.FindIndex(index));
}

NPC_PROPERTY* CSceneAIGenerator::GetNPCProperty(const AString& name)
{
	POSITION pos = m_listNPC.GetHeadPosition();
	while(pos)
	{
		NPC_PROPERTY *pro = (NPC_PROPERTY *)m_listNPC.GetNext(pos);
		if(strcmp(pro->strNpcName,name)==0)
			return pro;
	}	
	return NULL;
}

void CSceneAIGenerator::AddNPC(NPC_PROPERTY* npc)
{
	m_listNPC.AddTail(npc);
}

void CSceneAIGenerator::DeleteNPC(int index)
{
	m_listNPC.RemoveAt(m_listNPC.FindIndex(index));
}

float RandBetween(float fLow, float fHei)
{
	float r = fHei - fLow;
	r = r*((float)rand()/(float)RAND_MAX);
	r = fLow + r;
	return r;
}

AString  CSceneAIGenerator::GetNPCModelPath(int index)
{
	NPC_PROPERTY *pro = (NPC_PROPERTY *)m_listNPC.GetAt(m_listNPC.FindIndex(index));
	return GetModelPath(pro->dwID);
}

CECModel* CSceneAIGenerator::GetNpcModel(int index)
{
	POSITION pos = m_listModels.FindIndex(index);
	if(pos) return (CECModel*)m_listModels.GetAt(pos);
	else return NULL;
}


void CSceneAIGenerator::ReloadModels()
{
	ReleaseModels();
	A3DVECTOR3 vDelta;
	POSITION pos = m_listNPC.GetHeadPosition();
	while(pos)
	{
		NPC_PROPERTY *pro = (NPC_PROPERTY *)m_listNPC.GetNext(pos);
		AString pathName = GetModelPath(pro->dwID);
		if(pathName.IsEmpty()) 
		{
			g_Log.Log("Warning: CSceneAIGenerator::ReloadModels(),Model's path is empty!");
			continue;
		}
		CECModel* pNew = new CECModel;
		ASSERT(pNew);
		if(!pNew->Load(pathName,false,A3DSkinModel::LSF_NOSKIN,true,false))
		{
			pNew->Release();
			g_Log.Log("CSceneAIGenerator::ReloadModels() failed! Can't load file %s",pathName);
			if(!pNew->Load("Models\\error\\error.ecm"))
			{
				pNew->Release();
				delete pNew;
				continue;
			}
		}
		
		//这儿重新加载皮肤
		AString strSkin = pathName;
		int pos = strSkin.ReverseFind('.');
		strSkin = strSkin.Left(pos);
		strSkin += ".ski";
		
		A3DSkinMan* pSkinMan = g_Render.GetA3DEngine()->GetA3DSkinMan();
		A3DSkin* pSkin = pSkinMan->LoadSkinFile(strSkin);
		if(pSkin)
		{
			pNew->GetA3DSkinModel()->AddSkin(pSkin,true);
		}else 
		{
			g_Log.Log("CSceneAIGenerator::ReloadModels(), Failed load %s", strSkin);
			pNew->Release();
			delete pNew;
			continue;
		}
		
		float fRandX = RandBetween(-1.0f,1.0f);
		float fRandZ = RandBetween(-1.0f,1.0f);
		float fEdgeLen,fEdgeWth;
		if(m_Data.fEdgeLen > 10.0f) fEdgeLen = 10.0f;
		else fEdgeLen = m_Data.fEdgeLen;
		if(m_Data.fEdgeWth > 10.0f) fEdgeWth = 10.0f;
		else fEdgeWth = m_Data.fEdgeWth;
		vDelta = A3DVECTOR3(fRandX*fEdgeLen/2.0f,0,fRandZ*fEdgeWth/2.0f);
		
		CElementMap *pMap = AUX_GetMainFrame()->GetMap();
		if(pMap==NULL) return;
		vDelta = vDelta + m_vPos;
		if(m_Data.dwAreaType==TYPE_TERRAIN_FOLLOW) vDelta.y = pMap->GetTerrain()->GetPosHeight(vDelta) + 0.1f;
		pNew->GetA3DSkinModel()->SetPos(vDelta);
		pNew->GetA3DSkinModel()->SetDirAndUp(m_matRotate.GetRow(2),m_matRotate.GetRow(1));
		pNew->Tick(10);
		m_listModels.AddTail(pNew);
	}	
}

void CSceneAIGenerator::ReleaseModels()
{
	POSITION pos = m_listModels.GetHeadPosition();
	while(pos)
	{
		CECModel *pModels = (CECModel *)m_listModels.GetNext(pos);
		pModels->Release();
		delete pModels;
	}	
	m_listModels.RemoveAll();
}

void CSceneAIGenerator::RenderModels(A3DViewport *pView)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	LIGHTINGPARAMS light = pMap->GetLightingParams();
	A3DSkinModel::LIGHTINFO LightInfo;
	LightInfo.colAmbient	= A3DCOLORVALUE(light.dwAmbient);//	Directional light's specular color
	LightInfo.vLightDir		= light.vSunDir;//Direction of directional light
	LightInfo.colDirDiff	= A3DCOLORVALUE(light.dwSunCol);//Directional light's diffuse color
	LightInfo.colDirSpec	= A3DCOLORVALUE(0.8f,0.8f,0.8f,1.0f);//	Directional light's specular color
	LightInfo.bPtLight		= false;//	false, disable dynamic point light,
	
	POSITION pos = m_listModels.GetHeadPosition();
	while(pos)
	{
		CECModel *pModels = (CECModel *)m_listModels.GetNext(pos);
		CBox3D renderBox;
		pModels->GetA3DSkinModel()->SetLightInfo(LightInfo);
		pModels->Render(pView);
	}	
}

//Load data
bool CSceneAIGenerator::Load(CELArchive& ar,DWORD dwVersion,int iLoadFlags)
{
	init_ai_data(m_Data);
	char cReviveType = m_Data.iReviveType;
	if(dwVersion > 8 && dwVersion < 10)
	{
		m_strName = ar.ReadString();
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.dwAreaType,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeHei,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeLen,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeWth,sizeof(DWORD)))
			return false;
		int num;
		if(!ar.Read(&num,sizeof(int)))
			return false;
		
		for(int i = 0; i<num; i++)
		{
			NPC_PROPERTY* pro = new NPC_PROPERTY;
			Init_npc_property(pro);
			bool bTemp;
			if(!ar.Read(&bTemp,sizeof(bool)))
				return false;
			pro->dwInitiative = (int)bTemp;
			if(!ar.Read(&pro->dwDiedTimes,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwID,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwNum,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwRefurbish,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->fOffsetTerrain,sizeof(float)))
				return false;
			if(!ar.Read(&pro->fOffsetWater,sizeof(float)))
				return false;
			pro->strNpcName = ar.ReadString();
			pro->dwMounsterFaction = 0;
			pro->dwMounsterFactionAccept = 0;
			pro->dwMounsterFactionHelper = 0;
			pro->bNeedHelp = false;
			pro->nLoopType = 1;
			pro->nPathID =  -1;
			pro->nSpeedFlags = 0;
			m_listNPC.AddTail(pro);
		}

		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
		//if(iLoadFlags!= LOAD_EXPLIGHTMAP) ReloadModels();
	}else
	if(dwVersion > 9 && dwVersion<= 0xa)
	{
		m_strName = ar.ReadString();
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.dwAreaType,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeHei,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeLen,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeWth,sizeof(DWORD)))
			return false;
		int num;
		if(!ar.Read(&num,sizeof(int)))
			return false;
		
		for(int i = 0; i<num; i++)
		{
			NPC_PROPERTY* pro = new NPC_PROPERTY;
			Init_npc_property(pro);
			if(!ar.Read(&pro->dwInitiative,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwDiedTimes,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwID,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwNum,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwRefurbish,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->fOffsetTerrain,sizeof(float)))
				return false;
			if(!ar.Read(&pro->fOffsetWater,sizeof(float)))
				return false;
			pro->strNpcName = ar.ReadString();
			pro->dwMounsterFaction = 0;
			pro->dwMounsterFactionAccept = 0;
			pro->dwMounsterFactionHelper = 0;
			pro->bNeedHelp = false;
			pro->nLoopType = 1;
			pro->nPathID =  -1;
			pro->nSpeedFlags = 0;
			m_listNPC.AddTail(pro);
		}

		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
		//if(iLoadFlags!= LOAD_EXPLIGHTMAP) ReloadModels();
	}else if(dwVersion==0xb)
	{
		m_strName = ar.ReadString();
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.dwAreaType,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeHei,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeLen,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeWth,sizeof(DWORD)))
			return false;
		int num;
		if(!ar.Read(&num,sizeof(int)))
			return false;
		
		for(int i = 0; i<num; i++)
		{
			NPC_PROPERTY* pro = new NPC_PROPERTY;
			Init_npc_property(pro);
			if(!ar.Read(&pro->dwInitiative,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwDiedTimes,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwID,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwNum,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwRefurbish,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->fOffsetTerrain,sizeof(float)))
				return false;
			if(!ar.Read(&pro->fOffsetWater,sizeof(float)))
				return false;

			if(!ar.Read(&pro->dwMounsterFaction,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwMounsterFactionAccept,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwMounsterFactionHelper,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->bNeedHelp,sizeof(bool)))
				return false;
			pro->strNpcName = ar.ReadString();
			pro->nLoopType = 1;
			pro->nPathID =  -1;
			pro->nSpeedFlags = 0;
			m_listNPC.AddTail(pro);
		}

		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
		//if(iLoadFlags!= LOAD_EXPLIGHTMAP) ReloadModels();
	}else if(dwVersion > 0xb && dwVersion <= 0xd)
	{
		m_strName = ar.ReadString();
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.dwAreaType,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeHei,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeLen,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeWth,sizeof(DWORD)))
			return false;
		int num;
		if(!ar.Read(&num,sizeof(int)))
			return false;
		
		for(int i = 0; i<num; i++)
		{
			NPC_PROPERTY* pro = new NPC_PROPERTY;
			Init_npc_property(pro);
			if(!ar.Read(&pro->dwInitiative,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwDiedTimes,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwID,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwNum,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwRefurbish,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->fOffsetTerrain,sizeof(float)))
				return false;
			if(!ar.Read(&pro->fOffsetWater,sizeof(float)))
				return false;

			if(!ar.Read(&pro->dwMounsterFaction,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwMounsterFactionAccept,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwMounsterFactionHelper,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->bNeedHelp,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFaction,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFactionAccept,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFactionHelper,sizeof(bool)))
				return false;
			pro->strNpcName = ar.ReadString();
			pro->nLoopType = 1;
			pro->nPathID =  -1;
			pro->nSpeedFlags = 0; 
			m_listNPC.AddTail(pro);
		}

		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
		//if(iLoadFlags!= LOAD_EXPLIGHTMAP)  ReloadModels();
	}else if(dwVersion>0xd && dwVersion <= 0x13)
	{
		m_strName = ar.ReadString();
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.dwAreaType,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeHei,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeLen,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeWth,sizeof(DWORD)))
			return false;
		int num;
		if(!ar.Read(&num,sizeof(int)))
			return false;
		
		for(int i = 0; i<num; i++)
		{
			NPC_PROPERTY* pro = new NPC_PROPERTY;
			Init_npc_property(pro);
			if(!ar.Read(&pro->dwInitiative,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwDiedTimes,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwID,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwNum,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwRefurbish,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->fOffsetTerrain,sizeof(float)))
				return false;
			if(!ar.Read(&pro->fOffsetWater,sizeof(float)))
				return false;

			if(!ar.Read(&pro->dwMounsterFaction,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwMounsterFactionAccept,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwMounsterFactionHelper,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->bNeedHelp,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFaction,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFactionAccept,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFactionHelper,sizeof(bool)))
				return false;
			pro->strNpcName = ar.ReadString();
			pro->nLoopType = 1;
			pro->nPathID =  -1;
			pro->nSpeedFlags = 0; 
			m_listNPC.AddTail(pro);
		}

		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
		//if(iLoadFlags!= LOAD_EXPLIGHTMAP) ReloadModels();	
	}else if(dwVersion > 0x13 && dwVersion <= 0x16)
	{
		m_strName = ar.ReadString();
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.dwAreaType,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeHei,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeLen,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeWth,sizeof(DWORD)))
			return false;
		bool same;
		if(!ar.Read(&same,sizeof(bool)))
			return false;
		m_Data.nMonsterType = 0;
		int num;
		if(!ar.Read(&num,sizeof(int)))
			return false;
		
		for(int i = 0; i<num; i++)
		{
			NPC_PROPERTY* pro = new NPC_PROPERTY;
			Init_npc_property(pro);
			if(!ar.Read(&pro->dwInitiative,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwDiedTimes,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwID,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwNum,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwRefurbish,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->fOffsetTerrain,sizeof(float)))
				return false;
			if(!ar.Read(&pro->fOffsetWater,sizeof(float)))
				return false;

			if(!ar.Read(&pro->dwMounsterFaction,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwMounsterFactionAccept,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwMounsterFactionHelper,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->bNeedHelp,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFaction,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFactionAccept,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFactionHelper,sizeof(bool)))
				return false;
			pro->strNpcName = ar.ReadString();
			pro->nLoopType = 1;
			pro->nPathID =  -1;
			pro->nSpeedFlags = 0;
			m_listNPC.AddTail(pro);
		}

		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
		//if(iLoadFlags!= LOAD_EXPLIGHTMAP) ReloadModels();	
	}else  if(dwVersion > 0x16 && dwVersion <= 0x23)
	{
		m_strName = ar.ReadString();
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.dwAreaType,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeHei,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeLen,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeWth,sizeof(DWORD)))
			return false;
		//bool b = 0;
		if(!ar.Read(&m_Data.nMonsterType,sizeof(int)))
			return false;
		//if(b) m_Data.nMonsterType = 1;
		//else m_Data.nMonsterType = 0;
		int num;
		if(!ar.Read(&num,sizeof(int)))
			return false;
		
		for(int i = 0; i<num; i++)
		{
			NPC_PROPERTY* pro = new NPC_PROPERTY;
			Init_npc_property(pro);
			if(!ar.Read(&pro->dwInitiative,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwDiedTimes,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwID,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwNum,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwRefurbish,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->fOffsetTerrain,sizeof(float)))
				return false;
			if(!ar.Read(&pro->fOffsetWater,sizeof(float)))
				return false;

			if(!ar.Read(&pro->dwMounsterFaction,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwMounsterFactionAccept,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwMounsterFactionHelper,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->bNeedHelp,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFaction,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFactionAccept,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFactionHelper,sizeof(bool)))
				return false;
			pro->strNpcName = ar.ReadString();
			pro->nLoopType = 1;
			pro->nPathID =  -1;
			pro->nSpeedFlags = 0;
			m_listNPC.AddTail(pro);
		}

		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
		//if(iLoadFlags!= LOAD_EXPLIGHTMAP) ReloadModels();	
	}else if(dwVersion > 0x23 && dwVersion <= 0x26)
	{
		m_strName = ar.ReadString();
		//m_Data.strPathName = ar.ReadString();
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.dwAreaType,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeHei,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeLen,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeWth,sizeof(DWORD)))
			return false;
		//bool b = 0;
		if(!ar.Read(&m_Data.nMonsterType,sizeof(int)))
			return false;
		//if(!ar.Read(&m_Data.nPathID,sizeof(int)))
		//	return false;
		//if(!ar.Read(&m_Data.nLoopType,sizeof(int)))
		//	return false;
		//if(!ar.Read(&m_Data.fSpeed,sizeof(float)))
		//	return false;
		//if(b) m_Data.nMonsterType = 1;
		//else m_Data.nMonsterType = 0;
		int num;
		if(!ar.Read(&num,sizeof(int)))
			return false;
		
		for(int i = 0; i<num; i++)
		{
			NPC_PROPERTY* pro = new NPC_PROPERTY;
			Init_npc_property(pro);
			if(!ar.Read(&pro->dwInitiative,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwDiedTimes,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwID,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwNum,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwRefurbish,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->fOffsetTerrain,sizeof(float)))
				return false;
			if(!ar.Read(&pro->fOffsetWater,sizeof(float)))
				return false;

			if(!ar.Read(&pro->dwMounsterFaction,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwMounsterFactionAccept,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwMounsterFactionHelper,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->bNeedHelp,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFaction,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFactionAccept,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFactionHelper,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->nLoopType,sizeof(int)))
				return false;
			if(!ar.Read(&pro->nPathID,sizeof(int)))
				return false;
			pro->strPathName = ar.ReadString();
			pro->strNpcName = ar.ReadString();
			pro->nSpeedFlags = 0;
			m_listNPC.AddTail(pro);
		}

		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
		//if(iLoadFlags!= LOAD_EXPLIGHTMAP) ReloadModels();	
	}else if(dwVersion > 0x26 && dwVersion <= 0x2f)
	{
		m_strName = ar.ReadString();
		//m_Data.strPathName = ar.ReadString();
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.dwAreaType,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeHei,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeLen,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeWth,sizeof(DWORD)))
			return false;
		//bool b = 0;
		if(!ar.Read(&m_Data.nMonsterType,sizeof(int)))
			return false;
		//if(!ar.Read(&m_Data.nPathID,sizeof(int)))
		//	return false;
		//if(!ar.Read(&m_Data.nLoopType,sizeof(int)))
		//	return false;
		//if(!ar.Read(&m_Data.fSpeed,sizeof(float)))
		//	return false;
		//if(b) m_Data.nMonsterType = 1;
		//else m_Data.nMonsterType = 0;
		int num;
		if(!ar.Read(&num,sizeof(int)))
			return false;
		
		for(int i = 0; i<num; i++)
		{
			NPC_PROPERTY* pro = new NPC_PROPERTY;
			Init_npc_property(pro);
			if(!ar.Read(&pro->dwInitiative,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwDiedTimes,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwID,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwNum,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwRefurbish,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->fOffsetTerrain,sizeof(float)))
				return false;
			if(!ar.Read(&pro->fOffsetWater,sizeof(float)))
				return false;

			if(!ar.Read(&pro->dwMounsterFaction,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwMounsterFactionAccept,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwMounsterFactionHelper,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->bNeedHelp,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFaction,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFactionAccept,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFactionHelper,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->nLoopType,sizeof(int)))
				return false;
			if(!ar.Read(&pro->nPathID,sizeof(int)))
				return false;
			if(!ar.Read(&pro->nSpeedFlags,sizeof(int)))
				return false;
			pro->strPathName = ar.ReadString();
			pro->strNpcName = ar.ReadString();

			m_listNPC.AddTail(pro);
		}

		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
		//if(iLoadFlags!= LOAD_EXPLIGHTMAP) ReloadModels();		
	}else if(dwVersion > 0x2f && dwVersion <= 0x30)
	{
		m_strName = ar.ReadString();
		//m_Data.strPathName = ar.ReadString();
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.dwAreaType,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeHei,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeLen,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeWth,sizeof(DWORD)))
			return false;
		//bool b = 0;
		if(!ar.Read(&m_Data.nMonsterType,sizeof(int)))
			return false;
		//if(!ar.Read(&m_Data.nPathID,sizeof(int)))
		//	return false;
		//if(!ar.Read(&m_Data.nLoopType,sizeof(int)))
		//	return false;
		//if(!ar.Read(&m_Data.fSpeed,sizeof(float)))
		//	return false;
		//if(b) m_Data.nMonsterType = 1;
		//else m_Data.nMonsterType = 0;
		int num;
		if(!ar.Read(&num,sizeof(int)))
			return false;
		
		for(int i = 0; i<num; i++)
		{
			NPC_PROPERTY* pro = new NPC_PROPERTY;
			Init_npc_property(pro);
			if(!ar.Read(&pro->dwInitiative,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwDiedTimes,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwID,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwNum,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwRefurbish,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->fOffsetTerrain,sizeof(float)))
				return false;
			if(!ar.Read(&pro->fOffsetWater,sizeof(float)))
				return false;

			if(!ar.Read(&pro->dwMounsterFaction,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwMounsterFactionAccept,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwMounsterFactionHelper,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->bNeedHelp,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFaction,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFactionAccept,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFactionHelper,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->nLoopType,sizeof(int)))
				return false;
			if(!ar.Read(&pro->nPathID,sizeof(int)))
				return false;
			if(!ar.Read(&pro->nSpeedFlags,sizeof(int)))
				return false;
			if(!ar.Read(&pro->dwDisapearTime,sizeof(DWORD)))
				return false;
			pro->strPathName = ar.ReadString();
			pro->strNpcName = ar.ReadString();

			m_listNPC.AddTail(pro);
		}

		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
		//if(iLoadFlags!= LOAD_EXPLIGHTMAP) ReloadModels();		
	}else if(dwVersion > 0x30 && dwVersion <=0x34)
	{
		m_strName = ar.ReadString();
		//m_Data.strPathName = ar.ReadString();
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.dwAreaType,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeHei,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeLen,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeWth,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.nMonsterType,sizeof(int)))
			return false;
		
		if(!ar.Read(&m_Data.bGenMounsterInit,sizeof(bool)))
			return false;
		if(!ar.Read(&cReviveType,sizeof(char)))
			return false;
		if(!ar.Read(&m_Data.bValidAtOneTime,sizeof(bool)))
			return false;
		if(!ar.Read(&m_Data.dwGenID,sizeof(DWORD)))
			return false;

		//if(!ar.Read(&m_Data.nPathID,sizeof(int)))
		//	return false;
		//if(!ar.Read(&m_Data.nLoopType,sizeof(int)))
		//	return false;
		//if(!ar.Read(&m_Data.fSpeed,sizeof(float)))
		//	return false;
		//if(b) m_Data.nMonsterType = 1;
		//else m_Data.nMonsterType = 0;
		int num;
		if(!ar.Read(&num,sizeof(int)))
			return false;
		
		for(int i = 0; i<num; i++)
		{
			NPC_PROPERTY* pro = new NPC_PROPERTY;
			Init_npc_property(pro);
			if(!ar.Read(&pro->dwInitiative,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwDiedTimes,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwID,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwNum,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwRefurbish,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->fOffsetTerrain,sizeof(float)))
				return false;
			if(!ar.Read(&pro->fOffsetWater,sizeof(float)))
				return false;

			if(!ar.Read(&pro->dwMounsterFaction,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwMounsterFactionAccept,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwMounsterFactionHelper,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->bNeedHelp,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFaction,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFactionAccept,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFactionHelper,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->nLoopType,sizeof(int)))
				return false;
			if(!ar.Read(&pro->nPathID,sizeof(int)))
				return false;
			if(!ar.Read(&pro->nSpeedFlags,sizeof(int)))
				return false;
			if(!ar.Read(&pro->dwDisapearTime,sizeof(DWORD)))
				return false;
			pro->strPathName = ar.ReadString();
			pro->strNpcName = ar.ReadString();

			m_listNPC.AddTail(pro);
		}

		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
		//if(iLoadFlags!= LOAD_EXPLIGHTMAP) ReloadModels();		
	}else if(dwVersion > 0x34 && dwVersion <= 0x3b)
	{
		m_strName = ar.ReadString();
		//m_Data.strPathName = ar.ReadString();
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_nCopy,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.dwAreaType,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeHei,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeLen,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeWth,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.nMonsterType,sizeof(int)))
			return false;
		
		
		if(!ar.Read(&m_Data.bGenMounsterInit,sizeof(bool)))
			return false;
		if(!ar.Read(&cReviveType,sizeof(char)))
			return false;
		if(!ar.Read(&m_Data.bValidAtOneTime,sizeof(bool)))
			return false;
		if(!ar.Read(&m_Data.dwGenID,sizeof(DWORD)))
			return false;

		//if(!ar.Read(&m_Data.nPathID,sizeof(int)))
		//	return false;
		//if(!ar.Read(&m_Data.nLoopType,sizeof(int)))
		//	return false;
		//if(!ar.Read(&m_Data.fSpeed,sizeof(float)))
		//	return false;
		//if(b) m_Data.nMonsterType = 1;
		//else m_Data.nMonsterType = 0;
		int num;
		if(!ar.Read(&num,sizeof(int)))
			return false;
		
		for(int i = 0; i<num; i++)
		{
			NPC_PROPERTY* pro = new NPC_PROPERTY;
			Init_npc_property(pro);
			if(!ar.Read(&pro->dwInitiative,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwDiedTimes,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwID,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwNum,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwRefurbish,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->fOffsetTerrain,sizeof(float)))
				return false;
			if(!ar.Read(&pro->fOffsetWater,sizeof(float)))
				return false;

			if(!ar.Read(&pro->dwMounsterFaction,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwMounsterFactionAccept,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwMounsterFactionHelper,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->bNeedHelp,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFaction,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFactionAccept,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFactionHelper,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->nLoopType,sizeof(int)))
				return false;
			if(!ar.Read(&pro->nPathID,sizeof(int)))
				return false;
			if(!ar.Read(&pro->nSpeedFlags,sizeof(int)))
				return false;
			if(!ar.Read(&pro->dwDisapearTime,sizeof(DWORD)))
				return false;
			pro->strPathName = ar.ReadString();
			pro->strNpcName = ar.ReadString();

			m_listNPC.AddTail(pro);
		}

		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
		//if(iLoadFlags!= LOAD_EXPLIGHTMAP) ReloadModels();		
	}else if(dwVersion > 0x3b && dwVersion <= 0x41)
	{
		m_strName = ar.ReadString();
		//m_Data.strPathName = ar.ReadString();
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_nCopy,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.dwAreaType,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeHei,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeLen,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeWth,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.nMonsterType,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nControllerId,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nMounsterLife,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nMounsterNum,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.bGenMounsterInit,sizeof(bool)))
			return false;
		if(!ar.Read(&cReviveType,sizeof(char)))
			return false;
		if(!ar.Read(&m_Data.bValidAtOneTime,sizeof(bool)))
			return false;
		if(!ar.Read(&m_Data.dwGenID,sizeof(DWORD)))
			return false;

		//if(!ar.Read(&m_Data.nPathID,sizeof(int)))
		//	return false;
		//if(!ar.Read(&m_Data.nLoopType,sizeof(int)))
		//	return false;
		//if(!ar.Read(&m_Data.fSpeed,sizeof(float)))
		//	return false;
		//if(b) m_Data.nMonsterType = 1;
		//else m_Data.nMonsterType = 0;
		int num;
		if(!ar.Read(&num,sizeof(int)))
			return false;
		
		for(int i = 0; i<num; i++)
		{
			NPC_PROPERTY* pro = new NPC_PROPERTY;
			Init_npc_property(pro);
			if(!ar.Read(&pro->dwInitiative,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwDiedTimes,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwID,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwNum,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwRefurbish,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->fOffsetTerrain,sizeof(float)))
				return false;
			if(!ar.Read(&pro->fOffsetWater,sizeof(float)))
				return false;

			if(!ar.Read(&pro->dwMounsterFaction,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwMounsterFactionAccept,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwMounsterFactionHelper,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->bNeedHelp,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFaction,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFactionAccept,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFactionHelper,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->nLoopType,sizeof(int)))
				return false;
			if(!ar.Read(&pro->nPathID,sizeof(int)))
				return false;
			if(!ar.Read(&pro->nSpeedFlags,sizeof(int)))
				return false;
			if(!ar.Read(&pro->dwDisapearTime,sizeof(DWORD)))
				return false;
			pro->strPathName = ar.ReadString();
			pro->strNpcName = ar.ReadString();

			m_listNPC.AddTail(pro);
		}

		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
		//if(iLoadFlags!= LOAD_EXPLIGHTMAP) ReloadModels();		
	}else if(dwVersion > 0x41)
	{
		m_strName = ar.ReadString();
		//m_Data.strPathName = ar.ReadString();
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_nCopy,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.dwAreaType,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeHei,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeLen,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fEdgeWth,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.nMonsterType,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nControllerId,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nMounsterLife,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nMounsterNum,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.bGenMounsterInit,sizeof(bool)))
			return false;
		if(!ar.Read(&cReviveType,sizeof(char)))
			return false;
		if(!ar.Read(&m_Data.bValidAtOneTime,sizeof(bool)))
			return false;
		if(!ar.Read(&m_Data.dwGenID,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_bExtraArea,sizeof(bool)))
			return false;
	
		int num;
		if(!ar.Read(&num,sizeof(int)))
			return false;
		int i;
		for(i = 0; i<num; i++)
		{
			NPC_PROPERTY* pro = new NPC_PROPERTY;
			Init_npc_property(pro);
			if(!ar.Read(&pro->dwInitiative,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwDiedTimes,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwID,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwNum,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwRefurbish,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->fOffsetTerrain,sizeof(float)))
				return false;
			if(!ar.Read(&pro->fOffsetWater,sizeof(float)))
				return false;

			if(!ar.Read(&pro->dwMounsterFaction,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwMounsterFactionAccept,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->dwMounsterFactionHelper,sizeof(DWORD)))
				return false;
			if(!ar.Read(&pro->bNeedHelp,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFaction,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFactionAccept,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->bUseDefautFactionHelper,sizeof(bool)))
				return false;
			if(!ar.Read(&pro->nLoopType,sizeof(int)))
				return false;
			if(!ar.Read(&pro->nPathID,sizeof(int)))
				return false;
			if(!ar.Read(&pro->nSpeedFlags,sizeof(int)))
				return false;
			if(!ar.Read(&pro->dwDisapearTime,sizeof(DWORD)))
				return false;
			pro->strPathName = ar.ReadString();
			pro->strNpcName = ar.ReadString();

			m_listNPC.AddTail(pro);
		}

		num = m_listExtraArea.size();
		if(!ar.Read(&num,sizeof(int))) return false;
		for( i = 0; i < num; ++i)
		{
			int extraid; 
			if(!ar.Read(&extraid,sizeof(int)))
				return false;
			m_listExtraArea.push_back(extraid);
		}
		
		
		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
		//if(iLoadFlags!= LOAD_EXPLIGHTMAP) ReloadModels();		
	}

	m_Data.iReviveType = cReviveType;

	if(m_Data.dwGenID != 0 || !m_Data.bGenMounsterInit)
	{
		CString msg;
		msg.Format("注意：需要手动更新区域完成版本转换：%s(区域编号: %d,初始时生成怪物: %d)",GetObjectName(),m_Data.dwGenID, m_Data.bGenMounsterInit);
		g_LogDlg.Log(msg);
	}

	//if(m_bExtraArea) g_Log.Log("aa--------------------------------%d--\r\n",m_nObjectID);

	if(m_Data.nControllerId == -1) m_Data.nControllerId = 0;

	return true;
}

//Save data
bool CSceneAIGenerator::Save(CELArchive& ar,DWORD dwVersion)
{
	if(dwVersion>8)
	{
		ar.WriteString(m_strName);
		//ar.WriteString(m_Data.strPathName);
		if(!ar.Write(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Write(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Write(&m_nCopy,sizeof(int)))
			return false;
		if(!ar.Write(&m_Data.dwAreaType,sizeof(DWORD)))
			return false;
		if(!ar.Write(&m_Data.fEdgeHei,sizeof(DWORD)))
			return false;
		if(!ar.Write(&m_Data.fEdgeLen,sizeof(DWORD)))
			return false;
		if(!ar.Write(&m_Data.fEdgeWth,sizeof(DWORD)))
			return false;
		if(!ar.Write(&m_Data.nMonsterType,sizeof(int)))
			return false;

		if(!ar.Write(&m_Data.nControllerId,sizeof(int)))
			return false;
		if(!ar.Write(&m_Data.nMounsterLife,sizeof(int)))
			return false;
		if(!ar.Write(&m_Data.nMounsterNum,sizeof(int)))
			return false;

		if(!ar.Write(&m_Data.bGenMounsterInit,sizeof(bool)))
			return false;
		char cReviveType = m_Data.iReviveType;
		if(!ar.Write(&cReviveType,sizeof(char)))
			return false;
		if(!ar.Write(&m_Data.bValidAtOneTime,sizeof(bool)))
			return false;
		if(!ar.Write(&m_Data.dwGenID,sizeof(DWORD)))
			return false; 
		if(!ar.Write(&m_bExtraArea,sizeof(bool)))
			return false; 
		//if(!ar.Write(&m_Data.nPathID,sizeof(int)))
		//	return false;
		//if(!ar.Write(&m_Data.nLoopType,sizeof(int)))
		//	return false;
		//if(!ar.Write(&m_Data.fSpeed,sizeof(float)))
		//	return false;

		int num = GetNPCNum();
		if(!ar.Write(&num,sizeof(int)))
			return false;
		
		int i;
		for(i = 0; i<num; i++)
		{
			NPC_PROPERTY* pro = (NPC_PROPERTY*)GetNPCProperty(i);
			
			if(!ar.Write(&pro->dwInitiative,sizeof(DWORD)))
				return false;
			if(!ar.Write(&pro->dwDiedTimes,sizeof(DWORD)))
				return false;
			if(!ar.Write(&pro->dwID,sizeof(DWORD)))
				return false;
			if(!ar.Write(&pro->dwNum,sizeof(DWORD)))
				return false;
			if(!ar.Write(&pro->dwRefurbish,sizeof(DWORD)))
				return false;
			if(!ar.Write(&pro->fOffsetTerrain,sizeof(float)))
				return false;
			if(!ar.Write(&pro->fOffsetWater,sizeof(float)))
				return false;
			if(!ar.Write(&pro->dwMounsterFaction,sizeof(DWORD)))
				return false;
			if(!ar.Write(&pro->dwMounsterFactionAccept,sizeof(DWORD)))
				return false;
			if(!ar.Write(&pro->dwMounsterFactionHelper,sizeof(DWORD)))
				return false;
			if(!ar.Write(&pro->bNeedHelp,sizeof(bool)))
				return false;
			if(!ar.Write(&pro->bUseDefautFaction,sizeof(bool)))
				return false;
			if(!ar.Write(&pro->bUseDefautFactionAccept,sizeof(bool)))
				return false;
			if(!ar.Write(&pro->bUseDefautFactionHelper,sizeof(bool)))
				return false;
			if(!ar.Write(&pro->nLoopType,sizeof(int)))
				return false;
			if(!ar.Write(&pro->nPathID,sizeof(int)))
				return false;
			if(!ar.Write(&pro->nSpeedFlags,sizeof(int)))
				return false;
			if(!ar.Write(&pro->dwDisapearTime,sizeof(DWORD)))
				return false;
			ar.WriteString(pro->strPathName);
			ar.WriteString(pro->strNpcName);
		}

		num = m_listExtraArea.size();
		if(!ar.Write(&num,sizeof(int))) return false;
		for( i = 0; i < num; ++i)
		{
			int extraid = m_listExtraArea[i]; 
			if(!ar.Write(&extraid,sizeof(int)))
				return false;
		}
	}
	return true;
}

bool CSceneAIGenerator::ExpData(AFile *pFile,DWORD dwVersion)
{
	DWORD rs;
	pFile->WriteString(m_strName);
	if(!pFile->Write(&m_vPos,sizeof(A3DVECTOR3),&rs))
		return false;
	if(!pFile->Write(&m_Data.dwAreaType,sizeof(DWORD),&rs))
		return false;
	if(!pFile->Write(&m_Data.fEdgeHei,sizeof(DWORD),&rs))
		return false;
	if(!pFile->Write(&m_Data.fEdgeLen,sizeof(DWORD),&rs))
		return false;
	if(!pFile->Write(&m_Data.fEdgeWth,sizeof(DWORD),&rs))
		return false;
	int num = GetNPCNum();
	if(!pFile->Write(&num,sizeof(int),&rs))
		return false;
	
	for(int i = 0; i<num; i++)
	{
		NPC_PROPERTY* pro = (NPC_PROPERTY*)GetNPCProperty(i);
		
		if(!pFile->Write(&pro->dwInitiative,sizeof(DWORD),&rs))
			return false;
		if(!pFile->Write(&pro->dwDiedTimes,sizeof(DWORD),&rs))
			return false;
		if(!pFile->Write(&pro->dwID,sizeof(DWORD),&rs))
			return false;
		if(!pFile->Write(&pro->dwNum,sizeof(DWORD),&rs))
			return false;
		if(!pFile->Write(&pro->dwRefurbish,sizeof(DWORD),&rs))
			return false;
		if(!pFile->Write(&pro->fOffsetTerrain,sizeof(float),&rs))
			return false;
		if(!pFile->Write(&pro->fOffsetWater,sizeof(float),&rs))
			return false;
		if(!pFile->Write(&pro->dwMounsterFaction,sizeof(DWORD),&rs))
			return false;
		if(!pFile->Write(&pro->dwMounsterFactionAccept,sizeof(DWORD),&rs))
			return false;
		if(!pFile->Write(&pro->dwMounsterFactionHelper,sizeof(DWORD),&rs))
			return false;
		if(!pFile->Write(&pro->bNeedHelp,sizeof(bool),&rs))
			return false;
		if(!pFile->Write(&pro->bUseDefautFaction,sizeof(bool),&rs))
			return false;
		if(!pFile->Write(&pro->bUseDefautFactionAccept,sizeof(bool),&rs))
			return false;
		if(!pFile->Write(&pro->bUseDefautFactionHelper,sizeof(bool),&rs))
			return false;
		pFile->WriteString(pro->strNpcName);
	}
	return true;
}

bool CSceneAIGenerator::ImpData(AFile *pFile,DWORD dwVersion)
{
	if(dwVersion<0xa) return false;
	DWORD rs;
	if(dwVersion==0xa)
	{
		pFile->ReadString(m_strName);
		if(!pFile->Read(&m_vPos,sizeof(A3DVECTOR3),&rs))
			return false;
		if(!pFile->Read(&m_Data.dwAreaType,sizeof(DWORD),&rs))
			return false;
		if(!pFile->Read(&m_Data.fEdgeHei,sizeof(DWORD),&rs))
			return false;
		if(!pFile->Read(&m_Data.fEdgeLen,sizeof(DWORD),&rs))
			return false;
		if(!pFile->Read(&m_Data.fEdgeWth,sizeof(DWORD),&rs))
			return false;
		int num;
		if(!pFile->Read(&num,sizeof(int),&rs))
			return false;
		
		for(int i = 0; i<num; i++)
		{
			NPC_PROPERTY* pro = new NPC_PROPERTY;
			
			if(!pFile->Read(&pro->dwInitiative,sizeof(DWORD),&rs))
				return false;
			if(!pFile->Read(&pro->dwDiedTimes,sizeof(DWORD),&rs))
				return false;
			if(!pFile->Read(&pro->dwID,sizeof(DWORD),&rs))
				return false;
			if(!pFile->Read(&pro->dwNum,sizeof(DWORD),&rs))
				return false;
			if(!pFile->Read(&pro->dwRefurbish,sizeof(DWORD),&rs))
				return false;
			if(!pFile->Read(&pro->fOffsetTerrain,sizeof(float),&rs))
				return false;
			if(!pFile->Read(&pro->fOffsetWater,sizeof(float),&rs))
				return false;

			pFile->ReadString(pro->strNpcName);
			
			m_listNPC.AddTail(pro);
		}
	}else if(dwVersion==0xb)
	{
		pFile->ReadString(m_strName);
		if(!pFile->Read(&m_vPos,sizeof(A3DVECTOR3),&rs))
			return false;
		if(!pFile->Read(&m_Data.dwAreaType,sizeof(DWORD),&rs))
			return false;
		if(!pFile->Read(&m_Data.fEdgeHei,sizeof(DWORD),&rs))
			return false;
		if(!pFile->Read(&m_Data.fEdgeLen,sizeof(DWORD),&rs))
			return false;
		if(!pFile->Read(&m_Data.fEdgeWth,sizeof(DWORD),&rs))
			return false;
		int num;
		if(!pFile->Read(&num,sizeof(int),&rs))
			return false;
		
		for(int i = 0; i<num; i++)
		{
			NPC_PROPERTY* pro = new NPC_PROPERTY;
			
			if(!pFile->Read(&pro->dwInitiative,sizeof(DWORD),&rs))
				return false;
			if(!pFile->Read(&pro->dwDiedTimes,sizeof(DWORD),&rs))
				return false;
			if(!pFile->Read(&pro->dwID,sizeof(DWORD),&rs))
				return false;
			if(!pFile->Read(&pro->dwNum,sizeof(DWORD),&rs))
				return false;
			if(!pFile->Read(&pro->dwRefurbish,sizeof(DWORD),&rs))
				return false;
			if(!pFile->Read(&pro->fOffsetTerrain,sizeof(float),&rs))
				return false;
			if(!pFile->Read(&pro->fOffsetWater,sizeof(float),&rs))
				return false;
			if(!pFile->Read(&pro->dwMounsterFaction,sizeof(DWORD),&rs))
				return false;
			if(!pFile->Read(&pro->dwMounsterFactionAccept,sizeof(DWORD),&rs))
				return false;
			if(!pFile->Read(&pro->dwMounsterFactionHelper,sizeof(DWORD),&rs))
				return false;
			if(!pFile->Read(&pro->bNeedHelp,sizeof(bool),&rs))
				return false;

			pFile->ReadString(pro->strNpcName);
			
			m_listNPC.AddTail(pro);
		}
	}else if(dwVersion>0xb)
	{
		pFile->ReadString(m_strName);
		if(!pFile->Read(&m_vPos,sizeof(A3DVECTOR3),&rs))
			return false;
		if(!pFile->Read(&m_Data.dwAreaType,sizeof(DWORD),&rs))
			return false;
		if(!pFile->Read(&m_Data.fEdgeHei,sizeof(DWORD),&rs))
			return false;
		if(!pFile->Read(&m_Data.fEdgeLen,sizeof(DWORD),&rs))
			return false;
		if(!pFile->Read(&m_Data.fEdgeWth,sizeof(DWORD),&rs))
			return false;
		int num;
		if(!pFile->Read(&num,sizeof(int),&rs))
			return false;
		
		for(int i = 0; i<num; i++)
		{
			NPC_PROPERTY* pro = new NPC_PROPERTY;
			
			if(!pFile->Read(&pro->dwInitiative,sizeof(DWORD),&rs))
				return false;
			if(!pFile->Read(&pro->dwDiedTimes,sizeof(DWORD),&rs))
				return false;
			if(!pFile->Read(&pro->dwID,sizeof(DWORD),&rs))
				return false;
			if(!pFile->Read(&pro->dwNum,sizeof(DWORD),&rs))
				return false;
			if(!pFile->Read(&pro->dwRefurbish,sizeof(DWORD),&rs))
				return false;
			if(!pFile->Read(&pro->fOffsetTerrain,sizeof(float),&rs))
				return false;
			if(!pFile->Read(&pro->fOffsetWater,sizeof(float),&rs))
				return false;
			if(!pFile->Read(&pro->dwMounsterFaction,sizeof(DWORD),&rs))
				return false;
			if(!pFile->Read(&pro->dwMounsterFactionAccept,sizeof(DWORD),&rs))
				return false;
			if(!pFile->Read(&pro->dwMounsterFactionHelper,sizeof(DWORD),&rs))
				return false;
			if(!pFile->Read(&pro->bNeedHelp,sizeof(bool),&rs))
				return false;
			if(!pFile->Read(&pro->bUseDefautFaction,sizeof(bool),&rs))
				return false;
			if(!pFile->Read(&pro->bUseDefautFactionAccept,sizeof(bool),&rs))
				return false;
			if(!pFile->Read(&pro->bUseDefautFactionHelper,sizeof(bool),&rs))
				return false;
			
			pFile->ReadString(pro->strNpcName);
			
			m_listNPC.AddTail(pro);
		}
	}

	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap) m_vPos.y = pMap->GetTerrain()->GetPosHeight(m_vPos) + 0.1f;
	m_matTrans.SetRow(3,m_vPos);
	m_matAbs = m_matScale * m_matRotate * m_matTrans;
	ReloadModels();
	return true;
}

AString CSceneAIGenerator::GetModelPath(DWORD dwId)
{
	AString name;
	DATA_TYPE type = DT_MONSTER_ESSENCE;
	MONSTER_ESSENCE* pData;
	pData = (MONSTER_ESSENCE*)g_dataMan.get_data_ptr(dwId,ID_SPACE_ESSENCE,type);
	if(pData) name = g_dataPathMan.GetPathByID(pData->file_model);
	return name;
}

///////////////////////////////////////////////////////////////////////////////////////
// class CSceneFixedNpcGenerator

AString CSceneFixedNpcGenerator::GetModelPath(DWORD dwId)
{
	AString name;
	if(m_Data.cNpcInfo==1)
	{
		DATA_TYPE type = DT_MONSTER_ESSENCE;
		MONSTER_ESSENCE* pData;
		pData = (MONSTER_ESSENCE*)g_dataMan.get_data_ptr(dwId,ID_SPACE_ESSENCE,type);
		if(pData) name = g_dataPathMan.GetPathByID(pData->file_model);
		return name;
		
	}
	else if(m_Data.cNpcInfo==0)
	{
		DATA_TYPE type = DT_NPC_ESSENCE;
		NPC_ESSENCE* pData;
		pData = (NPC_ESSENCE*)g_dataMan.get_data_ptr(dwId,ID_SPACE_ESSENCE,type);
		if(pData) name = g_dataPathMan.GetPathByID(pData->file_model);
		return name;
	}
	else if(m_Data.cNpcInfo==2)
	{
		DATA_TYPE type = DT_INTERACTION_OBJECT_ESSENCE;
		INTERACTION_OBJECT_ESSENCE* pData;
		pData = (INTERACTION_OBJECT_ESSENCE*)g_dataMan.get_data_ptr(dwId,ID_SPACE_ESSENCE,type);
		if(pData) name = g_dataPathMan.GetPathByID(pData->file_matter);
		return name;
	}
	return name;
}

int CSceneFixedNpcGenerator::GetSightRange(DWORD dwId)
{
	int sight = 0;
	if(m_Data.cNpcInfo==1)
	{
		DATA_TYPE type = DT_MONSTER_ESSENCE;
		MONSTER_ESSENCE* pData;
		pData = (MONSTER_ESSENCE*)g_dataMan.get_data_ptr(dwId,ID_SPACE_ESSENCE,type);
		if(pData)  sight = pData->sight_range;
		return sight;
	}
	return sight;
}

AString CSceneFixedNpcGenerator::GetNPCName(DWORD dwId)
{
	AString name;
	if(m_Data.cNpcInfo==1)
	{
		DATA_TYPE type = DT_MONSTER_ESSENCE;
		MONSTER_ESSENCE* pData;
		pData = (MONSTER_ESSENCE*)g_dataMan.get_data_ptr(dwId,ID_SPACE_ESSENCE,type);
		if(pData)  name = WC2AS((const wchar_t*)pData->name);
		return name;
	}
	return name;
}

CSceneFixedNpcGenerator::CSceneFixedNpcGenerator()
{
	init_fixed_npc_data(m_Data);
	m_nObjectType = SO_TYPE_FIXED_NPC;
	m_pIDSelFuncs = new FIXED_NPC_ID_FUNCS();
	m_pBezierSelFuncs = new NPC_BEZIER_SEL_ID_FUNCS();
	m_pControllerSelFuncs = new CONTROLLER_NPC_ID_FUNCS();
	m_pExtraIDSelFuncs = new NPC_EXTRA_ID_FUNCS();
	m_pModel = NULL;
	m_nSight = 0;
	m_nCopy = 0;
	m_nObjectID = 0;
	m_bExtraArea = false; 
	npc_type.AddElement("随地形起伏",TYPE_TERRAIN_FOLLOW);
	npc_type.AddElement("平面",TYPE_PLANE);
	
	loop_type.AddElement("终点停止",0);
	loop_type.AddElement("原路返回",1);
	loop_type.AddElement("始终循环",2);
	speed_type.AddElement("慢速移动",0);
	speed_type.AddElement("快速移动",1);
	
	revive_type.AddElement("不重生",0);
	revive_type.AddElement("消失后重生",1);
	revive_type.AddElement("开启后重生",2);

	BuildProperty();
}

CSceneFixedNpcGenerator::~CSceneFixedNpcGenerator()
{
}

void CSceneFixedNpcGenerator::Render(A3DViewport* pA3DViewport)
{
	g_pA3DConfig->RT_SetShowBoundBoxFlag(g_Configs.bShowBoundBox);

	if(m_nCopy != g_Configs.nShowCopyNum) return;
	if(!g_Configs.bShowFixedNpc) return;
	

	if(!g_bShowPosDot) return;
	
	A3DVECTOR3 vPos = m_matTrans.GetRow(3);
	A3DVECTOR3 vScreenPos,vPosEx,vScreenPosEx;
	
	//求投影比例
	A3DVECTOR3 camPos = g_Render.GetA3DEngine()->GetActiveCamera()->GetPos();
	A3DVECTOR3 mAxisGizmoCenter = m_vPos;
	// assumes a 90deg FOV
	A3DVECTOR3 dir = mAxisGizmoCenter - camPos;
	A3DVIEWPORTPARAM* param = g_Render.GetA3DEngine()->GetActiveViewport()->GetParam();

	A3DMATRIX4 matScale;
	float mAxisGizmoProjLen = (2.0f/(float)param->Height) * dir.Magnitude() * tan(A3D_PI/180.0f * 45.0f);
	m_fScaleUnit = mAxisGizmoProjLen;

	if(!g_Render.GetA3DEngine()->GetActiveCamera()->GetWorldFrustum()->PointInFrustum(m_vPos)) return;
	
	CBox3D renderBox;
	renderBox.Init(&g_Render,m_fScaleUnit,true);
	renderBox.SetDirAndUp(A3DVECTOR3(0,0,1.0f),A3DVECTOR3(0,1.0f,0));
	if(m_bSelected)
		renderBox.SetColor(A3DCOLORRGB(255,0,0));
	else 
	{
		if(m_bExtraArea) renderBox.SetColor(A3DCOLORRGB(0,0,255));
		else 
		{
			if(m_listExtraArea.size() > 0) renderBox.SetColor(A3DCOLORRGB(0,0,125));
			else renderBox.SetColor(A3DCOLORRGB(255,255,0));
		}
	}
	renderBox.SetPos(m_vPos);
	renderBox.Render();

	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	LIGHTINGPARAMS light = pMap->GetLightingParams();
	A3DSkinModel::LIGHTINFO LightInfo;
	LightInfo.colAmbient	= A3DCOLORVALUE(light.dwAmbient);//	Directional light's specular color
	LightInfo.vLightDir		= light.vSunDir;//Direction of directional light
	LightInfo.colDirDiff	= A3DCOLORVALUE(light.dwSunCol);//Directional light's diffuse color
	LightInfo.colDirSpec	= A3DCOLORVALUE(0.8f,0.8f,0.8f,1.0f);//	Directional light's specular color
	LightInfo.bPtLight		= false;//	false, disable dynamic point light,
	if(m_nSight>0 && m_nSight < 2048 && g_Configs.bShowFixedNpcWire)
	{//显示视野范围
		A3DWireCollector *pWireCollector = g_Render.GetA3DEngine()->GetA3DWireCollector();
		pWireCollector->AddSphere(m_vPos,m_nSight,A3DCOLORRGB(128,128,128));
#ifndef  _ANGELICA22
		pWireCollector->Flush();
#endif
	}
	if(m_pModel) 
	{
		m_pModel->GetA3DSkinModel()->SetLightInfo(LightInfo);
		m_pModel->Render(pA3DViewport);

		if(g_Configs.bShowBoundBox)
		{
			float fWidth  = m_pModel->GetModelAABB().Extents.x * 2.0f;
			float fHeight = m_pModel->GetModelAABB().Extents.y * 2.0f;
			float fLength = m_pModel->GetModelAABB().Extents.z * 2.0f;

			if(!(fabs(fWidth)<1e-6 || fabs(fHeight)<1e-6 || fabs(fLength)<1e-6)) // no need to display models with no aabb
			{
				AString str;
				str.Format("(%.2f,%.2f,%.2f)", fWidth, fLength, fHeight);

				A3DVECTOR3 vScreenPos;
				g_Render.GetA3DEngine()->GetActiveViewport()->Transform(m_vPos + A3DVECTOR3(0.0f, fHeight * 1.5f, 0.0f), vScreenPos);
				if(g_Render.GetA3DEngine()->GetActiveCamera()->GetWorldFrustum()->PointInFrustum(m_vPos))
					g_Render.TextOut(vScreenPos.x,vScreenPos.y, str, A3DCOLORRGB(255,255,0));
			}
		}
	}
	
	CSceneObject::Render(pA3DViewport);
}


void	CSceneFixedNpcGenerator::PostRender(A3DViewport* pA3DViewport)
{

	if(m_nCopy != g_Configs.nShowCopyNum) return;

	CBox3D renderBox;
	renderBox.Init(&g_Render,m_fScaleUnit,true);
	renderBox.SetDirAndUp(A3DVECTOR3(0,0,1.0f),A3DVECTOR3(0,1.0f,0));
	if(m_bSelected)
		renderBox.SetColor(A3DCOLORRGB(255,0,0));
	else 
	{
		if(m_bExtraArea) renderBox.SetColor(A3DCOLORRGB(0,0,255));
		else 
		{
			if(m_listExtraArea.size() > 0) renderBox.SetColor(A3DCOLORRGB(0,0,125));
			else renderBox.SetColor(A3DCOLORRGB(255,255,0));
		}
	}
	renderBox.SetPos(m_vPos);
	renderBox.Render();

	DrawCenter();

	CSceneObject::PostRender(pA3DViewport);

}


void CSceneFixedNpcGenerator::Tick(DWORD timeDelta)
{
	if(m_nCopy != g_Configs.nShowCopyNum) return;

	A3DVECTOR3 vScreenPos;
	g_Render.GetA3DEngine()->GetActiveViewport()->Transform(m_vPos, vScreenPos);
	if(!g_Render.GetA3DEngine()->GetActiveCamera()->GetWorldFrustum()->PointInFrustum(m_vPos))
		return;

	if(m_pModel) 
	{	
		if(m_pModel->GetA3DSkinModel())
		{
			m_pModel->GetA3DSkinModel()->SetPos(m_vPos);
			m_pModel->Tick(timeDelta);
		}
	}
}

void CSceneFixedNpcGenerator::UpdateProperty(bool bGet)
{
	if(bGet)
	{
		AString temp = AString(m_pProperty->GetPropVal(0));
		CElementMap *pMap = AUX_GetMainFrame()->GetMap();
		if(pMap)
		{
			CSceneObjectManager *pSManager = pMap->GetSceneObjectMan();
			CSceneObject*pObj = pSManager->FindSceneObject(temp);
			if(pObj != NULL && pObj != this)
			{
				AfxMessageBox("该对象的名字已经存在，编辑器将恢复原对象名字！",NULL,MB_OK);
				m_pProperty->SetPropVal(0,m_strName);
				m_pProperty->SetPropVal(1,m_vPos);
				m_pProperty->SetPropVal(2,m_nCopy);
				m_pIDSelFuncs->OnSetValue(m_Data.strNpcName);
				m_pProperty->SetPropVal(4,m_Data.dwID);
				m_pProperty->SetPropVal(5,m_Data.dwRefurbish);
				m_pProperty->SetPropVal(6,m_Data.fRadius);
				m_pProperty->SetPropVal(7,m_Data.dwNpcType);
				m_pProperty->SetPropVal(9,m_Data.nLoopType);
				m_pProperty->SetPropVal(10,m_Data.nSpeedFlags);
				m_pProperty->SetPropVal(11,m_Data.dwDisapearTime);
				m_pProperty->SetPropVal(12,m_Data.iReviveType);
				m_pProperty->SetPropVal(13,m_Data.nMounsterNum);
				m_pProperty->SetPropVal(14,m_Data.nMounsterLife);
				m_pProperty->SetPropVal(17,m_bExtraArea);
				m_pControllerSelFuncs->OnSetValue(m_Data.nControllerId);
				m_pExtraIDSelFuncs->OnSetValue("");
				m_pProperty->SetPropVal(16,m_Data.bPatrol);
				m_pBezierSelFuncs->OnSetValue(m_Data.strPathName);
				m_pProperty->SetPropVal(19,m_nObjectID);
				m_pProperty->SetPropVal(20,(int)m_Data.sPhase);
				AUX_GetMainFrame()->GetToolTabWnd()->UpdatePropertyData(false);
				return;
			}
		}
		m_strName	        = temp;
		m_vPos	            = m_pProperty->GetPropVal(1);
		m_nCopy	            = m_pProperty->GetPropVal(2);
		m_Data.dwRefurbish     = m_pProperty->GetPropVal(5);
		m_Data.fRadius     = m_pProperty->GetPropVal(6);
		m_Data.dwNpcType   = m_pProperty->GetPropVal(7);
		m_Data.nLoopType   = m_pProperty->GetPropVal(9);
		m_Data.nSpeedFlags = m_pProperty->GetPropVal(10);
		m_Data.dwDisapearTime = m_pProperty->GetPropVal(11);

		m_Data.iReviveType = m_pProperty->GetPropVal(12);
		m_Data.nMounsterNum = m_pProperty->GetPropVal(13);
		m_Data.nMounsterLife = m_pProperty->GetPropVal(14);
	
		m_Data.bPatrol = m_pProperty->GetPropVal(16);
		m_bExtraArea = m_pProperty->GetPropVal(17);
		m_Data.sPhase = m_pProperty->GetPropVal(20);
		if(m_Data.dwGenID!=0 || !m_Data.bGenMounsterInit)
		{
			if(IDYES==AfxMessageBox("这个区域已经被你更新，由于是旧版数据，现需要完成版本转换，你确定要转换吗？",MB_YESNO|MB_ICONQUESTION))
			{
				m_Data.bGenMounsterInit = true;
				m_Data.dwGenID = 0;
			}
		}

		m_matTrans.Translate(m_vPos.x,m_vPos.y,m_vPos.z);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
		ReloadModel();
	}else
	{
		m_pProperty->SetPropVal(0,m_strName);
		m_pProperty->SetPropVal(1,m_vPos);
		m_pProperty->SetPropVal(2,m_nCopy);
		m_pIDSelFuncs->OnSetValue(m_Data.strNpcName);
		m_pProperty->SetPropVal(4,m_Data.dwID);
		m_pProperty->SetPropVal(5,m_Data.dwRefurbish);
		m_pProperty->SetPropVal(6,m_Data.fRadius);
		m_pProperty->SetPropVal(7,m_Data.dwNpcType);
		m_pProperty->SetPropVal(9,m_Data.nLoopType);
		m_pProperty->SetPropVal(10,m_Data.nSpeedFlags);
		m_pProperty->SetPropVal(11,m_Data.dwDisapearTime);
		m_pProperty->SetPropVal(12,m_Data.iReviveType);
		m_pProperty->SetPropVal(13,m_Data.nMounsterNum);
		m_pProperty->SetPropVal(14,m_Data.nMounsterLife);
		m_pProperty->SetPropVal(17,m_bExtraArea);
		m_pControllerSelFuncs->OnSetValue(m_Data.nControllerId);
		m_pExtraIDSelFuncs->OnSetValue("");
		m_pBezierSelFuncs->OnSetValue(m_Data.strPathName);
		m_pProperty->SetPropVal(19,m_nObjectID);
		m_pProperty->SetPropVal(20,(int)m_Data.sPhase);
	}
}

void CSceneFixedNpcGenerator::DelExtra( int id)
{
	int n = m_listExtraArea.size();
	for( int i = 0; i < n; ++i)
	{
		if(m_listExtraArea[i] == id)
		{
			m_listExtraArea.erase(&m_listExtraArea[i]);
			break;
		}
	}
}

void CSceneFixedNpcGenerator::BuildProperty()
{
	if(m_pProperty)
	{
		m_pProperty->DynAddProperty(AVariant(m_strName), "名字");
		m_pProperty->DynAddProperty(AVariant(m_vPos), "位置坐标");
		m_pProperty->DynAddProperty(AVariant(m_nCopy),"第几套数据");
		m_pProperty->DynAddProperty(AVariant(m_strText), "选择NPC",(ASet*)m_pIDSelFuncs, NULL, WAY_CUSTOM);
		m_pProperty->DynAddProperty(AVariant(m_Data.dwID),"NPC id",NULL,NULL,WAY_READONLY);
		m_pProperty->DynAddProperty(AVariant(m_Data.dwRefurbish),"刷新速度(s)");
		m_pProperty->DynAddProperty(AVariant(m_Data.fRadius),"随机半径");
		m_pProperty->DynAddProperty(AVariant(m_Data.dwNpcType),"NPC类型", &npc_type);
		m_pProperty->DynAddProperty(AVariant(m_strText), "选择移动路径",(ASet*)m_pBezierSelFuncs, NULL, WAY_CUSTOM);
		m_pProperty->DynAddProperty(AVariant(m_Data.nLoopType),"沿路径循环类型", &loop_type);
		m_pProperty->DynAddProperty(AVariant(m_Data.nSpeedFlags),"沿路径移动速度", &speed_type);
		m_pProperty->DynAddProperty(AVariant(m_Data.dwDisapearTime),"死亡消失时间(s)");
		//m_pProperty->DynAddProperty(AVariant(m_Data.bGenMounsterInit), "是否起始时生成");
		//m_pProperty->DynAddProperty(AVariant(m_Data.iReviveType), "重生模式");
		//m_pProperty->DynAddProperty(AVariant(m_Data.bValidAtOneTime), "是否同时生效一次");		
		//m_pProperty->DynAddProperty(AVariant(m_Data.dwGenID), "编号");
		m_pProperty->DynAddProperty(AVariant(m_Data.iReviveType), "重生模式", &revive_type);
		m_pProperty->DynAddProperty(AVariant(m_Data.nMounsterNum), "生成对象的最大数");
		m_pProperty->DynAddProperty(AVariant(m_Data.nMounsterLife), " 生成对象的寿命");
		m_pProperty->DynAddProperty(AVariant(m_Data.nControllerId), "关联怪物控制器", (ASet*)m_pControllerSelFuncs, NULL, WAY_CUSTOM);
		m_pProperty->DynAddProperty(AVariant(m_Data.bPatrol), "怪物自动巡逻");
		m_pProperty->DynAddProperty(AVariant(m_bExtraArea), "是否为附属区域");
		m_pProperty->DynAddProperty(AVariant(""), "附属区域列表", (ASet*)m_pExtraIDSelFuncs, NULL, WAY_CUSTOM);
		m_pProperty->DynAddProperty(AVariant(m_nObjectID), "场景对象ID（海外版专用）", NULL,NULL,WAY_READONLY);
		m_pProperty->DynAddProperty(AVariant(m_Data.sPhase), "相位(1-200)");
	}	
}


CSceneObject* CSceneFixedNpcGenerator::CopyObject()
{
	CSceneFixedNpcGenerator *pNewObject = new CSceneFixedNpcGenerator();
	if(pNewObject==NULL) return NULL;
	int Num = 0;
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		AString name;
		CSceneObjectManager *pSManager = pMap->GetSceneObjectMan();
		pSManager->ClearSelectedList();
		while(1)
		{
			name.Format("FixedNpc_%d",Num);
			if(pSManager->FindSceneObject(name)!=NULL)
			{
				Num++;
			}else 
			{
				Num++;
				break;
			}
		}
		pSManager->InsertSceneObject(pNewObject);
		pSManager->AddObjectPtrToSelected(pNewObject);
		pNewObject->SetObjectName(name);
		pNewObject->SetPos(m_vPos +	A3DVECTOR3(0.1f,0.0f,0.1f));
		pNewObject->SetCopyFlags(GetCopyFlags());
		pNewObject->SetProperty(GetProperty());
		
		pNewObject->SetExtraArea(IsExtraArea());
		for( unsigned int i = 0; i < m_listExtraArea.size(); ++i)
			pNewObject->AddExtra(m_listExtraArea[i]);

		pNewObject->ReloadModel();
		pNewObject->UpdateProperty(false);
		AUX_GetMainFrame()->GetToolTabWnd()->ShowPropertyPannel(pNewObject->m_pProperty);//把灯光的属性给对象属性表
		AUX_GetMainFrame()->GetToolTabWnd()->UpdatePropertyData(false);//输出属性数据
	}
	return pNewObject;
}

void CSceneFixedNpcGenerator::ReloadModel()
{
	AString pathName = GetModelPath(m_Data.dwID);
	if(pathName.IsEmpty())
	{
		g_Log.Log("Warning: CSceneFixedNpcGenerator::ReloadModel(),Model's path is empty!");
		return;
	}

	if(m_pModel) m_pModel->Release();
	else m_pModel = new CECModel;
	ASSERT(m_pModel);

	if(!m_pModel->Load(pathName,false,A3DSkinModel::LSF_NOSKIN,true,false))
	{
		g_Log.Log("CSceneFixedNpcGenerator::ReloadModel() failed! Can't load file %s",pathName);
		m_pModel->Release();
		if(!m_pModel->Load("Models\\error\\error.ecm"))
		{
			m_pModel->Release();
			delete m_pModel;
			m_pModel = NULL;
			m_nSight = GetSightRange(m_Data.dwID);
			return;
		}
	}

	//这儿重新加载皮肤
	AString strSkin = pathName;
	int pos = strSkin.ReverseFind('.');
	strSkin = strSkin.Left(pos);
	strSkin += ".ski";

	A3DSkinMan* pSkinMan = g_Render.GetA3DEngine()->GetA3DSkinMan();
	A3DSkin* pSkin = pSkinMan->LoadSkinFile(strSkin);
	if(pSkin)
	{
		m_pModel->GetA3DSkinModel()->AddSkin(pSkin,true);
	}else 
	{
		g_Log.Log("CSceneFixedNpcGenerator::LoadSkinModel(), Failed load %s", strSkin);
		m_pModel->Release();
		delete m_pModel;
		m_pModel = NULL;
		return ;
	}

	m_pModel->GetA3DSkinModel()->SetPos(m_vPos);
	m_pModel->GetA3DSkinModel()->SetDirAndUp(m_matRotate.GetRow(2),m_matRotate.GetRow(1));
	m_nSight = GetSightRange(m_Data.dwID);
}

void  CSceneFixedNpcGenerator::Release()
{
	if(m_pModel)
	{
		m_pModel->Release();
		delete m_pModel;
	}
	if(m_pIDSelFuncs) delete m_pIDSelFuncs;
	if(m_pBezierSelFuncs) delete m_pBezierSelFuncs;
	if(m_pControllerSelFuncs) delete m_pControllerSelFuncs;
	if(m_pExtraIDSelFuncs) delete m_pExtraIDSelFuncs;
}

//Load data
bool CSceneFixedNpcGenerator::Load(CELArchive& ar,DWORD dwVersion,int iLoadFlags)
{
	init_fixed_npc_data(m_Data);
	char cReviveType = m_Data.iReviveType;
	if(dwVersion>8 && dwVersion<=0xd)
	{
		m_strName = ar.ReadString();
		m_Data.strNpcName = ar.ReadString();
		
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.dwID,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.dwRefurbish,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fRadius,sizeof(float)))
			return false;
		
		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
		//if(iLoadFlags!= LOAD_EXPLIGHTMAP && iLoadFlags != LOAD_EXPSCENE) ReloadModel();
	}else if(dwVersion>0xd && dwVersion <= 0x11)
	{
		m_strName = ar.ReadString();
		m_Data.strNpcName = ar.ReadString();
		
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.dwID,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.dwRefurbish,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fRadius,sizeof(float)))
			return false;
		
		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
		//if(iLoadFlags!= LOAD_EXPLIGHTMAP && iLoadFlags != LOAD_EXPSCENE) ReloadModel();
	}else if(dwVersion > 0x11 && dwVersion <= 0x14)
	{
		m_strName = ar.ReadString();
		m_Data.strNpcName = ar.ReadString();

		A3DVECTOR3 vr,vu,vl;
		if(!ar.Read(&vr,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vu,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vl,sizeof(A3DVECTOR3)))
			return false;
		
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.dwID,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.dwRefurbish,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fRadius,sizeof(float)))
			return false;
		if(m_Data.cNpcInfo==1) m_Data.bValidAtOneTime = false;
		m_matRotate.SetRow(2,vr);
		m_matRotate.SetRow(1,vu);
		m_matRotate.SetRow(0,vl);
		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
		//if(iLoadFlags!= LOAD_EXPLIGHTMAP && iLoadFlags != LOAD_EXPSCENE) ReloadModel();
	}else if(dwVersion > 0x14 && dwVersion <= 0x23)
	{
		m_strName = ar.ReadString();
		m_Data.strNpcName = ar.ReadString();

		A3DVECTOR3 vr,vu,vl;
		if(!ar.Read(&vr,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vu,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vl,sizeof(A3DVECTOR3)))
			return false;
		
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.dwID,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.dwRefurbish,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fRadius,sizeof(float)))
			return false;
		if(!ar.Read(&m_Data.dwNpcType,sizeof(DWORD)))
			return false;
		if(m_Data.cNpcInfo==1) m_Data.bValidAtOneTime = false;
		m_matRotate.SetRow(2,vr);
		m_matRotate.SetRow(1,vu);
		m_matRotate.SetRow(0,vl);
		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
		//if(iLoadFlags!= LOAD_EXPLIGHTMAP && iLoadFlags != LOAD_EXPSCENE) ReloadModel();
	}else if(dwVersion > 0x23 && dwVersion <= 0x26)
	{
		m_strName = ar.ReadString();
		m_Data.strNpcName = ar.ReadString();
		m_Data.strPathName = ar.ReadString();
		A3DVECTOR3 vr,vu,vl;
		if(!ar.Read(&vr,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vu,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vl,sizeof(A3DVECTOR3)))
			return false;
		
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.dwID,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.dwRefurbish,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fRadius,sizeof(float)))
			return false;
		if(!ar.Read(&m_Data.dwNpcType,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.nPathID,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nLoopType,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.cNpcInfo,sizeof(char)))
			return false;
		if(m_Data.cNpcInfo==1) m_Data.bValidAtOneTime = false;
		m_matRotate.SetRow(2,vr);
		m_matRotate.SetRow(1,vu);
		m_matRotate.SetRow(0,vl);
		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
		//if(iLoadFlags!= LOAD_EXPLIGHTMAP && iLoadFlags != LOAD_EXPSCENE) ReloadModel();
	}else if(dwVersion > 0x26 && dwVersion <= 0x2f)
	{
		m_strName = ar.ReadString();
		m_Data.strNpcName = ar.ReadString();
		m_Data.strPathName = ar.ReadString();
		A3DVECTOR3 vr,vu,vl;
		if(!ar.Read(&vr,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vu,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vl,sizeof(A3DVECTOR3)))
			return false;
		
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.dwID,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.dwRefurbish,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fRadius,sizeof(float)))
			return false;
		if(!ar.Read(&m_Data.dwNpcType,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.nSpeedFlags,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nPathID,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nLoopType,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.cNpcInfo,sizeof(char)))
			return false;
		if(m_Data.cNpcInfo==1) m_Data.bValidAtOneTime = false;
		m_matRotate.SetRow(2,vr);
		m_matRotate.SetRow(1,vu);
		m_matRotate.SetRow(0,vl);
		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
		//if(iLoadFlags!= LOAD_EXPLIGHTMAP && iLoadFlags != LOAD_EXPSCENE) ReloadModel();
	}else if(dwVersion > 0x2f && dwVersion <= 0x30)
	{
		m_strName = ar.ReadString();
		m_Data.strNpcName = ar.ReadString();
		m_Data.strPathName = ar.ReadString();
		A3DVECTOR3 vr,vu,vl;
		if(!ar.Read(&vr,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vu,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vl,sizeof(A3DVECTOR3)))
			return false;
		
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.dwID,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.dwRefurbish,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fRadius,sizeof(float)))
			return false;
		if(!ar.Read(&m_Data.dwNpcType,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.nSpeedFlags,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nPathID,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nLoopType,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.cNpcInfo,sizeof(char)))
			return false;
		if(!ar.Read(&m_Data.dwDisapearTime,sizeof(DWORD)))
			return false;
		if(m_Data.cNpcInfo==1) m_Data.bValidAtOneTime = false;
		m_matRotate.SetRow(2,vr);
		m_matRotate.SetRow(1,vu);
		m_matRotate.SetRow(0,vl);
		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
		//if(iLoadFlags!= LOAD_EXPLIGHTMAP && iLoadFlags != LOAD_EXPSCENE) ReloadModel();
	}else if(dwVersion > 0x30 && dwVersion <= 0x31)
	{
		m_strName = ar.ReadString();
		m_Data.strNpcName = ar.ReadString();
		m_Data.strPathName = ar.ReadString();
		A3DVECTOR3 vr,vu,vl;
		if(!ar.Read(&vr,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vu,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vl,sizeof(A3DVECTOR3)))
			return false;
		
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.dwID,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.dwRefurbish,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fRadius,sizeof(float)))
			return false;
		if(!ar.Read(&m_Data.dwNpcType,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.nSpeedFlags,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nPathID,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nLoopType,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.cNpcInfo,sizeof(char)))
			return false;
		if(!ar.Read(&m_Data.dwDisapearTime,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.bGenMounsterInit,sizeof(bool)))
			return false;
		if(!ar.Read(&cReviveType,sizeof(char)))
			return false;
		if(!ar.Read(&m_Data.bValidAtOneTime,sizeof(bool)))
			return false;
		if(!ar.Read(&m_Data.dwGenID,sizeof(DWORD)))
			return false;
		
		m_matRotate.SetRow(2,vr);
		m_matRotate.SetRow(1,vu);
		m_matRotate.SetRow(0,vl);
		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
		//if(iLoadFlags!= LOAD_EXPLIGHTMAP && iLoadFlags != LOAD_EXPSCENE) ReloadModel();
	}else if(dwVersion > 0x31 && dwVersion <= 0x34)
	{
		m_strName = ar.ReadString();
		m_Data.strNpcName = ar.ReadString();
		m_Data.strPathName = ar.ReadString();
		A3DVECTOR3 vr,vu,vl;
		if(!ar.Read(&vr,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vu,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vl,sizeof(A3DVECTOR3)))
			return false;
		
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.dwID,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.dwRefurbish,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fRadius,sizeof(float)))
			return false;
		if(!ar.Read(&m_Data.dwNpcType,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.nSpeedFlags,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nPathID,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nLoopType,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.cNpcInfo,sizeof(char)))
			return false;
		if(!ar.Read(&m_Data.dwDisapearTime,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.bGenMounsterInit,sizeof(bool)))
			return false;
		if(!ar.Read(&cReviveType,sizeof(char)))
			return false;
		if(!ar.Read(&m_Data.bValidAtOneTime,sizeof(bool)))
			return false;
		if(!ar.Read(&m_Data.dwGenID,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.bPatrol,sizeof(bool)))
			return false;
		
		m_matRotate.SetRow(2,vr);
		m_matRotate.SetRow(1,vu);
		m_matRotate.SetRow(0,vl);
		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
		//if(iLoadFlags!= LOAD_EXPLIGHTMAP && iLoadFlags != LOAD_EXPSCENE) ReloadModel();
	}else if(dwVersion > 0x34 && dwVersion <= 0x3b)
	{
		m_strName = ar.ReadString();
		m_Data.strNpcName = ar.ReadString();
		m_Data.strPathName = ar.ReadString();
		A3DVECTOR3 vr,vu,vl;
		if(!ar.Read(&vr,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vu,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vl,sizeof(A3DVECTOR3)))
			return false;
		
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_nCopy,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.dwID,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.dwRefurbish,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fRadius,sizeof(float)))
			return false;
		if(!ar.Read(&m_Data.dwNpcType,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.nSpeedFlags,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nPathID,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nLoopType,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.cNpcInfo,sizeof(char)))
			return false;
		if(!ar.Read(&m_Data.dwDisapearTime,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.bGenMounsterInit,sizeof(bool)))
			return false;
		if(!ar.Read(&cReviveType,sizeof(char)))
			return false;
		if(!ar.Read(&m_Data.bValidAtOneTime,sizeof(bool)))
			return false;
		if(!ar.Read(&m_Data.dwGenID,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.bPatrol,sizeof(bool)))
			return false;
		
		m_matRotate.SetRow(2,vr);
		m_matRotate.SetRow(1,vu);
		m_matRotate.SetRow(0,vl);
		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
		//if(iLoadFlags!= LOAD_EXPLIGHTMAP && iLoadFlags != LOAD_EXPSCENE) ReloadModel();
	}else if(dwVersion > 0x3b && dwVersion <= 0x41)
	{
		m_strName = ar.ReadString();
		m_Data.strNpcName = ar.ReadString();
		m_Data.strPathName = ar.ReadString();
		A3DVECTOR3 vr,vu,vl;
		if(!ar.Read(&vr,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vu,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vl,sizeof(A3DVECTOR3)))
			return false;
		
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_nCopy,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.dwID,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.dwRefurbish,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fRadius,sizeof(float)))
			return false;
		if(!ar.Read(&m_Data.dwNpcType,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.nSpeedFlags,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nPathID,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nLoopType,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.cNpcInfo,sizeof(char)))
			return false;
		if(!ar.Read(&m_Data.dwDisapearTime,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.bGenMounsterInit,sizeof(bool)))
			return false;
		if(!ar.Read(&cReviveType,sizeof(char)))
			return false;
		if(!ar.Read(&m_Data.bValidAtOneTime,sizeof(bool)))
			return false;
		if(!ar.Read(&m_Data.dwGenID,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.bPatrol,sizeof(bool)))
			return false;
		if(!ar.Read(&m_Data.nControllerId,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nMounsterLife,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nMounsterNum,sizeof(int)))
			return false;
		
		m_matRotate.SetRow(2,vr);
		m_matRotate.SetRow(1,vu);
		m_matRotate.SetRow(0,vl);
		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
		//if(iLoadFlags!= LOAD_EXPLIGHTMAP && iLoadFlags != LOAD_EXPSCENE) ReloadModel();
	}else if(dwVersion > 0x41 && dwVersion <= 0x4f)
	{
		m_strName = ar.ReadString();
		m_Data.strNpcName = ar.ReadString();
		m_Data.strPathName = ar.ReadString();
		A3DVECTOR3 vr,vu,vl;
		if(!ar.Read(&vr,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vu,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vl,sizeof(A3DVECTOR3)))
			return false;
		
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_nCopy,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.dwID,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.dwRefurbish,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fRadius,sizeof(float)))
			return false;
		if(!ar.Read(&m_Data.dwNpcType,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.nSpeedFlags,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nPathID,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nLoopType,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.cNpcInfo,sizeof(char)))
			return false;
		if(!ar.Read(&m_Data.dwDisapearTime,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.bGenMounsterInit,sizeof(bool)))
			return false;
		if(!ar.Read(&cReviveType,sizeof(char)))
			return false;
		if(!ar.Read(&m_Data.bValidAtOneTime,sizeof(bool)))
			return false;
		if(!ar.Read(&m_Data.dwGenID,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.bPatrol,sizeof(bool)))
			return false;
		if(!ar.Read(&m_Data.nControllerId,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nMounsterLife,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nMounsterNum,sizeof(int)))
			return false;

		if(!ar.Read(&m_bExtraArea,sizeof(bool)))
			return false;
		int num = 0;
		if(!ar.Read(&num,sizeof(int))) return false;
		for(int i = 0; i < num; ++i)
		{
			int extraid; 
			if(!ar.Read(&extraid,sizeof(int)))
				return false;
			m_listExtraArea.push_back(extraid);
		}
		
		m_matRotate.SetRow(2,vr);
		m_matRotate.SetRow(1,vu);
		m_matRotate.SetRow(0,vl);
		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
		//if(iLoadFlags!= LOAD_EXPLIGHTMAP && iLoadFlags != LOAD_EXPSCENE) ReloadModel();
	}
	else
	{
		m_strName = ar.ReadString();
		m_Data.strNpcName = ar.ReadString();
		m_Data.strPathName = ar.ReadString();
		A3DVECTOR3 vr,vu,vl;
		if(!ar.Read(&vr,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vu,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vl,sizeof(A3DVECTOR3)))
			return false;

		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_nCopy,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.dwID,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.dwRefurbish,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.fRadius,sizeof(float)))
			return false;
		if(!ar.Read(&m_Data.dwNpcType,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.nSpeedFlags,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nPathID,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nLoopType,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.cNpcInfo,sizeof(char)))
			return false;
		if(!ar.Read(&m_Data.dwDisapearTime,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.bGenMounsterInit,sizeof(bool)))
			return false;
		if(!ar.Read(&cReviveType,sizeof(char)))
			return false;
		if(!ar.Read(&m_Data.bValidAtOneTime,sizeof(bool)))
			return false;
		if(!ar.Read(&m_Data.dwGenID,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.bPatrol,sizeof(bool)))
			return false;
		if(!ar.Read(&m_Data.sPhase,sizeof(short)))
			return false;
		if(!ar.Read(&m_Data.nControllerId,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nMounsterLife,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nMounsterNum,sizeof(int)))
			return false;

		if(!ar.Read(&m_bExtraArea,sizeof(bool)))
			return false;
		int num = 0;
		if(!ar.Read(&num,sizeof(int))) return false;
		for(int i = 0; i < num; ++i)
		{
			int extraid; 
			if(!ar.Read(&extraid,sizeof(int)))
				return false;
			m_listExtraArea.push_back(extraid);
		}

		m_matRotate.SetRow(2,vr);
		m_matRotate.SetRow(1,vu);
		m_matRotate.SetRow(0,vl);
		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
		//if(iLoadFlags!= LOAD_EXPLIGHTMAP && iLoadFlags != LOAD_EXPSCENE) ReloadModel();
	}

	m_Data.iReviveType = cReviveType;

	if(dwVersion < 0x33)
		m_Data.bPatrol = false;
	if (dwVersion <= 0x4f)
		m_Data.sPhase = 0;

	if(m_Data.dwGenID != 0 || !m_Data.bGenMounsterInit)
	{
		CString msg;
		msg.Format("注意：需要手动更新区域完成版本转换：%s(区域编号: %d,初始时生成怪物: %d)",GetObjectName(),m_Data.dwGenID, m_Data.bGenMounsterInit);
		g_LogDlg.Log(msg);
	}

	//if(m_bExtraArea) g_Log.Log("bb--------------------------------%d\r\n",m_nObjectID);

	if(m_Data.nControllerId == -1) m_Data.nControllerId = 0;

	return true;
}

//Save data
bool CSceneFixedNpcGenerator::Save(CELArchive& ar,DWORD dwVersion)
{
	if(dwVersion>8)
	{
		ar.WriteString(m_strName);
		ar.WriteString(m_Data.strNpcName);
		ar.WriteString(m_Data.strPathName);

		A3DVECTOR3 vr,vu,vl;
		vr = m_matRotate.GetRow(2);
		vu = m_matRotate.GetRow(1);
		vl = m_matRotate.GetRow(0);
		
		if(!ar.Write(&vr,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Write(&vu,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Write(&vl,sizeof(A3DVECTOR3)))
			return false;
		
		if(!ar.Write(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Write(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Write(&m_nCopy,sizeof(int)))
			return false;
		if(!ar.Write(&m_Data.dwID,sizeof(DWORD)))
			return false;
		if(!ar.Write(&m_Data.dwRefurbish,sizeof(DWORD)))
			return false;
		if(!ar.Write(&m_Data.fRadius,sizeof(float)))
			return false;
		if(!ar.Write(&m_Data.dwNpcType,sizeof(DWORD)))
			return false;
		if(!ar.Write(&m_Data.nSpeedFlags,sizeof(int)))
			return false;
		
		if(!ar.Write(&m_Data.nPathID,sizeof(int)))
			return false;
		if(!ar.Write(&m_Data.nLoopType,sizeof(int)))
			return false;
		if(!ar.Write(&m_Data.cNpcInfo,sizeof(char)))
			return false;
		if(!ar.Write(&m_Data.dwDisapearTime,sizeof(DWORD)))
			return false;
		if(!ar.Write(&m_Data.bGenMounsterInit,sizeof(bool)))
			return false;
		char cReviveType = m_Data.iReviveType;
		if(!ar.Write(&cReviveType,sizeof(char)))
			return false;
		if(!ar.Write(&m_Data.bValidAtOneTime,sizeof(bool)))
			return false;
		if(!ar.Write(&m_Data.dwGenID,sizeof(DWORD)))
			return false;
		if(!ar.Write(&m_Data.bPatrol,sizeof(bool)))
			return false;

		// 13.8.7 add
		if(!ar.Write(&m_Data.sPhase,sizeof(short)))
			return false;
		// 13.8.7 add end

		if(!ar.Write(&m_Data.nControllerId,sizeof(int)))
			return false;
		if(!ar.Write(&m_Data.nMounsterLife,sizeof(int)))
			return false;
		if(!ar.Write(&m_Data.nMounsterNum,sizeof(int)))
			return false;

		if(!ar.Write(&m_bExtraArea,sizeof(bool)))
			return false;
		int num = m_listExtraArea.size();
		if(!ar.Write(&num,sizeof(int))) return false;
		for(int i = 0; i < num; ++i)
		{
			int extraid = m_listExtraArea[i]; 
			if(!ar.Write(&extraid,sizeof(int)))
				return false;
		}
	}
	return true;
}

//Exp data
bool CSceneFixedNpcGenerator::ExpData(AFile *pFile,DWORD dwVersion)
{
	DWORD rs;
	pFile->WriteString(m_strName);
	pFile->WriteString(m_Data.strNpcName);
	
	if(!pFile->Write(&m_vPos,sizeof(A3DVECTOR3),&rs))
		return false;
	if(!pFile->Write(&m_Data.dwID,sizeof(DWORD),&rs))
		return false;
	if(!pFile->Write(&m_Data.dwRefurbish,sizeof(DWORD),&rs))
		return false;
	if(!pFile->Write(&m_Data.fRadius,sizeof(float),&rs))
		return false;
	if(!pFile->Write(&m_Data.dwNpcType,sizeof(DWORD),&rs))
		return false;
	return true;
}

bool CSceneFixedNpcGenerator::ImpData(AFile *pFile,DWORD dwVersion)
{
	DWORD rs;
	if(dwVersion<0xa) return false;
	
	if(dwVersion<0x15)
	{
		pFile->ReadString(m_strName);
		pFile->ReadString(m_Data.strNpcName);
		
		if(!pFile->Read(&m_vPos,sizeof(A3DVECTOR3),&rs))
			return false;
		if(!pFile->Read(&m_Data.dwID,sizeof(DWORD),&rs))
			return false;
		if(!pFile->Read(&m_Data.dwRefurbish,sizeof(DWORD),&rs))
			return false;
		if(!pFile->Read(&m_Data.fRadius,sizeof(float),&rs))
			return false;
	}else if(dwVersion>=0x15)
	{
		pFile->ReadString(m_strName);
		pFile->ReadString(m_Data.strNpcName);
		
		if(!pFile->Read(&m_vPos,sizeof(A3DVECTOR3),&rs))
			return false;
		if(!pFile->Read(&m_Data.dwID,sizeof(DWORD),&rs))
			return false;
		if(!pFile->Read(&m_Data.dwRefurbish,sizeof(DWORD),&rs))
			return false;
		if(!pFile->Read(&m_Data.fRadius,sizeof(float),&rs))
			return false;
		if(!pFile->Read(&m_Data.dwNpcType,sizeof(DWORD),&rs))
			return false;
	}
	
	return true;
}

void CSceneFixedNpcGenerator::Rotate(const A3DVECTOR3& axis,float fRad)
{
	A3DMATRIX4 mat;
	if(fabs(axis.x)<0.001f && fabs(axis.z)<0.001f && fabs(axis.y)>0.5f && fabs(axis.y)<1.001f)
	{
		mat.RotateAxis(axis,fRad);
		m_matRotate = m_matRotate*mat;
		m_vDirection = m_matRotate.GetRow(2);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
		if(m_pModel) m_pModel->GetA3DSkinModel()->SetDirAndUp(m_matRotate.GetRow(2),m_matRotate.GetRow(1));
	}
}

void CSceneFixedNpcGenerator::Translate(const A3DVECTOR3& vDelta)
{
	m_vPos += vDelta;
	m_matTrans.SetRow(3,m_vPos);
	m_matAbs = m_matScale * m_matRotate * m_matTrans;
	if(m_pModel) m_pModel->GetA3DSkinModel()->SetPos(m_vPos);
}

void CSceneFixedNpcGenerator::SetDirAndUp(const A3DVECTOR3& vDir, const A3DVECTOR3& vUp)
{
	if(m_pModel) m_pModel->SetDirAndUp(vDir,vUp);
	CSceneObject::SetDirAndUp(vDir,vUp);
}

bool CSceneFixedNpcGenerator::AABBCollideWithMe(BrushTraceInfo * pInfo)
{
	if (!GetECModel())
	{
		return false;
	}

	
	assert(!pInfo->bIsPoint);
	A3DAABB aabb = GetECModel()->GetModelAABB(); 
	aabb.Extents+=A3DVECTOR3(8.0f, 8.0f, 8.0f);
	aabb.CompleteMinsMaxs();

	A3DVECTOR3 vPoint, vNormal;
	float		fFraction;		//	Fraction
	
	
	if (!CLS_AABBAABBOverlap(aabb.Center, aabb.Extents, pInfo->BoundAABB.Center, pInfo->BoundAABB.Extents) ) 
	{
		return false;
	}
	//save original result
	bool		bStartSolid = pInfo->bStartSolid;	//	Collide something at start point
	bool		bAllSolid = pInfo->bAllSolid;		//	All in something
	int			iClipPlane = pInfo->iClipPlane;		//	Clip plane's index
	
	fFraction = 100.0f;
	vNormal = pInfo->ClipPlane.GetNormal(); //clip plane normal
	float       fDist = pInfo->ClipPlane.GetDist();	//clip plane dist

	//create brush on the fly
	A3DOBB           obb;
	obb.Build(GetECModel()->GetModelAABB());
	CConvexHullData  convexhull;
	convexhull.Import(&obb);
	CQBrush     qBrush;
	qBrush.AddBrushBevels(&convexhull);
	
	// Detect the collision with Convex Hull
	bool bCollide = false;

#ifndef _ANGELICA22
	if (qBrush.Trace(pInfo) && (pInfo->fFraction < fFraction))
	{
		//update the saving info
		bStartSolid = pInfo->bStartSolid;
		bAllSolid = pInfo->bAllSolid;
		iClipPlane = pInfo->iClipPlane;
		fFraction = pInfo->fFraction;
		vNormal = pInfo->ClipPlane.GetNormal();
		fDist = pInfo->ClipPlane.GetDist();
		bCollide = true;
	}
#endif
	if (bCollide) 
	{
		pInfo->bStartSolid = bStartSolid;
		pInfo->bAllSolid = bAllSolid;
		pInfo->iClipPlane = iClipPlane;
		pInfo->fFraction = fFraction;
		pInfo->ClipPlane.SetNormal(vNormal);
		pInfo->ClipPlane.SetD(fDist);
	}
	
	return bCollide;
}






