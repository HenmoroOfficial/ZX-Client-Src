#include "global.h"
#include "elementeditor.h"
#include "SceneDynamicObject.h"
#include "SceneObjectManager.h"
#include "DynamicObjectSelDlg.h"
#include "MainFrm.h"
#include "DynamicObjectManDlg.h"
#include "ElementMap.h"
#include "SceneObjectManager.h"
#include "EC_Model.h"
#include "Box3D.h"
#include "NpcControllerSelDlg.h"
#include "NpcControllerManDlg.h"
#include <A3DGFXEx.h>

#include "render.h"
#include <a3d.h>
#include <A3DGFXExMan.h>


CSceneDynamicObject::CSceneDynamicObject()
{
	m_uDynamicObjectID = 0;
	m_uControllerID = 0;
	m_iPhase = 0;
	m_pIDSelFuncs = new DYNAMIC_SEL_FUNCS;
	m_pControllerSelFuncs = new CONTROLLER_DYNAMIC_ID_FUNCS;
	m_nObjectType = SO_TYPE_DYNAMIC;
	BuildProperty();
	m_pGfxEx = 0;
	m_pECModel = 0;
	m_nCopy = 0;
}

CSceneDynamicObject::~CSceneDynamicObject()
{
}

void CSceneDynamicObject::ReInit()
{
	CSceneObject::ReInit();
	m_pIDSelFuncs = new DYNAMIC_SEL_FUNCS;
	m_pControllerSelFuncs = new CONTROLLER_DYNAMIC_ID_FUNCS;
}


void CSceneDynamicObject::Release()
{
	if(m_pIDSelFuncs) delete m_pIDSelFuncs;
	if(m_pControllerSelFuncs) delete m_pControllerSelFuncs;
	
	if(m_pGfxEx)
	{
		m_pGfxEx->Stop();
		m_pGfxEx->Release();
		delete m_pGfxEx;
		m_pGfxEx = NULL;
	}

	if(m_pECModel)
	{
		m_pECModel->Release();
		delete m_pECModel;
		m_pECModel = NULL;
	}
	
	CSceneObject::Release();	
}


void CSceneDynamicObject::BuildProperty()
{
	if(m_pProperty)
	{
		m_pProperty->DynAddProperty(AVariant(m_strName), "名字");
		m_pProperty->DynAddProperty(AVariant(m_vPos), "位置坐标");
		m_pProperty->DynAddProperty(AVariant(""), "动态对象", (ASet*)m_pIDSelFuncs, NULL, WAY_CUSTOM);
		m_pProperty->DynAddProperty(AVariant(""), "关联怪物控制器", (ASet*)m_pControllerSelFuncs, NULL, WAY_CUSTOM);
		m_pProperty->DynAddProperty(AVariant(m_nCopy), "第几套数据");
		m_pProperty->DynAddProperty(AVariant(m_nObjectID), "场景对象ID（海外版专用）", NULL,NULL,WAY_READONLY);
		m_pProperty->DynAddProperty(AVariant(m_iPhase), "相位(1-200)");
	}
}

void CSceneDynamicObject::Render(A3DViewport* pA3DViewport)
{
	if(m_nCopy != g_Configs.nShowCopyNum || !g_Configs.bShowDynamic) return;	
	if(m_pECModel)
	{
		CElementMap *pMap = AUX_GetMainFrame()->GetMap();
		LIGHTINGPARAMS light = pMap->GetLightingParams();
		A3DLIGHTPARAM lightParams = g_Render.GetDirLight()->GetLightparam();
		A3DSkinModel::LIGHTINFO LightInfo;
		LightInfo.colAmbient	= g_Render.GetA3DDevice()->GetAmbientColor();
		LightInfo.vLightDir		= lightParams.Direction;//Direction of directional light
		LightInfo.colDirDiff	= lightParams.Diffuse;//Directional light's diffuse color
		LightInfo.colDirSpec	= lightParams.Specular;//	Directional light's specular color
		LightInfo.bPtLight		= false;//	false, disable dynamic point light,
		m_pECModel->GetA3DSkinModel()->SetLightInfo(LightInfo);
		m_pECModel->Render(pA3DViewport);
	}
	
	CSceneObject::Render(pA3DViewport);
}


void CSceneDynamicObject::PostRender(A3DViewport* pA3DViewport)
{
	if(m_nCopy != g_Configs.nShowCopyNum || !g_Configs.bShowDynamic) return;
	DrawCenter();

	if(m_pECModel)
	{
		CBox3D renderBox;
		renderBox.Init(&g_Render,1.0f,false);
		A3DAABB aabb = m_pECModel->GetA3DSkinModel()->GetModelAABB();
		renderBox.SetSize(aabb.Extents.x,aabb.Extents.y,aabb.Extents.z);
		renderBox.SetPos(aabb.Center);
		renderBox.SetColor(A3DCOLORRGB(255,0,0));
		if(m_bSelected) renderBox.Render();
	}

	CSceneObject::PostRender(pA3DViewport);
}

void CSceneDynamicObject::Tick(DWORD timeDelta)
{
	if(m_nCopy != g_Configs.nShowCopyNum) return;	// add by ice, 2011/05/09

	if(m_pECModel) m_pECModel->Tick(timeDelta);
	if(m_pGfxEx)
	{	
		float scale = m_matScale._11;
		m_pGfxEx->SetScale(scale);
		m_pGfxEx->SetParentTM(m_matRotate * m_matTrans);
		m_pGfxEx->TickAnimation(timeDelta);
		AfxGetGFXExMan()->RegisterGfx(m_pGfxEx);
	}
}

void CSceneDynamicObject::UpdateProperty(bool bGet)
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
				m_pProperty->SetPropVal(2,m_uDynamicObjectID);
				m_pProperty->SetPropVal(4,m_nCopy);
				m_pIDSelFuncs->OnSetValue("");
				m_pControllerSelFuncs->OnSetValue("");
				m_pProperty->SetPropVal(5,m_nObjectID);
				m_pProperty->SetPropVal(6,m_iPhase);
				AUX_GetMainFrame()->GetToolTabWnd()->UpdatePropertyData(false);
				return;
			}
		}
		m_strName	        = temp;
		m_vPos	            = m_pProperty->GetPropVal(1);
		m_nCopy             = m_pProperty->GetPropVal(4);
		//m_uDynamicObjectID	= m_pProperty->GetPropVal(2);
		m_iPhase			= m_pProperty->GetPropVal(6);
	
		ReloadModels();
	}else
	{
		m_pProperty->SetPropVal(0,m_strName);
		m_pProperty->SetPropVal(1,m_vPos);
		m_pProperty->SetPropVal(2,m_uDynamicObjectID);
		m_pProperty->SetPropVal(4,m_nCopy);
		m_pIDSelFuncs->OnSetValue("");
		m_pControllerSelFuncs->OnSetValue("");
		m_pProperty->SetPropVal(5,m_nObjectID);
		m_pProperty->SetPropVal(6,m_iPhase);
	}
}

void  CSceneDynamicObject::ReloadModels()
{
	//Realse all old objects
	if(m_pGfxEx)
	{
		m_pGfxEx->Stop();
		m_pGfxEx->Release();
		delete m_pGfxEx;
		m_pGfxEx = NULL;
	}

	if(m_pECModel)
	{
		m_pECModel->Release();
		delete m_pECModel;
		m_pECModel = NULL;
	}
	
	
	DYNAMIC_OBJECT obj;
	InitDynamicObj(&obj);
	g_DynamicObjectMan.GetObjByID(m_uDynamicObjectID,obj);
	if(obj.id == 0) return;
	CString strPath = obj.szObjectPath;
	int pos = strPath.ReverseFind('.');
	if(pos == -1) return;
	strPath = strPath.Right(strPath.GetLength() - pos);
	strPath.MakeLower();
	if(strPath == ".gfx")
	{
		m_pGfxEx = AfxGetGFXExMan()->LoadGfx(g_Render.GetA3DDevice(),obj.szObjectPath);
		if(m_pGfxEx==NULL)
		{
			AString msg;
			msg.Format("CSceneDynamicObject::ReloadModels(), Load %s file failed",obj.szObjectPath);
			g_Log.Log(msg);
			return;
		}
		m_pGfxEx->Start(true);
	}else if(strPath == ".ecm")
	{
		m_pECModel = new CECModel;
		bool bResult = m_pECModel->Load(obj.szObjectPath);
		if(!bResult)
		{
			g_Log.Log("CSceneDynamicObject::ReloadModels(), Failed load %s", obj.szObjectPath);
			delete m_pECModel;
			m_pECModel = NULL;
			return;
		}
		m_pECModel->GetA3DSkinModel()->SetPos(m_vPos);
		m_pECModel->GetA3DSkinModel()->SetDirAndUp(m_matRotate.GetRow(2),m_matRotate.GetRow(1));
		m_pECModel->GetA3DSkinModel()->SetAutoAABBType(A3DSkinModel::AUTOAABB_INITMESH);
	}
}

//Load data
bool CSceneDynamicObject::Load(CELArchive& ar,DWORD dwVersion,int iLoadFlags)
{
	
	if(dwVersion > 0x38 && dwVersion <=0x3d)
	{
		A3DVECTOR3 vr,vu,vl,vs;
		m_strName = AString(ar.ReadString());
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vr,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vu,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vl,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vs,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_uDynamicObjectID,sizeof(unsigned int)))
		return false;
		m_matTrans.SetRow(3,m_vPos);
		m_matRotate.SetRow(2,vr);
		m_matRotate.SetRow(1,vu);
		m_matRotate.SetRow(0,vl);
		m_matScale._11 = vs.x;
		m_matScale._22 = vs.y;
		m_matScale._33 = vs.z;
		m_matAbs = m_matScale * m_matRotate * m_matTrans;	

		ReloadModels();
	}else if(dwVersion > 0x3d && dwVersion < 0x40)
	{
		A3DVECTOR3 vr,vu,vl,vs;
		m_strName = AString(ar.ReadString());
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vr,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vu,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vl,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vs,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_uDynamicObjectID,sizeof(unsigned int)))
			return false;
		if(!ar.Read(&m_uControllerID,sizeof(unsigned int)))
			return false;
		
		m_matTrans.SetRow(3,m_vPos);
		m_matRotate.SetRow(2,vr);
		m_matRotate.SetRow(1,vu);
		m_matRotate.SetRow(0,vl);
		m_matScale._11 = vs.x;
		m_matScale._22 = vs.y;
		m_matScale._33 = vs.z;
		m_matAbs = m_matScale * m_matRotate * m_matTrans;	

		ReloadModels();	
	}else if(dwVersion >= 0x40 && dwVersion < 0x51)
	{
		A3DVECTOR3 vr,vu,vl,vs;
		m_strName = AString(ar.ReadString());
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vr,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vu,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vl,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vs,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_uDynamicObjectID,sizeof(unsigned int)))
			return false;
		if(!ar.Read(&m_uControllerID,sizeof(unsigned int)))
			return false;
		if(!ar.Read(&m_nCopy,sizeof(int)))
			return false;
		
		m_matTrans.SetRow(3,m_vPos);
		m_matRotate.SetRow(2,vr);
		m_matRotate.SetRow(1,vu);
		m_matRotate.SetRow(0,vl);
		m_matScale._11 = vs.x;
		m_matScale._22 = vs.y;
		m_matScale._33 = vs.z;
		m_matAbs = m_matScale * m_matRotate * m_matTrans;	

		ReloadModels();	
	}else if(dwVersion >= 0x51)
	{
		A3DVECTOR3 vr,vu,vl,vs;
		m_strName = AString(ar.ReadString());
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vr,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vu,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vl,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&vs,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_uDynamicObjectID,sizeof(unsigned int)))
			return false;
		if(!ar.Read(&m_uControllerID,sizeof(unsigned int)))
			return false;
		if(!ar.Read(&m_nCopy,sizeof(int)))
			return false;
		if(!ar.Read(&m_iPhase,sizeof(int)))
			return false;

		m_matTrans.SetRow(3,m_vPos);
		m_matRotate.SetRow(2,vr);
		m_matRotate.SetRow(1,vu);
		m_matRotate.SetRow(0,vl);
		m_matScale._11 = vs.x;
		m_matScale._22 = vs.y;
		m_matScale._33 = vs.z;
		m_matAbs = m_matScale * m_matRotate * m_matTrans;	

		ReloadModels();	
	}
	return true;
}
	//Save data
bool CSceneDynamicObject::Save(CELArchive& ar,DWORD dwVersion)
{

	m_matRotate = GetRotateMatrix();
	A3DVECTOR3 vr,vu,vl,vs;
	vr = m_matRotate.GetRow(2);
	vu = m_matRotate.GetRow(1);
	vl = m_matRotate.GetRow(0);
	vs = A3DVECTOR3(m_matScale._11,m_matScale._22,m_matScale._33);
	
	ar.WriteString(m_strName);
	if(!ar.Write(&m_nObjectID,sizeof(int)))
		return false;
	if(!ar.Write(&m_vPos,sizeof(A3DVECTOR3)))
		return false;
	if(!ar.Write(&vr,sizeof(A3DVECTOR3)))
		return false;
	if(!ar.Write(&vu,sizeof(A3DVECTOR3)))
		return false;
	if(!ar.Write(&vl,sizeof(A3DVECTOR3)))
		return false;
	if(!ar.Write(&vs,sizeof(A3DVECTOR3)))
		return false;
	if(!ar.Write(&m_uDynamicObjectID,sizeof(unsigned int)))
		return false;
	if(!ar.Write(&m_uControllerID,sizeof(unsigned int)))
		return false;
	if(!ar.Write(&m_nCopy,sizeof(int)))
		return false;
	if(!ar.Write(&m_iPhase,sizeof(int)))
		return false;

	return true;
}

void CSceneDynamicObject::SetDirAndUp(const A3DVECTOR3& vDir, const A3DVECTOR3& vUp)
{
	if(m_pECModel) m_pECModel->SetDirAndUp(vDir,vUp);
	CSceneObject::SetDirAndUp(vDir,vUp);
}

void CSceneDynamicObject::Translate(A3DVECTOR3 vDelta)
{
	if(m_pECModel)
	{
		m_vPos +=vDelta;
		m_pECModel->GetA3DSkinModel()->SetPos(m_vPos);
	}
	CSceneObject::Translate(vDelta);
}

void CSceneDynamicObject::RotateX(float fRad)
{
	if(m_pECModel) m_pECModel->GetA3DSkinModel()->RotateX(fRad);
	CSceneObject::RotateX(fRad);	
}

void CSceneDynamicObject::RotateY(float fRad)
{
	if(m_pECModel) m_pECModel->GetA3DSkinModel()->RotateY(fRad);
	CSceneObject::RotateY(fRad);	
}

void CSceneDynamicObject::RotateZ(float fRad)
{
	if(m_pECModel) m_pECModel->GetA3DSkinModel()->RotateZ(fRad);
	CSceneObject::RotateZ(fRad);
}

void CSceneDynamicObject::Rotate(A3DVECTOR3 axis,float fRad)
{
	if(m_pECModel)
	{
		A3DMATRIX4 mat;
		mat.Identity();
		mat.RotateAxis(axis,fRad);
		
		A3DVECTOR3 vDir = m_pECModel->GetA3DSkinModel()->GetDir();
		A3DVECTOR3 vUp = m_pECModel->GetA3DSkinModel()->GetUp();
		vDir = mat * vDir;
		vUp = mat * vUp;
		m_pECModel->GetA3DSkinModel()->SetDirAndUp(vDir,vUp);
	}
	
	CSceneObject::Rotate(axis,fRad);
}

void CSceneDynamicObject::ResetRotateMat()
{
	if(m_pECModel) m_pECModel->GetA3DSkinModel()->SetDirAndUp(A3DVECTOR3(0,0,1.0f),A3DVECTOR3(0,1.0f,0));
	m_matRotate.Identity();
}

void CSceneDynamicObject::Scale(float fScale)
{
	///A3DVECTOR3 vScale = m_pECModel->GetScale();
	//vScale *= fScale; 
	//m_pECModel->SetScale(vScale.x,vScale.y,vScale.z);
	CSceneObject::Scale(fScale);
}

void CSceneDynamicObject::SetPos(A3DVECTOR3 vPos)
{
	if(m_pECModel) m_pECModel->SetPos(vPos);
	CSceneObject::SetPos(vPos);
}

CSceneObject* CSceneDynamicObject::CopyObject()
{
	CSceneDynamicObject *pNewObject = new CSceneDynamicObject(*this);
	ASSERT(pNewObject);
	pNewObject->ReInit();
	pNewObject->SetAllPrtToNull();
	pNewObject->m_pProperty = new ADynPropertyObject();	
	pNewObject->SetPropertyPtr(new DYNAMIC_SEL_FUNCS);
	pNewObject->BuildProperty();
	pNewObject->ReloadModels();
	int Num = 0;
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		AString name;
		CSceneObjectManager *pSManager = pMap->GetSceneObjectMan();
		pSManager->ClearSelectedList();
		while(1)
		{
			name.Format("Dynamic_%d",Num);
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
		
		pNewObject->UpdateProperty(false);
		AUX_GetMainFrame()->GetToolTabWnd()->ShowPropertyPannel(pNewObject->m_pProperty);
		AUX_GetMainFrame()->GetToolTabWnd()->UpdatePropertyData(false);
	}
	return pNewObject;
}

BOOL CALLBACK DYNAMIC_SEL_FUNCS::OnActivate(void)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
	CSceneObject *pObj = 0;
	if(pManager->m_listSelectedObject.GetCount()==1)
	{
		pObj = pManager->m_listSelectedObject.GetHead();
		if(pObj->GetObjectType()==CSceneObject::SO_TYPE_DYNAMIC)
		{
			CSceneDynamicObject *pObj1 = (CSceneDynamicObject*)pObj;
			CDynamicObjectSelDlg dlg;
			dlg.m_nDyObjID = pObj1->GetDynamicObjectID();
			if(IDOK==dlg.DoModal())
			{
				pObj1->SetDynamicObjectID(dlg.m_nDyObjID);
				m_var = dlg.m_nDyObjID;
				DYNAMIC_OBJECT obj;
				InitDynamicObj(&obj);
				g_DynamicObjectMan.GetObjByID(dlg.m_nDyObjID,obj);
				m_strText.Format("[%d]%s",obj.id,obj.szName);
			}
		}
	}
	return TRUE;
}

LPCTSTR CALLBACK DYNAMIC_SEL_FUNCS::OnGetShowString(void) const
{
	return m_strText;
}

AVariant CALLBACK DYNAMIC_SEL_FUNCS::OnGetValue(void) const
{
	return m_var;
}

void CALLBACK DYNAMIC_SEL_FUNCS::OnSetValue(const AVariant& var)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(pManager->m_listSelectedObject.GetCount()==1)
		{
			CSceneObject* pObj = (CSceneObject*)pManager->m_listSelectedObject.GetHead();
			if(pObj->GetObjectType()==CSceneObject::SO_TYPE_DYNAMIC)
			{
				unsigned int id = ((CSceneDynamicObject*)pObj)->GetDynamicObjectID();
				DYNAMIC_OBJECT obj;
				InitDynamicObj(&obj);
				g_DynamicObjectMan.GetObjByID(id,obj);
				m_strText.Format("[%d]%s",obj.id,obj.szName);
			}
		}
	}
}

//Controller select function
BOOL CALLBACK CONTROLLER_DYNAMIC_ID_FUNCS::OnActivate(void)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(pManager->m_listSelectedObject.GetCount()==1)
		{
			CSceneObject *pObj = (CSceneObject*)pManager->m_listSelectedObject.GetHead();
			if(pObj->GetObjectType()==CSceneObject::SO_TYPE_DYNAMIC)
			{
				CNpcContollerSelDlg dlg;
				int id = ((CSceneDynamicObject*)pObj)->GetControllerID();
				dlg.m_nDyObjID = id;
				if(dlg.DoModal()==IDOK)
				{
					id = dlg.m_nDyObjID;
					((CSceneDynamicObject*)pObj)->SetControllerID(id);
					CONTROLLER_OBJECT obj;
					InitControllerObj(&obj);
					g_ControllerObjectMan.GetObjByID(id,obj);
					if(obj.id==0) m_strText = "";
					else m_strText.Format("[%d]%s",obj.id,obj.szName);
				}
			}
		}
	}
	
	return TRUE;
}

LPCTSTR CALLBACK CONTROLLER_DYNAMIC_ID_FUNCS::OnGetShowString(void) const
{
	return m_strText;
}

AVariant CALLBACK CONTROLLER_DYNAMIC_ID_FUNCS::OnGetValue(void) const
{
	return m_var;
}

void CALLBACK CONTROLLER_DYNAMIC_ID_FUNCS::OnSetValue(const AVariant& var)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(pManager->m_listSelectedObject.GetCount()==1)
		{
			CSceneObject *pObj = (CSceneObject*)pManager->m_listSelectedObject.GetHead();
			if(pObj->GetObjectType()==CSceneObject::SO_TYPE_DYNAMIC)
			{
				CNpcContollerSelDlg dlg;
				int id = ((CSceneDynamicObject*)pObj)->GetControllerID();
				CONTROLLER_OBJECT obj;
				InitControllerObj(&obj);
				g_ControllerObjectMan.GetObjByID(id,obj);
				if(obj.id!=0) m_strText.Format("[%d]%s",obj.id,obj.szName);
			}
		}
	}
}



