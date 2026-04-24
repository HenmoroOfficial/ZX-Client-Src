#include "global.h"
#include "elementeditor.h"
#include "ElementMap.h"
#include "SceneObjectManager.h"
#include "SceneGuildBuilding.h"
#include "Render.h"
#include "MainFrm.h"
#include "Box3D.h"
#include "EC_Model.h"
#include "a3d.h"
#include "elementdataman.h"
#include "DataPathMan.h"
#include "DlgTmplIDSel.h"

#include <A3DCombinedAction.h>




//////////////////////////////////////////////////////////////////////
//class FIXED_NPC_ID_FUNCS
BOOL CALLBACK TMPL_ID_FUNCS::OnActivate(void)
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
			if(pObj->GetObjectType()==CSceneObject::SO_TYPE_GUILD_BUILDING)
			{
				CDlgTmplIDSel dlg;
				CSceneGuildBuilding *pObj1 = (CSceneGuildBuilding*)pObj;
				EC_GUILDBUILDING_PROPERTY prop = pObj1->GetProperty();

				dlg.m_dwSelId = prop.iTmplId;
				if(dlg.DoModal()==IDOK)
				{
					prop.iTmplId = dlg.m_dwSelId;
					pObj1->SetProperty(prop);
					strID.Format("(ID:%d)",prop.iTmplId);
					m_strText = dlg.m_strName;
					m_strText += strID;
					pObj1->UpdateProperty(false);
				}
			}
		}
	}

	return TRUE;
}

LPCTSTR CALLBACK TMPL_ID_FUNCS::OnGetShowString(void) const
{
	return m_strText;
}

AVariant CALLBACK TMPL_ID_FUNCS::OnGetValue(void) const
{
	return m_var;
}

void CALLBACK TMPL_ID_FUNCS::OnSetValue(const AVariant& var)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	CString strID;
	if(pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(pManager->m_listSelectedObject.GetCount()==1)
		{
			CSceneGuildBuilding* pObj = (CSceneGuildBuilding*)pManager->m_listSelectedObject.GetHead();
			if(pObj->GetObjectType()==CSceneObject::SO_TYPE_GUILD_BUILDING)
			{
				EC_GUILDBUILDING_PROPERTY pro = pObj->GetProperty();
				strID.Format("(ID:%d)",pro.iTmplId);

				DATA_TYPE type;
				REGION_BLOCK_ESSENCE* pData;
				pData = (REGION_BLOCK_ESSENCE*)g_dataMan.get_data_ptr(pro.iTmplId,ID_SPACE_ESSENCE,type);
				assert(type==DT_REGION_BLOCK_ESSENCE);
				m_strText = pData->name;
				m_strText += strID;
				m_var = var;
			}
		}
	}
}



//#define new A_DEBUG_NEW

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSceneGuildBuilding::CSceneGuildBuilding()
{
	m_pECModel = NULL;
	m_nObjectType = SO_TYPE_GUILD_BUILDING;
	m_pIDSelFuncs = new TMPL_ID_FUNCS;
	m_nCopy = 0;
	memset(&m_Property, 0, sizeof(m_Property));
	BuildPropery();
}

CSceneGuildBuilding::~CSceneGuildBuilding()
{

}

void CSceneGuildBuilding::Release()
{
	if(m_pECModel)
	{
		m_pECModel->Release();
		delete m_pECModel;
		m_pECModel = NULL;
	}
	if(m_pIDSelFuncs) delete m_pIDSelFuncs;
}

A3DVECTOR3 CSceneGuildBuilding::GetMin()
{
	if (m_pECModel)
	{
		ASSERT(m_pECModel);
		A3DAABB aabb = m_pECModel->GetA3DSkinModel()->GetModelAABB();
		return aabb.Mins;
	}
	else
		return CSceneObject::GetMin();
}

A3DVECTOR3 CSceneGuildBuilding::GetMax()
{
	if (m_pECModel)
	{
		ASSERT(m_pECModel);
		A3DAABB aabb = m_pECModel->GetA3DSkinModel()->GetModelAABB();
		return aabb.Maxs;
	}
	else
		return CSceneObject::GetMax();
}

void CSceneGuildBuilding::Translate(const A3DVECTOR3& vDelta)
{
	m_vPos += vDelta;
	m_matTrans.SetRow(3,m_vPos);
	m_matAbs = m_matScale * m_matRotate * m_matTrans;
	if (m_pECModel)
	{
		m_vPos = m_pECModel->GetA3DSkinModel()->GetPos();
		m_vPos +=vDelta;
		m_pECModel->GetA3DSkinModel()->SetPos(m_vPos);
	}	
}

void CSceneGuildBuilding::RotateX(float fRad)
{
	if (!m_pECModel)
		return;

	m_pECModel->GetA3DSkinModel()->RotateX(fRad);
}

void CSceneGuildBuilding::RotateY(float fRad)
{
	if (!m_pECModel)
		return;

	m_pECModel->GetA3DSkinModel()->RotateY(fRad);
}

void CSceneGuildBuilding::RotateZ(float fRad)
{
	if (!m_pECModel)
		return;

	m_pECModel->GetA3DSkinModel()->RotateZ(fRad);
}

void CSceneGuildBuilding::Rotate(const A3DVECTOR3& axis,float fRad)
{
	if (!m_pECModel)
		return;

	CSceneObject::Rotate(axis,fRad);
	A3DMATRIX4 mat;
	mat.Identity();
	mat.RotateAxis(axis,fRad);

	A3DVECTOR3 vDir = m_pECModel->GetA3DSkinModel()->GetDir();
	A3DVECTOR3 vUp = m_pECModel->GetA3DSkinModel()->GetUp();
	vDir = mat * vDir;
	vUp = mat * vUp;
	m_pECModel->GetA3DSkinModel()->SetDirAndUp(vDir,vUp);
}

void CSceneGuildBuilding::ResetRotateMat()
{
	if (!m_pECModel)
		return;

	CSceneObject::ResetRotateMat();
	m_pECModel->GetA3DSkinModel()->SetDirAndUp(A3DVECTOR3(0,0,1.0f),A3DVECTOR3(0,1.0f,0));
}

void CSceneGuildBuilding::Scale(float fScale)
{
	CSceneObject::Scale(fScale);
	///A3DVECTOR3 vScale = m_pECModel->GetScale();
	//vScale *= fScale; 
	//m_pECModel->SetScale(vScale.x,vScale.y,vScale.z);
}

void CSceneGuildBuilding::SetPos(const A3DVECTOR3& vPos)
{
	if (!m_pECModel)
		return;

	m_pECModel->SetPos(vPos);
	CSceneObject::SetPos(vPos);
}

void CSceneGuildBuilding::UpdateProperty(bool bGet)
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
				m_pProperty->SetPropVal(2,m_nCopy);
				m_pIDSelFuncs->OnSetValue(m_Property.iTmplId);
				m_pProperty->SetPropVal(4,m_Property.iWidth);
				m_pProperty->SetPropVal(5,m_Property.iHeight);
				m_pProperty->SetPropVal(6,m_nObjectID);

				AUX_GetMainFrame()->GetToolTabWnd()->UpdatePropertyData(false);
				return;
			}
		}
		m_strName = temp;
		m_vPos	= m_pProperty->GetPropVal(1);
		m_nCopy = m_pProperty->GetPropVal(2);
		m_Property.iWidth = m_pProperty->GetPropVal(4);
		m_Property.iHeight = m_pProperty->GetPropVal(5);
		m_matTrans.Translate(m_vPos.x,m_vPos.y,m_vPos.z);
		ReLoadModel();
	}else
	{
		m_pProperty->SetPropVal(0,m_strName);
		m_pProperty->SetPropVal(1,m_vPos);
		m_pProperty->SetPropVal(2,m_nCopy);
		m_pIDSelFuncs->OnSetValue(m_Property.iTmplId);
		m_pProperty->SetPropVal(4,m_Property.iWidth);
		m_pProperty->SetPropVal(5,m_Property.iHeight);
		m_pProperty->SetPropVal(6,m_nObjectID);
	}
}

bool CSceneGuildBuilding::LoadModel(AString strPathName)
{
	if(m_pECModel)
	{
		m_pECModel->Release();
	}else
	{
		m_pECModel = new CECModel;
	}
	bool bResult = m_pECModel->Load(strPathName,false,A3DSkinModel::LSF_NOSKIN,true,false);
	if(!bResult)
	{
		m_pECModel->Release();
		g_Log.Log("CSceneGuildBuilding::LoadSkinModel(), Failed load %s", strPathName);
		delete m_pECModel;
		m_pECModel = NULL;
		return false;
	}
	m_pECModel->GetA3DSkinModel()->SetAutoAABBType(A3DSkinModel::AUTOAABB_INITMESH);


	//这儿重新加载皮肤
	AString strSkin = strPathName;
	int pos = strSkin.ReverseFind('.');
	strSkin = strSkin.Left(pos);
	strSkin += ".ski";

	A3DSkinMan* pSkinMan = g_Render.GetA3DEngine()->GetA3DSkinMan();
	A3DSkin* pSkin = pSkinMan->LoadSkinFile(strSkin);
	if(pSkin)
	{
		m_pECModel->GetA3DSkinModel()->AddSkin(pSkin,true);
	}else 
	{
		g_Log.Log("CSceneGuildBuilding::LoadSkinModel(), Failed load %s", strSkin);
		//m_pECModel->Release();
		//delete m_pECModel;
		//m_pECModel = NULL;
		//return false;
	}


	return true;
}

void CSceneGuildBuilding::BuildPropery()
{
	if(m_pProperty)
	{
		m_pProperty->DynAddProperty(AVariant(m_strName), "名字");
		m_pProperty->DynAddProperty(AVariant(m_vPos), "位置坐标");
		m_pProperty->DynAddProperty(AVariant(m_nCopy), "第几套数据");
		m_pProperty->DynAddProperty(AVariant(m_Property.iTmplId),"地块列表", (ASet*)m_pIDSelFuncs, NULL, WAY_CUSTOM);
		m_pProperty->DynAddProperty(AVariant(m_Property.iWidth),"长");
		m_pProperty->DynAddProperty(AVariant(m_Property.iHeight),"宽");
		m_pProperty->DynAddProperty(AVariant(m_nObjectID), "场景对象ID（海外版专用）", NULL,NULL,WAY_READONLY);



		m_pProperty->SetPropVal(0,m_strName);
		m_pProperty->SetPropVal(1,m_vPos);
		m_pProperty->SetPropVal(2,m_nCopy);
		m_pProperty->SetPropVal(3,m_Property.iTmplId);
		m_pProperty->SetPropVal(4,m_Property.iWidth);
		m_pProperty->SetPropVal(5,m_Property.iHeight);
		m_pProperty->SetPropVal(6,m_nObjectID);
	}
}

void CSceneGuildBuilding::Render(A3DViewport* pA3DViewport)
{
	if(m_nCopy != g_Configs.nShowCopyNum) return;
	if(m_pECModel==NULL) return;

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

	CSceneObject::Render(pA3DViewport);
}

void	CSceneGuildBuilding::PostRender(A3DViewport* pA3DViewport)
{
	if(m_pECModel==NULL) return;
	DrawCenter();
	CBox3D renderBox;
	renderBox.Init(&g_Render,1.0f,false);
	A3DAABB aabb = m_pECModel->GetA3DSkinModel()->GetModelAABB();
	renderBox.SetSize(aabb.Extents.x,aabb.Extents.y,aabb.Extents.z);
	renderBox.SetPos(aabb.Center);
	renderBox.SetColor(A3DCOLORRGB(255,0,0));
	if(m_bSelected) renderBox.Render();	

	if (m_Property.iWidth>.0f && m_Property.iHeight>.0f)
	{
		renderBox.SetSize(m_Property.iWidth, 2.0f, m_Property.iHeight);
		renderBox.SetColor(A3DCOLORRGB(0,255,0));
		renderBox.Render();
	}
	CSceneObject::PostRender(pA3DViewport);

}

void CSceneGuildBuilding::RenderForLight(A3DViewport* pA3DViewport, float fOffsetX, float fOffsetZ)
{
	if(m_pECModel==0) return;
	A3DLIGHTPARAM lightParams = g_Render.GetDirLight()->GetLightparam();
	A3DSkinModel::LIGHTINFO LightInfo;
	LightInfo.colAmbient	= g_Render.GetA3DDevice()->GetAmbientColor();
	LightInfo.vLightDir		= lightParams.Direction;//Direction of directional light
	LightInfo.colDirDiff	= lightParams.Diffuse;//Directional light's diffuse color
	LightInfo.colDirSpec	= lightParams.Specular;//	Directional light's specular color
	LightInfo.bPtLight		= false;//	false, disable dynamic point light,
	m_pECModel->GetA3DSkinModel()->SetLightInfo(LightInfo);
	m_pECModel->SetPos(m_vPos + A3DVECTOR3(fOffsetX,0.0f,fOffsetZ));
	m_pECModel->Render(pA3DViewport);
	m_pECModel->SetPos(m_vPos);
}

void CSceneGuildBuilding::RenderForLightMapShadow(A3DViewport* pA3DViewport)
{
	if(m_pECModel==0) return;
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

void CSceneGuildBuilding::Tick(DWORD timeDelta)
{
	if(m_pECModel)
	{
		m_pECModel->Tick(timeDelta);
	}
}

void CSceneGuildBuilding::SetDirAndUp(const A3DVECTOR3& vDir, const A3DVECTOR3& vUp)
{
	if(m_pECModel) m_pECModel->SetDirAndUp(vDir,vUp);
	CSceneObject::SetDirAndUp(vDir,vUp);
}

//	Load data
bool CSceneGuildBuilding::Load(CELArchive& ar,DWORD dwVersion,int iLoadFlags)
{
	if (dwVersion<0x53)
	{
		A3DVECTOR3 vr,vu,vl,vs;
		m_strName = AString(ar.ReadString());
		if(!ar.Read(&m_Property,sizeof(m_Property)))
			return false;
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

		m_matTrans.SetRow(3,m_vPos);
		m_matRotate.SetRow(2,vr);
		m_matRotate.SetRow(1,vu);
		m_matRotate.SetRow(0,vl);
		m_matScale._11 = vs.x;
		m_matScale._22 = vs.y;
		m_matScale._33 = vs.z;

		ReLoadModel();
	}
	else
	{
		A3DVECTOR3 vr,vu,vl,vs;
		m_strName = AString(ar.ReadString());
		if(!ar.Read(&m_Property,sizeof(m_Property)))
			return false;
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_nCopy,sizeof(int)))
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

		m_matTrans.SetRow(3,m_vPos);
		m_matRotate.SetRow(2,vr);
		m_matRotate.SetRow(1,vu);
		m_matRotate.SetRow(0,vl);
		m_matScale._11 = vs.x;
		m_matScale._22 = vs.y;
		m_matScale._33 = vs.z;

		ReLoadModel();
	}
return true;
}
//	Save data
bool CSceneGuildBuilding::Save(CELArchive& ar,DWORD dwVersion)
{
	m_matRotate = GetRotateMatrix();
	A3DVECTOR3 vr,vu,vl,vs;
	vr = m_matRotate.GetRow(2);
	vu = m_matRotate.GetRow(1);
	vl = m_matRotate.GetRow(0);
	vs = A3DVECTOR3(m_matScale._11,m_matScale._22,m_matScale._33);
	ar.WriteString(m_strName);

	ar.Write(&m_Property, sizeof(m_Property));

	if(!ar.Write(&m_nObjectID,sizeof(int)))
		return false;
	if(!ar.Write(&m_nCopy,sizeof(int)))
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

	return true;
}

int	CSceneGuildBuilding::GetTriangleNum()
{
	if(m_pECModel)
	{
		if(m_pECModel->GetA3DSkinModel())
		{
			int nTriangle = 0;
			int nSkin = m_pECModel->GetA3DSkinModel()->GetSkinNum();
			for( int i = 0; i < nSkin; i++)
			{
				A3DSkin *pSkin = m_pECModel->GetA3DSkinModel()->GetA3DSkin(i);
				int num = pSkin->GetMorphRigidMeshNum();
				int l;
				for( l = 0; l < num; l++)
				{
					A3DMorphRigidMesh *pMesh = pSkin->GetMorphRigidMesh(l);
					nTriangle += pMesh->GetIndexNum()/3;
				}

				num = pSkin->GetMorphSkinMeshNum();
				for( l = 0; l < num; l++)
				{
					A3DMorphSkinMesh *pMesh = pSkin->GetMorphSkinMesh(l);
					nTriangle += pMesh->GetIndexNum()/3;
				}

				num = pSkin->GetMuscleMeshNum();
				for( l = 0; l < num; l++)
				{
					A3DMuscleMesh *pMesh = pSkin->GetMuscleMesh(l);
					nTriangle +=pMesh->GetIndexNum()/3;
				}

				num = pSkin->GetRigidMeshNum();
				for( l = 0; l < num; l++)
				{
					A3DRigidMesh *pMesh = pSkin->GetRigidMesh(l);
					nTriangle +=pMesh->GetIndexNum()/3;
				}

				num = pSkin->GetSkinMeshNum();
				for( l = 0; l < num; l++)
				{
					A3DSkinMesh *pMesh = pSkin->GetSkinMesh(l);
					nTriangle +=pMesh->GetIndexNum()/3;
				}

				num = pSkin->GetSuppleMeshNum();
				for( l = 0; l < num; l++)
				{
					A3DSuppleMesh *pMesh = pSkin->GetSuppleMesh(l);
					nTriangle +=pMesh->GetIndexNum()/3;
				}
			}
			return nTriangle;
		}
	}
	return 0;
}

void CSceneGuildBuilding::ReLoadModel()
{
	DATA_TYPE type;
	REGION_BLOCK_ESSENCE* pDataBlock = (REGION_BLOCK_ESSENCE*)g_dataMan.get_data_ptr(m_Property.iTmplId, ID_SPACE_ESSENCE, type);
	if (type!=DT_REGION_BLOCK_ESSENCE)
		return;

	BUILDING_ESSENCE* pDataBuilding = (BUILDING_ESSENCE*)g_dataMan.get_data_ptr(pDataBlock->building[0], ID_SPACE_ESSENCE, type);
	if (type!=DT_BUILDING_ESSENCE)
		return;
	
	AString strModel = g_dataPathMan.GetPathByID(pDataBuilding->up_building[0].building_model);
	if(!LoadModel(strModel))
	{
		CString msg;
		g_Log.Log(msg);
		msg.Format("CSceneGuildBuilding::Load(),Failed to Load skin model %s !", strModel);
		AfxMessageBox(msg);
		return;
	}
	m_pECModel->GetA3DSkinModel()->SetPos(m_vPos);
	m_pECModel->GetA3DSkinModel()->SetDirAndUp(m_matRotate.GetRow(2),m_matRotate.GetRow(1));
}
