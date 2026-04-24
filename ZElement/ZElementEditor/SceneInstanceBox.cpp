// SceneInstanceBox.cpp: implementation of the SceneInstanceBox class.
//
//////////////////////////////////////////////////////////////////////

#include "global.h"
#include "elementeditor.h"
#include "SceneInstanceBox.h"
#include "Render.h"
#include "Box3D.h"
#include "A3D.h"
#include "MainFrm.h"
#include "SceneObjectManager.h"
#include "NpcControllerSelDlg.h"
#include "NpcControllerManDlg.h"

//#define new A_DEBUG_NEW

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void init_instance_data( INSTANCE_DATA &dat)
{
	dat.nWorldId = 1;
	dat.nSelfWorldId = 1;
	dat.vTargetPos = A3DVECTOR3(0,0,0);
	dat.fEdgeHei = 20;
	dat.fEdgeLen = 20;
	dat.fEdgeWth = 20; 
	dat.nLimitLevel = 0;
	dat.nLine = 0;
	dat.nControllerId = 0;
}


SceneInstanceBox::SceneInstanceBox()
{
	m_nObjectType = SO_TYPE_INSTANCE_BOX;
	m_pControllerSelFuncs = new CONTROLLER_INSTANCE_ID_FUNCS;
	init_instance_data(m_Data);
	m_nCopy = 0;
	BuildProperty();
}

SceneInstanceBox::~SceneInstanceBox()
{

}

void SceneInstanceBox::BuildProperty()
{
	if(m_pProperty)
	{
		m_pProperty->DynAddProperty(AVariant(m_strName), "名字");
		m_pProperty->DynAddProperty(AVariant(m_vPos), "位置坐标");
		m_pProperty->DynAddProperty(AVariant(m_Data.fEdgeHei), "盒子高(y)");
		m_pProperty->DynAddProperty(AVariant(m_Data.fEdgeLen),"盒子长(z)");
		m_pProperty->DynAddProperty(AVariant(m_Data.fEdgeWth), "盒子宽(x)");
		m_pProperty->DynAddProperty(AVariant(m_Data.nWorldId), "目标世界ID");
		m_pProperty->DynAddProperty(AVariant(m_Data.vTargetPos), "目标位置");
		m_pProperty->DynAddProperty(AVariant(m_Data.nSelfWorldId), "自己的世界ID");
		m_pProperty->DynAddProperty(AVariant(m_Data.nLimitLevel), "等级限制");
		m_pProperty->DynAddProperty(AVariant(m_Data.nLine), "传送线");
		m_pProperty->DynAddProperty(AVariant(m_Data.nControllerId), "控制器", (ASet*)m_pControllerSelFuncs, NULL, WAY_CUSTOM);
		m_pProperty->DynAddProperty(AVariant(m_nCopy), "第几套数据");
		m_pProperty->DynAddProperty(AVariant(m_nObjectID), "场景对象ID（海外版专用）", NULL,NULL,WAY_READONLY);
	}
}

void SceneInstanceBox::Render(A3DViewport* pA3DViewport)
{
	CSceneObject::Render(pA3DViewport);
}

void SceneInstanceBox::PostRender(A3DViewport* pA3DViewport)
{

	if(m_nCopy != g_Configs.nShowCopyNum) return;
	CSceneObject::DrawCenter();
	CBox3D renderBox;
	CBox3D renderWire;
	renderBox.Init(&g_Render,1.0f,true);
	renderWire.Init(&g_Render,1.0f,false);
	renderBox.SetSize(m_Data.fEdgeWth/2.0f,m_Data.fEdgeHei/2.0f,m_Data.fEdgeLen/2.0f);
	renderWire.SetSize(m_Data.fEdgeWth/2.0f,m_Data.fEdgeHei/2.0f,m_Data.fEdgeLen/2.0f);
	renderBox.SetTM(m_matTrans);
	renderWire.SetTM(m_matTrans);
	renderWire.SetColor(A3DCOLORRGBA(0,255,0,255));
	if(!m_bSelected) renderBox.SetColor(A3DCOLORRGBA(0,0,255,128));
	else renderBox.SetColor(A3DCOLORRGBA(255,0,0,128));
	renderBox.Render();
	renderWire.Render();	
	CSceneObject::PostRender(pA3DViewport);
}



void SceneInstanceBox::UpdateProperty(bool bGet)
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
				MessageBox(NULL,"该对象的名字已经存在，编辑器将恢复原对象名字！",NULL,MB_OK);
				m_pProperty->SetPropVal(0,m_strName);
				m_pProperty->SetPropVal(1,m_vPos);
				m_pProperty->SetPropVal(2,m_Data.fEdgeHei);
				m_pProperty->SetPropVal(3,m_Data.fEdgeLen);
				m_pProperty->SetPropVal(4,m_Data.fEdgeWth);
				m_pProperty->SetPropVal(5,m_Data.nWorldId);
				m_pProperty->SetPropVal(6,m_Data.vTargetPos);
				m_pProperty->SetPropVal(7,m_Data.nSelfWorldId);
				m_pProperty->SetPropVal(8,m_Data.nLimitLevel);
				m_pProperty->SetPropVal(9,m_Data.nLine);
				m_pProperty->SetPropVal(11,m_nCopy);
				m_pControllerSelFuncs->OnSetValue(m_Data.nControllerId);
				m_pProperty->SetPropVal(12,m_nObjectID);
				AUX_GetMainFrame()->GetToolTabWnd()->UpdatePropertyData(false);
				return;
			}
		}
		m_strName				= temp;
		m_vPos					= m_pProperty->GetPropVal(1);
		m_Data.fEdgeHei		= m_pProperty->GetPropVal(2);
		m_Data.fEdgeLen		= m_pProperty->GetPropVal(3);
		m_Data.fEdgeWth		= m_pProperty->GetPropVal(4);
		m_Data.nWorldId	    = m_pProperty->GetPropVal(5);
		m_Data.vTargetPos   = m_pProperty->GetPropVal(6);
		m_Data.nSelfWorldId = m_pProperty->GetPropVal(7);
		m_Data.nLimitLevel  = m_pProperty->GetPropVal(8);
		m_Data.nLine		= m_pProperty->GetPropVal(9);
		m_nCopy             = m_pProperty->GetPropVal(11);
		m_matTrans.Translate(m_vPos.x,m_vPos.y,m_vPos.z);
		SetPos(m_vPos);
	}else
	{
	
		m_pProperty->SetPropVal(0,m_strName);
		m_pProperty->SetPropVal(1,m_vPos);
		m_pProperty->SetPropVal(2,m_Data.fEdgeHei);
		m_pProperty->SetPropVal(3,m_Data.fEdgeLen);
		m_pProperty->SetPropVal(4,m_Data.fEdgeWth);
		m_pProperty->SetPropVal(5,m_Data.nWorldId);
		m_pProperty->SetPropVal(6,m_Data.vTargetPos);
		m_pProperty->SetPropVal(7,m_Data.nSelfWorldId);
		m_pProperty->SetPropVal(8,m_Data.nLimitLevel);
		m_pProperty->SetPropVal(9,m_Data.nLine);
		m_pProperty->SetPropVal(11,m_nCopy);

		m_pControllerSelFuncs->OnSetValue(m_Data.nControllerId);

		m_pProperty->SetPropVal(12,m_nObjectID);
	}
}

void SceneInstanceBox::Release()
{
	if(m_pControllerSelFuncs)
		delete m_pControllerSelFuncs;
	
}

//  Copy object
CSceneObject* SceneInstanceBox::CopyObject()
{
	SceneInstanceBox *pNewObject = new SceneInstanceBox();
	ASSERT(pNewObject);
	int Num = 0;
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		AString name;
		CSceneObjectManager *pSManager = pMap->GetSceneObjectMan();
		pSManager->ClearSelectedList();
		while(1)
		{
			name.Format("InstanceBox_%d",Num);
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
		pNewObject->SetPos(m_vPos);
		pNewObject->SetCopyFlags(GetCopyFlags());
		pNewObject->SetProperty(GetProperty());
		
		pNewObject->UpdateProperty(false);
		AUX_GetMainFrame()->GetToolTabWnd()->ShowPropertyPannel(pNewObject->m_pProperty);
		AUX_GetMainFrame()->GetToolTabWnd()->UpdatePropertyData(false);
	}
	return pNewObject;
}
	//	Load data
bool SceneInstanceBox::Load(CELArchive& ar,DWORD dwVersion,int iLoadFlags)
{
	init_instance_data(m_Data);
	if(dwVersion>0x29 && dwVersion<=0x2b)
	{
		m_strName = ar.ReadString();
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.vTargetPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.fEdgeHei,sizeof(float)))
			return false;
		if(!ar.Read(&m_Data.fEdgeLen,sizeof(float)))
			return false;
		if(!ar.Read(&m_Data.fEdgeWth,sizeof(float)))
			return false;
		if(!ar.Read(&m_Data.nWorldId,sizeof(int)))
			return false;
	}else if(dwVersion > 0x2b && dwVersion <=0x35)
	{
		m_strName = ar.ReadString();
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.vTargetPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.fEdgeHei,sizeof(float)))
			return false;
		if(!ar.Read(&m_Data.fEdgeLen,sizeof(float)))
			return false;
		if(!ar.Read(&m_Data.fEdgeWth,sizeof(float)))
			return false;
		if(!ar.Read(&m_Data.nWorldId,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nSelfWorldId,sizeof(int)))
			return false;
	}else if(dwVersion > 0x35 && dwVersion < 0x40)
	{
		m_strName = ar.ReadString();
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.vTargetPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.fEdgeHei,sizeof(float)))
			return false;
		if(!ar.Read(&m_Data.fEdgeLen,sizeof(float)))
			return false;
		if(!ar.Read(&m_Data.fEdgeWth,sizeof(float)))
			return false;
		if(!ar.Read(&m_Data.nWorldId,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nSelfWorldId,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nLimitLevel,sizeof(int)))
			return false;
	}else if(dwVersion >= 0x40 && dwVersion < 0x44)
	{
		m_strName = ar.ReadString();
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.vTargetPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.fEdgeHei,sizeof(float)))
			return false;
		if(!ar.Read(&m_Data.fEdgeLen,sizeof(float)))
			return false;
		if(!ar.Read(&m_Data.fEdgeWth,sizeof(float)))
			return false;
		if(!ar.Read(&m_Data.nWorldId,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nSelfWorldId,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nLimitLevel,sizeof(int)))
			return false;
		if(!ar.Read(&m_nCopy,sizeof(int)))
			return false;
	}else if(dwVersion >= 0x44 && dwVersion < 0x47)
	{
		m_strName = ar.ReadString();
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.vTargetPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.fEdgeHei,sizeof(float)))
			return false;
		if(!ar.Read(&m_Data.fEdgeLen,sizeof(float)))
			return false;
		if(!ar.Read(&m_Data.fEdgeWth,sizeof(float)))
			return false;
		if(!ar.Read(&m_Data.nWorldId,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nSelfWorldId,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nLimitLevel,sizeof(int)))
			return false;
		if(!ar.Read(&m_nCopy,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nLine,sizeof(int)))
			return false;
	}else
	{
		m_strName = ar.ReadString();
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.vTargetPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.fEdgeHei,sizeof(float)))
			return false;
		if(!ar.Read(&m_Data.fEdgeLen,sizeof(float)))
			return false;
		if(!ar.Read(&m_Data.fEdgeWth,sizeof(float)))
			return false;
		if(!ar.Read(&m_Data.nWorldId,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nSelfWorldId,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nLimitLevel,sizeof(int)))
			return false;
		if(!ar.Read(&m_nCopy,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nLine,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nControllerId,sizeof(int)))
			return false;
	}

	if(dwVersion <= 0x36) 
	{//修复原来默认是150级的错误
		if(m_Data.nLimitLevel == 150) m_Data.nLimitLevel = 0;
	}

	if(m_Data.nSelfWorldId < 1 || m_Data.nSelfWorldId >= 10000)  m_Data.nSelfWorldId = 1;
	if(m_Data.nWorldId < 1 || m_Data.nWorldId > 10000 ) m_Data.nWorldId = 1;
	m_matTrans.Translate(m_vPos.x,m_vPos.y,m_vPos.z);
	return true;
}
//	Save data
bool SceneInstanceBox::Save(CELArchive& ar,DWORD dwVersion)
{
	if(dwVersion<=0x29) return true;
	ar.WriteString(m_strName);
	if(!ar.Write(&m_vPos,sizeof(A3DVECTOR3)))
		return false;
	if(!ar.Write(&m_Data.vTargetPos,sizeof(A3DVECTOR3)))
		return false;
	if(!ar.Write(&m_Data.fEdgeHei,sizeof(float)))
		return false;
	if(!ar.Write(&m_Data.fEdgeLen,sizeof(float)))
		return false;
	if(!ar.Write(&m_Data.fEdgeWth,sizeof(float)))
		return false;
	if(!ar.Write(&m_Data.nWorldId,sizeof(int)))
		return false;
	if(!ar.Write(&m_Data.nSelfWorldId,sizeof(int)))
		return false;
	if(!ar.Write(&m_Data.nLimitLevel,sizeof(int)))
		return false;
	if(!ar.Write(&m_nCopy,sizeof(int)))
		return false;
	if(!ar.Write(&m_Data.nLine,sizeof(int)))
		return false;
	if(!ar.Write(&m_Data.nControllerId,sizeof(int)))
		return false;
	return true;
}

void SceneInstanceBox::GetAABB(A3DVECTOR3 &vMin, A3DVECTOR3 &vMax)
{
	vMin = A3DVECTOR3(m_vPos.x - m_Data.fEdgeWth/2.0f, m_vPos.y - m_Data.fEdgeHei/2.0f, m_vPos.z - m_Data.fEdgeLen/2.0f);
	vMax = A3DVECTOR3(m_vPos.x + m_Data.fEdgeWth/2.0f, m_vPos.y + m_Data.fEdgeHei/2.0f, m_vPos.z + m_Data.fEdgeLen/2.0f);	
}
	
bool SceneInstanceBox::IsIntersectionWithArea(const A3DVECTOR3& vMin, const A3DVECTOR3& vMax)
{
	A3DVECTOR3 m_vMin,m_vMax;
	GetAABB(m_vMin,m_vMax);
	if(m_vMin.x>vMax.x) return false;
	if(m_vMin.z>vMax.z) return false;
	if(m_vMax.x<vMin.x) return false;
	if(m_vMax.z<vMin.z) return false;
	return true;
}


//Controller select function
BOOL CALLBACK CONTROLLER_INSTANCE_ID_FUNCS::OnActivate(void)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(pManager->m_listSelectedObject.GetCount()==1)
		{
			CSceneObject *pObj = (CSceneObject*)pManager->m_listSelectedObject.GetHead();
			if(pObj->GetObjectType()==CSceneObject::SO_TYPE_INSTANCE_BOX)
			{
				CNpcContollerSelDlg dlg;
				INSTANCE_DATA data = ((SceneInstanceBox*)pObj)->GetProperty();
				dlg.m_nDyObjID = data.nControllerId;
				if(dlg.DoModal()==IDOK)
				{
					data.nControllerId = dlg.m_nDyObjID;
					((SceneInstanceBox*)pObj)->SetProperty(data);
					CONTROLLER_OBJECT obj;
					InitControllerObj(&obj);
					g_ControllerObjectMan.GetObjByID(data.nControllerId,obj);
					if(obj.id==0) m_strText = "--None--";
					else m_strText.Format("[%d]%s",obj.id,obj.szName);
				}
			}
		}
	}
	
	return TRUE;
}

LPCTSTR CALLBACK CONTROLLER_INSTANCE_ID_FUNCS::OnGetShowString(void) const
{
	return m_strText;
}

AVariant CALLBACK CONTROLLER_INSTANCE_ID_FUNCS::OnGetValue(void) const
{
	return m_var;
}

void CALLBACK CONTROLLER_INSTANCE_ID_FUNCS::OnSetValue(const AVariant& var)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(pManager->m_listSelectedObject.GetCount()==1)
		{
			CSceneObject *pObj = (CSceneObject*)pManager->m_listSelectedObject.GetHead();
			if(pObj->GetObjectType()==CSceneObject::SO_TYPE_INSTANCE_BOX)
			{
				CNpcContollerSelDlg dlg;
				INSTANCE_DATA data = ((SceneInstanceBox*)pObj)->GetProperty();
				CONTROLLER_OBJECT obj;
				InitControllerObj(&obj);
				g_ControllerObjectMan.GetObjByID(data.nControllerId,obj);
				if(obj.id!=0) m_strText.Format("[%d]%s",obj.id,obj.szName);
				else m_strText = "--None--";
			}
		}
	}
}


