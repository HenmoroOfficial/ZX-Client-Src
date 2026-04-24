// StaticModelObject.cpp: implementation of the CStaticModelObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "elementeditor.h"
#include "StaticModelObject.h"
#include "Box3D.h"
#include "Render.h"
#include "EL_Building.h"

#include "ModelContainer.h"
#include "A3D.h"
#include "SceneObjectManager.h"
#include "SceneBoxArea.h"
#include "MainFrm.h"
#include "SceneLightObject.h"
#include "LightMapSizeDlg.h"
//#include "EffectProperty.h"
#ifdef _ANGELICA22
#include "DlgLitModelMtlProperty.h"
#endif

//#define new A_DEBUG_NEW

//#define ENABLE_EFFECT_EDIT		// 以后打开这个的话需要升级版本号!
#define EFFECT_DESC_BEGIN	"EffectDesc"
#define EFFECT_DESC_END		"/EffectDesc"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStaticModelObject::CStaticModelObject()
{
	m_pModel = NULL;
	m_bNoShadow = false;
	m_bCircle = false;
	m_bIsTree = false;
	m_nObjectType = SO_TYPE_STATIC_MODEL; 
	m_vPos = A3DVECTOR3(0);
	m_bReflect = false;
	m_bRefraction = false;
	m_dwLastTime = 0;
	m_bOptimizeEnable  = false;
	m_strModelPath = "";
	m_dwCollideFlags = 0;
	m_bSLAffect = true;
	m_nAutoLightmapSize = -1;
    m_nCustomLightmapSize = -1;
	m_fLitDirectScale = 1;
	m_fLitIndirectScale = 1;
	m_fLitAOScale = 1;
	m_strEmissiveMap = "";
	m_fRange = 10;
	m_fAttenuation0 = 0;
	m_fAttenuation1 = 1;
	m_fAttenuation2 = 0;
	m_fPower = 1;
	m_nEffectType = 2;
	m_nSampleCount = 50;
    m_bUseConsistentNormal = false;
    m_bIgnoreInnerShadow = false;
    m_pLightMapSizeFuns = new LIGHTMAP_SIZE_FUNCS(this);

	m_pEffectPropertyFuns = new EFFECT_PROPERTY_FUNCS();

	m_aSoundSet.AddElement("大石",0);
	m_aSoundSet.AddElement("沙滩",1);
	m_aSoundSet.AddElement("雪地",2);
	m_aSoundSet.AddElement("冰面",3);
	m_aSoundSet.AddElement("草地",4);
	m_aSoundSet.AddElement("碎石",5);
	m_aSoundSet.AddElement("泥土",6);
	m_aSoundSet.AddElement("浅水",7);
	m_aSoundSet.AddElement("木质",8);
	m_aSoundSet.AddElement("金属",9);
	m_aSoundSet.AddElement("岩浆",10);
	m_aSoundSet.AddElement("地毯",11);
	m_aSoundSet.AddElement("特殊1",12);
	m_aSoundSet.AddElement("特殊2",13);
	m_aSoundSet.AddElement("特殊3",14);
	m_aSoundSet.AddElement("特殊4",15);
	m_aSoundSet.AddElement("特殊5",16);
	m_aSoundSet.AddElement("特殊6",17);
	m_aSoundSet.AddElement("特殊7",18);
	m_aSoundSet.AddElement("特殊8",19);
	m_aSoundSet.AddElement("特殊9",20);


	m_aLitmapSize.AddElement("默认"		, -1);
	m_aLitmapSize.AddElement("64x64"	, 64);
	m_aLitmapSize.AddElement("128x128"	, 128);
	m_aLitmapSize.AddElement("256x256"	, 256);
	m_aLitmapSize.AddElement("512x512"	, 512);

	light_effect_type.AddElement("白天", 0);
	light_effect_type.AddElement("黑夜", 1);
	light_effect_type.AddElement("白天黑夜", 2);

	m_CollideType.AddElement("显示并碰撞", 0);
	m_CollideType.AddElement("只碰撞不显示", 1);
	m_CollideType.AddElement("不碰撞的模型", 2);

	m_nFeetSound = 0; 
	m_fLoadRadius = 0.0f;

	BuildProperty();
}

bool CStaticModelObject::BuildProperty()
{
	if(m_pProperty/*CSceneObject::BuildProperty()*/)
	{
		AString strHullPathName;
//		if(m_pModel && m_pModel->pModel) strHullPathName = m_pModel->pModel->GetChfPath();
		m_pProperty->DynAddProperty(AVariant(m_strName), "名字");
		m_pProperty->DynAddProperty(AVariant(m_vPos), "位置坐标");
		m_pProperty->DynAddProperty(AVariant(m_bNoShadow),"不产生阴影");
		m_pProperty->DynAddProperty(AVariant(m_vPos), "是否反射");
		m_pProperty->DynAddProperty(AVariant(m_bNoShadow),"是否折射");
		m_pProperty->DynAddProperty(AVariant(m_strModelPath),"模型路径",NULL,NULL,WAY_FILENAME,"");
		m_pProperty->DynAddProperty(AVariant(strHullPathName),"凸包路径",NULL,NULL,WAY_READONLY);
		m_pProperty->DynAddProperty(AVariant(m_nObjectID),"对象ID",NULL,NULL,WAY_READONLY);
		m_pProperty->DynAddProperty(AVariant(GetCollideFlags()),"只碰撞", &m_CollideType);
		m_pProperty->DynAddProperty(AVariant(m_bOptimizeEnable),"是否可被优化");
		m_pProperty->DynAddProperty(AVariant(m_bIsTree),"是否是树");
		m_pProperty->DynAddProperty(AVariant(m_bSLAffect),"受非主方向光影响");
		m_pProperty->DynAddProperty(AVariant(m_nAutoLightmapSize),"自动光照图大小",NULL,NULL,WAY_READONLY);
		m_pProperty->DynAddProperty(AVariant(m_fLitDirectScale),"直接光照系数");
		m_pProperty->DynAddProperty(AVariant(m_fLitIndirectScale),"间接光照系数");
		m_pProperty->DynAddProperty(AVariant(m_fLitAOScale),"环境遮蔽系数");
		m_pProperty->DynAddProperty(AVariant(m_strEmissiveMap),"自发光贴图", NULL, NULL, WAY_FILENAME, "");
		m_pProperty->DynAddProperty(AVariant(m_fPower),"亮度系数");
		m_pProperty->DynAddProperty(AVariant(m_fRange), "光照范围");
		m_pProperty->DynAddProperty(AVariant(m_fAttenuation0),"衰减0");
		m_pProperty->DynAddProperty(AVariant(m_fAttenuation1),"衰减1");
		m_pProperty->DynAddProperty(AVariant(m_fAttenuation2),"衰减2");
		m_pProperty->DynAddProperty(AVariant(m_nEffectType), "灯光影响", &light_effect_type);
		m_pProperty->DynAddProperty(AVariant(m_nSampleCount),"自发光采样");
		m_pProperty->DynAddProperty(AVariant(m_nFeetSound),"脚步声",&m_aSoundSet);
        m_pProperty->DynAddProperty(AVariant(0.0f),"体积",NULL,NULL,WAY_READONLY);
        m_pProperty->DynAddProperty(AVariant(m_nCustomLightmapSize),"自定义光照图大小",(ASet*)m_pLightMapSizeFuns, NULL, WAY_CUSTOM);
        m_pProperty->DynAddProperty(AVariant("0.00%"),"光照图利用率",NULL,NULL,WAY_READONLY);
        m_pProperty->DynAddProperty(AVariant(0),"子模型数",NULL,NULL,WAY_READONLY);
        m_pProperty->DynAddProperty(AVariant(0),"三角形数",NULL,NULL,WAY_READONLY);
		m_pProperty->DynAddProperty(0,"模型材质",(ASet*)m_pEffectPropertyFuns, NULL, WAY_CUSTOM);
		m_pProperty->DynAddProperty(AVariant(m_fLoadRadius),"模型的加载半径");
        m_pProperty->DynAddProperty(AVariant(m_bUseConsistentNormal),"平滑组法线校正");
        m_pProperty->DynAddProperty(AVariant(m_bIgnoreInnerShadow),"内部无阴影");
		return true;
	}
	return false;
}

CStaticModelObject::~CStaticModelObject()
{

}

void CStaticModelObject::Release()
{
	if(m_pModel )
	{
		if(m_pModel->pModel)
		{
			m_pModel->pModel->Release();
			delete m_pModel->pModel;
			m_pModel->pModel = NULL;
		}
		delete m_pModel;
		m_pModel = NULL;
	}

    if(m_pLightMapSizeFuns)
    {
        delete m_pLightMapSizeFuns;
        m_pLightMapSizeFuns = NULL;
    }

	if(m_pEffectPropertyFuns)
	{
		delete m_pEffectPropertyFuns;
		m_pEffectPropertyFuns = NULL;
	}
}

/*
void CStaticModelObject::SetDirAndUp(A3DVECTOR3 vr,A3DVECTOR3 vu, A3DVECTOR3 vl)
{
	if(m_pModel)
	{
		if(m_pModel->pModel)
		{
			m_pModel->pModel->SetDirAndUp(vr,vu);
		}
	}
	CSceneObject::SetDirAndUp(vr,vu,vl);
}*/

A3DVECTOR3 CStaticModelObject::GetMin()
{
	A3DVECTOR3 vMin(0.0f);
	if(m_pModel && m_pModel->pModel)
		vMin = m_pModel->pModel->GetModelAABB().Mins;
	return vMin;
}

A3DVECTOR3 CStaticModelObject::GetMax()
{
	A3DVECTOR3 vMax(0.0f);
	if(m_pModel && m_pModel->pModel)
		vMax = m_pModel->pModel->GetModelAABB().Maxs;
	return vMax;
}

/*
A3DVECTOR3 CStaticModelObject::GetPos()
{ 
	return m_pModel->pModel->GetPos();
};

A3DMATRIX4 CStaticModelObject::GetTransMatrix()
{ 
	return m_pModel->pModel->GetAbsoluteTM(); 
};

A3DMATRIX4 CStaticModelObject::GetRotateMatrix()
{ 
	A3DMATRIX4 mat;
	mat.Identity();
	mat = m_pModel->pModel->GetAbsoluteTM();
	mat.SetRow(3,A3DVECTOR3(0.0f));
	A3DVECTOR3 vecX = mat.GetRow(0);
	A3DVECTOR3 vecY = mat.GetRow(1);
	A3DVECTOR3 vecZ = mat.GetRow(2);
	vecX.Normalize();
	vecY.Normalize();
	vecZ.Normalize();
	mat.SetRow(0, vecX);
	mat.SetRow(1, vecY);
	mat.SetRow(2, vecZ);
	return mat;
};

A3DMATRIX4 CStaticModelObject::GetScaleMatrix()
{ 
	A3DMATRIX4 mat;
	mat.Identity();
	A3DVECTOR3 v = m_pModel->pModel->GetScale();
	mat._11 = v.x;
	mat._22 = v.y;
	mat._33 = v.z;
	return mat; 
};

void CStaticModelObject::SetScale(A3DVECTOR3 vs)
{
	if(m_pModel)
		m_pModel->pModel->SetScale(vs.x);
	CSceneObject::SetScale(vs);
}
*/

void CStaticModelObject::UpdateModel()
{
    m_Obb.Build(A3DAABB(0, 0));
	if(m_pModel)
	{
		if(m_pModel->pModel)
		{
			(m_pModel->pModel)->SetPos(m_vPos);
			(m_pModel->pModel)->SetScale(m_matScale._11);
			(m_pModel->pModel)->SetCollideOnly(GetCollideFlags() == 1);
			
			A3DVECTOR3 up = g_vAxisY * m_matRotate;
			A3DVECTOR3 dir = g_vAxisZ * m_matRotate;
			(m_pModel->pModel)->SetDirAndUp(dir,up);

            // Compute OBB
            A3DLitModel* pLitModel = m_pModel->pModel->GetA3DLitModel();
            if (pLitModel)
            {
                AArray<A3DVECTOR3> aPosList;
                for (int iMesh = 0; iMesh < pLitModel->GetNumMeshes(); iMesh++)
                {
                    A3DLitMesh* pMesh = pLitModel->GetMesh(iMesh);
                    // skip vertex light meshes
                    if (pMesh->GetLightMapCoords() == NULL)
                        continue;
                    for (int iVert = 0; iVert < pMesh->GetNumVerts(); iVert++)
                    {
                        aPosList.Add(pMesh->GetVerts()[iVert].pos);
                    }
                }
                m_Obb.Build(aPosList.GetData(), aPosList.GetSize());
                m_Obb.CompleteExtAxis();
				m_pEffectPropertyFuns->SetModel(pLitModel);
            }
		}
	}
}

void CStaticModelObject::PostRender(A3DViewport* pA3DViewport)
{
	if(m_pModel==NULL) return;
	if(m_pModel->pModel == NULL) return;
	if(g_bShowCollideModelOnly)
	{
		if(!GetCollideFlags()) return;
	}
	DrawCenter();

	m_vMin = ((m_pModel->pModel)->GetModelAABB()).Mins;
	m_vMax = ((m_pModel->pModel)->GetModelAABB()).Maxs;
	if(m_bSelected)
	{
		DWORD timeCur = a_GetTime();
		DWORD timeDelta = timeCur - m_dwLastTime;
		A3DCOLOR curClr;
		if(timeDelta>1000)
		{
			m_dwLastTime = timeCur;
			m_bCircle = !m_bCircle;
			timeDelta = 0;
		}
		if(m_bCircle) curClr = A3DCOLORRGB((int)(0.2f*timeDelta),(int)(0.2f*(1000 - timeDelta)),100);
		else curClr = A3DCOLORRGB((int)(0.2f*(1000-timeDelta)),(int)(0.2f*timeDelta),100);

		CBox3D renderBox;
		renderBox.Init(&g_Render,1.0f,false);
		renderBox.SetSize((m_pModel->pModel)->GetModelAABB().Extents.x,(m_pModel->pModel)->GetModelAABB().Extents.y,(m_pModel->pModel)->GetModelAABB().Extents.z);
		renderBox.SetPos((m_pModel->pModel)->GetModelAABB().Center);
		renderBox.SetColor(A3DCOLORRGB(255,0,0));
		renderBox.Render();

		if(g_Configs.bShowModelHull) 
			(m_pModel->pModel)->RenderConvexHull(pA3DViewport);

		if(g_bShowModelPlane)
		{
			CBox3D plane;
			plane.Init(&g_Render,100.0f,true);
			plane.SetDirAndUp(A3DVECTOR3(0,0,1.0f),A3DVECTOR3(0,1.0f,0));
			plane.SetColor(A3DCOLORRGBA(255,255,255,255));
			plane.SetSize((m_pModel->pModel)->GetModelAABB().Extents.x + 5.0f,0.01f,(m_pModel->pModel)->GetModelAABB().Extents.z + 5.0f);
			A3DVECTOR3 vp = m_vPos;
			vp.y = (m_pModel->pModel)->GetModelAABB().Mins.y;
			plane.SetPos(vp);
			plane.Render();
		}
	}
	else
	{
		CElementMap *pMap = AUX_GetMainFrame()->GetMap();
		if(g_Configs.bShowModelHull) 
			(m_pModel->pModel)->RenderConvexHull(pA3DViewport);

		m_bCircle = false;
	}

	CSceneObject::PostRender(pA3DViewport);
}

void CStaticModelObject::Render(A3DViewport* pA3DViewport)
{
	if(m_pModel==NULL) return;
	if(m_pModel->pModel == NULL) return;
	if(g_bShowCollideModelOnly)
	{
		if(!GetCollideFlags()) return;
	}

	//int iTick = GetTickCount();
	if(!g_Configs.bShowModelHull) 
		(m_pModel->pModel)->Render(pA3DViewport);

	/*int iTick1 = GetTickCount();

	AString strTime;
	strTime.Format("time:%d ms, mesh:%d, model id:%d, litmodel:%s\n", iTick1-iTick, m_pModel->pModel->GetA3DLitModel()->GetNumMeshes(), m_pModel->id, GetModelPath());
	if(iTick1-iTick>5)
		OutputDebugStringA(strTime);
	*/
	CSceneObject::Render(pA3DViewport);
}

void CStaticModelObject::RenderForLight(A3DViewport* pA3DViewport, float fOffsetX, float fOffsetZ)
{
	if(m_pModel==NULL) return;
	if(m_pModel->pModel == NULL) return;
	A3DVECTOR3 vDelta(fOffsetX,0.0f,fOffsetZ);
	vDelta = m_vPos + vDelta;
	m_pModel->pModel->SetPos(vDelta);
	m_pModel->pModel->Render(pA3DViewport);
}

void CStaticModelObject::RenderForLightMapShadow(A3DViewport* pA3DViewport)
{
	if(m_pModel==NULL) return;
	if(m_pModel->pModel == NULL) return;
    if (m_bNoShadow) return;
	if(m_bIsTree)
	{
		if(!g_Configs.bShowTree) return;
	}
	(m_pModel->pModel)->Render(pA3DViewport);
	
}

float CStaticModelObject::ComputeModelArea() const
{
    if (m_pModel == NULL || m_pModel->pModel == NULL)
        return 0;

    float fScale = m_pModel->pModel->GetScale().x * m_pModel->pModel->GetScale().y;
    float fArea = 0;
    for (int iMesh = 0; iMesh < m_pModel->pModel->GetA3DLitModel()->GetNumMeshes(); iMesh++)
    {
        A3DLitMesh* pMesh = m_pModel->pModel->GetA3DLitModel()->GetMesh(iMesh);

        A3DLIGHTMAPCOORD* pCoord = pMesh->GetLightMapCoords();
        if (pCoord == NULL)
            continue;

        A3DLMVERTEX* pVertices = pMesh->GetVerts();
        WORD* pIndices = pMesh->GetIndices();
        for (int iFace = 0; iFace < pMesh->GetNumFaces(); iFace++)
        {
            A3DVECTOR3 v1 = pVertices[pIndices[iFace * 3 + 0]].pos - pVertices[pIndices[iFace * 3 + 1]].pos;
            A3DVECTOR3 v2 = pVertices[pIndices[iFace * 3 + 0]].pos - pVertices[pIndices[iFace * 3 + 2]].pos;
            A3DVECTOR3 v3 = CrossProduct(v1, v2);
            fArea += v3.Magnitude();
        }
    }
    return fArea / 2 * fScale;
}

void CStaticModelObject::UpdateProperty(bool bGet)
{
	if(!m_pProperty) return;
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
				m_pProperty->SetPropVal(2,m_bNoShadow);
				m_pProperty->SetPropVal(3,m_bReflect);
				m_pProperty->SetPropVal(4,m_bRefraction);
				if(m_pModel)
				{
					m_pProperty->SetPropVal(5,m_strModelPath);
//					if(m_pModel->pModel)
//						m_pProperty->SetPropVal(6,m_pModel->pModel->GetChfPath());
				}
				m_pProperty->SetPropVal(7,m_nObjectID);
				m_pProperty->SetPropVal(8,GetCollideFlags());
				m_pProperty->SetPropVal(9,m_bOptimizeEnable);
				m_pProperty->SetPropVal(10,m_bIsTree);
				m_pProperty->SetPropVal(11,m_bSLAffect);
				m_pProperty->SetPropVal(12,m_nAutoLightmapSize);
				m_pProperty->SetPropVal(13,m_fLitDirectScale);
				m_pProperty->SetPropVal(14,m_fLitIndirectScale);
				m_pProperty->SetPropVal(15,m_fLitAOScale);
				m_pProperty->SetPropVal(16,m_strEmissiveMap);
				m_pProperty->SetPropVal(17,m_fPower);
				m_pProperty->SetPropVal(18,m_fRange);
				m_pProperty->SetPropVal(19,m_fAttenuation0);
				m_pProperty->SetPropVal(20,m_fAttenuation1);
				m_pProperty->SetPropVal(21,m_fAttenuation2);
				m_pProperty->SetPropVal(22,m_nEffectType);
				m_pProperty->SetPropVal(23,m_nSampleCount);
				m_pProperty->SetPropVal(24,m_nFeetSound);
				m_pProperty->SetPropVal(31,m_fLoadRadius);
				m_pProperty->SetPropVal(32,m_bUseConsistentNormal);
                m_pProperty->SetPropVal(33,m_bIgnoreInnerShadow);
                m_pLightMapSizeFuns->OnSetValue(m_nCustomLightmapSize);
                
				AUX_GetMainFrame()->GetToolTabWnd()->UpdatePropertyData(false);
				return;
			}
		}
		m_strName = temp;
		m_vPos	= m_pProperty->GetPropVal(1);
		m_bNoShadow = m_pProperty->GetPropVal(2);
		m_bReflect = m_pProperty->GetPropVal(3);
		m_bRefraction = m_pProperty->GetPropVal(4);
		AString strnew = AString(m_pProperty->GetPropVal(5));
		m_bSLAffect = m_pProperty->GetPropVal(11);
		m_matTrans.Translate(m_vPos.x,m_vPos.y,m_vPos.z);
		SetPos(m_vPos);
		if(m_pModel)
		{
			if(_stricmp(strnew,m_strModelPath)!=0)
			{
				m_strModelPath = strnew;
				ReloadModel();
			}
		}
		DWORD bCollide = m_pProperty->GetPropVal(8);
		SetCollideFlags(bCollide);
		if(bCollide == 2)
		{
			// 卸载凸包数据
			if(GetA3DModel() && GetA3DModel()->pModel)
			{
//				AString strChfSaved = GetA3DModel()->pModel->GetChfPath();
//				GetA3DModel()->pModel->SetChfPath(NULL);
//				GetA3DModel()->pModel->LoadChf();
//				GetA3DModel()->pModel->SetChfPath(strChfSaved);
			}
		}
		// else ... // 现在不能判断是否处于凸包加载状态。暂时使用凸包是否显示来代替
		else if(g_Configs.bShowModelHull)
		{
			if(GetA3DModel() && GetA3DModel()->pModel)
			{
//				GetA3DModel()->pModel->LoadChf();
			}
		}
		m_bOptimizeEnable = m_pProperty->GetPropVal(9);
		m_bIsTree = m_pProperty->GetPropVal(10);
		//m_nAutoLightmapSize = m_pProperty->GetPropVal(12);
		m_fLitDirectScale = m_pProperty->GetPropVal(13);
		m_fLitIndirectScale = m_pProperty->GetPropVal(14);
		m_fLitAOScale = m_pProperty->GetPropVal(15);
		m_strEmissiveMap = AString(m_pProperty->GetPropVal(16));
		m_fPower = m_pProperty->GetPropVal(17);
		m_fRange = m_pProperty->GetPropVal(18);
		m_fAttenuation0 = m_pProperty->GetPropVal(19);
		m_fAttenuation1 = m_pProperty->GetPropVal(20);
		m_fAttenuation2 = m_pProperty->GetPropVal(21);
		m_nEffectType = m_pProperty->GetPropVal(22);
		m_nSampleCount = m_pProperty->GetPropVal(23);
		m_nFeetSound	= m_pProperty->GetPropVal(24);
		m_fLoadRadius = m_pProperty->GetPropVal(31);
        m_bUseConsistentNormal = m_pProperty->GetPropVal(32);
        m_bIgnoreInnerShadow = m_pProperty->GetPropVal(33);
	}else
	{
		m_pProperty->SetPropVal(0,m_strName);
		m_pProperty->SetPropVal(1,m_vPos);
		m_pProperty->SetPropVal(2,m_bNoShadow);
		m_pProperty->SetPropVal(3,m_bReflect);
		m_pProperty->SetPropVal(4,m_bRefraction);
		if(m_pModel)
		{
			m_pProperty->SetPropVal(5,m_strModelPath);
//			if(m_pModel->pModel)
//				m_pProperty->SetPropVal(6,m_pModel->pModel->GetChfPath());
		}
		m_pProperty->SetPropVal(7,m_nObjectID);
		m_pProperty->SetPropVal(8,GetCollideFlags());
		m_pProperty->SetPropVal(9,m_bOptimizeEnable);
		m_pProperty->SetPropVal(10,m_bIsTree);
		m_pProperty->SetPropVal(11,m_bSLAffect);
        m_nAutoLightmapSize = GetAdaptLightmapSize(STATIC_OBJ_LITMAP_SIZE_FACTOR);
		m_pProperty->SetPropVal(12,m_nAutoLightmapSize);
		m_pProperty->SetPropVal(13,m_fLitDirectScale);
		m_pProperty->SetPropVal(14,m_fLitIndirectScale);
		m_pProperty->SetPropVal(15,m_fLitAOScale);
		m_pProperty->SetPropVal(16,m_strEmissiveMap);
		m_pProperty->SetPropVal(17,m_fPower);
		m_pProperty->SetPropVal(18,m_fRange);
		m_pProperty->SetPropVal(19,m_fAttenuation0);
		m_pProperty->SetPropVal(20,m_fAttenuation1);
		m_pProperty->SetPropVal(21,m_fAttenuation2);
		m_pProperty->SetPropVal(22,m_nEffectType);
		m_pProperty->SetPropVal(23,m_nSampleCount);
	    m_pProperty->SetPropVal(24,m_nFeetSound);

        m_pProperty->SetPropVal(25, ComputeVolume());
        m_pLightMapSizeFuns->OnSetValue(m_nCustomLightmapSize);
        AString strRate;
        strRate.Format("%.02f%%",GetLightmapRate() * 100);
        m_pProperty->SetPropVal(27, strRate);
        
        m_pProperty->SetPropVal(28, GetMeshCount());
        m_pProperty->SetPropVal(29, GetFaceCount());
		m_pProperty->SetPropVal(31,m_fLoadRadius);
        m_pProperty->SetPropVal(32,m_bUseConsistentNormal);
        m_pProperty->SetPropVal(33,m_bIgnoreInnerShadow);
	}
}

int GetAdaptLightmapSize(A3DLitModel* pModel)
{
	if(pModel==NULL) return 128;
	
	A3DAABB aabbModel = pModel->GetModelAABB();
	float vol = aabbModel.Extents.x * aabbModel.Extents.y * aabbModel.Extents.z * 8;
	
	if(vol < 64.0f) return 64; // 4*4*4
	if(vol < 32768) return 128;// 32*32*32
	if(vol < 262144) return 256; //64*64*64
	
	return 512;
}

void CStaticModelObject::ReloadModel()
{
	if(m_pModel)
	{
		CELBuilding *pLitModel = m_pModel->pModel;
		if(pLitModel==0) pLitModel = new CELBuilding;
		else pLitModel->Release();
		
		//Reload object
		AFile sFile;
		if(!sFile.Open(m_strModelPath,AFILE_OPENEXIST))
		{
			g_Log.Log("CStaticModelObject::ReloadModel(),Failed to open file %s",m_strModelPath);
			delete m_pModel->pModel;
			m_pModel->pModel = NULL;
			return;
		}else
		{
			if(!pLitModel->Load(g_Render.GetA3DDevice(),&sFile))
			{
				delete m_pModel->pModel;
				m_pModel->pModel = NULL;
				g_Log.Log("CStaticModelObject::ReloadModel(),Failed to load file %s",m_strModelPath);
				return;
			}
		}
		sFile.Close();
	
		m_pModel->pModel = pLitModel;
		UpdateModel();
	
        m_nAutoLightmapSize = GetAdaptLightmapSize(STATIC_OBJ_LITMAP_SIZE_FACTOR);
		
	}
}

bool CStaticModelObject::RayTrace(const A3DVECTOR3& vStart, const A3DVECTOR3& vEnd,A3DVECTOR3& inter)
{
	
	if(m_pModel)
	{
		if(m_pModel->pModel)
		{
			if(g_bShowCollideModelOnly)
			if(!GetCollideFlags()) return false;
			A3DVECTOR3 pos,nor;
			float fFraction = 0.1f;
			if(m_pModel->pModel->RayTraceMesh(vStart,vEnd - vStart,pos,&fFraction,nor))
			{
				inter = pos;
				return true;
			}
		}
	}
	
	return false;
}

/*
void CStaticModelObject::Translate(A3DVECTOR3 vDelta)
{
	m_vPos = m_pModel->pModel->GetPos();
	m_vPos +=vDelta;
	m_pModel->pModel->SetPos(m_vPos);
}

void CStaticModelObject::RotateX(float fRad)
{
	m_pModel->pModel->RotateX(fRad);
}

void CStaticModelObject::RotateY(float fRad)
{
	m_pModel->pModel->RotateY(fRad);
}

void CStaticModelObject::RotateZ(float fRad)
{
	m_pModel->pModel->RotateZ(fRad);
}

void CStaticModelObject::Rotate(A3DVECTOR3 axis,float fRad)
{
	A3DMATRIX4 mat;
	mat.Identity();
	mat.RotateAxis(axis,fRad);
	
	A3DVECTOR3 vDir = m_pModel->pModel->GetDir();
	A3DVECTOR3 vUp = m_pModel->pModel->GetUp();
	vDir = mat * vDir;
	vUp = mat * vUp;
	m_pModel->pModel->SetDirAndUp(vDir,vUp);
}

void CStaticModelObject::Scale(float fScale)
{
	A3DVECTOR3 vScale = m_pModel->pModel->GetScale();
	vScale *= fScale; 
	m_pModel->pModel->SetScale(vScale.x);
}

void CStaticModelObject::SetPos(A3DVECTOR3 vPos)
{
	CSceneObject::SetPos(vPos);
	if(m_pModel)
	{
		if(m_pModel->pModel) m_pModel->pModel->SetPos(m_vPos);
	}
	
}


void CStaticModelObject::ResetRotateMat()
{
	m_pModel->pModel->SetDirAndUp(A3DVECTOR3(0,0,1.0f),A3DVECTOR3(0,1.0f,0));
}
*/

void CStaticModelObject::ReInit()
{
	CSceneObject::ReInit();
	m_pModel = NULL;
	m_pLightMapSizeFuns = new LIGHTMAP_SIZE_FUNCS(this);
	m_pEffectPropertyFuns = new EFFECT_PROPERTY_FUNCS();
}

CSceneObject* CStaticModelObject::CopyObject()
{
	CStaticModelObject *pNewObject = new CStaticModelObject(*this);
	ASSERT(pNewObject);
	pNewObject->ReInit();
	pNewObject->m_pProperty = new ADynPropertyObject();	
	pNewObject->BuildProperty();
	PESTATICMODEL pEM = new ESTATICMODEL();
	ASSERT(pEM);
	pEM->pModel = new CELBuilding();
	ASSERT(pEM->pModel);
	AFile sFile;
	if(!sFile.Open(m_strModelPath,AFILE_OPENEXIST))
	{
		g_Log.Log("CStaticModelObject::CopyObject(),Open file %s failed!",m_strModelPath);
		delete pEM->pModel;
		delete pEM;
		return NULL;
	}else
	{
		if(!pEM->pModel->Load(g_Render.GetA3DDevice(),&sFile))
		{
			g_Log.Log("CStaticModelObject::CopyObject(),Load file %s failed!",m_strModelPath);
			delete pEM->pModel;
			delete pEM;
			return NULL;
		}
	}
	sFile.Close();
	pNewObject->SetA3dModel(pEM);
	
	int StaticModelNum = 0;
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		AString name;
		CSceneObjectManager *pSManager = pMap->GetSceneObjectMan();
		pSManager->ClearSelectedList();
		while(1)
		{
			name.Format("StaticModel_%d",StaticModelNum);
			if(pSManager->FindSceneObject(name)!=NULL)
			{
				StaticModelNum++;
			}else 
			{
				StaticModelNum++;
				break;
			}
		}
		pNewObject->m_nObjectID = pSManager->GenerateObjectID();
		pSManager->InsertSceneObject(pNewObject);
		pSManager->AddObjectPtrToSelected(pNewObject);
		pNewObject->SetObjectName(name);
		pNewObject->SetPos(m_vPos);
		//pNewObject->UpdateProperty(false);
//		AUX_GetMainFrame()->GetToolTabWnd()->ShowPropertyPannel(pNewObject);//把灯光的属性给对象属性表
		AUX_GetMainFrame()->GetToolTabWnd()->UpdatePropertyData(false);//输出属性数据
	}
	return pNewObject;
}

//判断区域是否跟该区域相交
bool CStaticModelObject::IsIntersectionWithArea(const A3DVECTOR3& vMin, const A3DVECTOR3& vMax)
{
	if(m_pModel == NULL) return false;
	if(m_pModel->pModel == NULL) return false;

	A3DVECTOR3 min = m_pModel->pModel->GetModelAABB().Mins;
	A3DVECTOR3 max = m_pModel->pModel->GetModelAABB().Maxs;
	
	if(min.x>vMax.x) return false;
	if(min.z>vMax.z) return false;
	if(max.x<vMin.x) return false;
	if(max.z<vMin.z) return false;
	return true;
}

//	Load data
bool CStaticModelObject::Load(CELArchive& ar,DWORD dwVersion,int iLoadFlags)
{
	if(dwVersion < 0x4e)
	{
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		A3DVECTOR3 vr,vu,vl,vs;
		m_strName = AString(ar.ReadString());
		m_strModelPath = AString(ar.ReadString());

		m_pModel = Aux_GetStaticModelFromTempMap(m_nObjectID);
		if(m_pModel==0)
		{
			AString msg;
			msg.Format("CStaticModelObject::Load(), can't get object ptr from object manager(id = %d)",m_nObjectID);
			g_Log.Log(msg);
			g_LogDlg.Log(CString(msg));
		}
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
		if(!ar.Read(&m_bNoShadow,sizeof(bool)))
			return false;
		if(!ar.Read(&m_bReflect,sizeof(bool)))
			return false;
		if(!ar.Read(&m_bRefraction,sizeof(bool)))
			return false;
		if(!ar.Read(&m_bOptimizeEnable,sizeof(bool)))
			return false;
		if(!ar.Read(&m_bIsTree,sizeof(bool)))
			return false;
		bool bCollideFlags;
		if(!ar.Read(&bCollideFlags,sizeof(bool)))
			return false;
/* repair probrom of model direction
		AString strInfo;
		strInfo.Format("%d", m_nObjectID);
		AUX_GetMainFrame()->pFileTest.WriteLine(strInfo);
		strInfo.Format("vr%.6f, %.6f, %.6f", vr.x, vr.y, vr.z);
		AUX_GetMainFrame()->pFileTest.WriteLine(strInfo);
		strInfo.Format("vu%.6f, %.6f, %.6f", vu.x, vu.y, vu.z);
		AUX_GetMainFrame()->pFileTest.WriteLine(strInfo);
		strInfo.Format("vl%.6f, %.6f, %.6f", vl.x, vl.y, vl.z);
		AUX_GetMainFrame()->pFileTest.WriteLine(strInfo);
		strInfo.Format("vs%.6f, %.6f, %.6f", vs.x, vs.y, vs.z);
		AUX_GetMainFrame()->pFileTest.WriteLine(strInfo);
*/
/*
		AString strInfo;
		float tTest1 = (vr-A3DVECTOR3(.0f, 0, 1.0f)).Magnitude();
		float tTest2 = (vu-A3DVECTOR3(.0f, 1.0f, .0f)).Magnitude();
		float tTest3 = (vl-A3DVECTOR3(1.0f, 0, .0f)).Magnitude();
		float tTest4 = .0f;//(vr-A3DVECTOR3(1.0f, 1.0, 1.0f)).Magnitude();
		if(fabs(tTest1)>0.001 || fabs(tTest2)>0.001 || fabs(tTest3)>0.001 || fabs(tTest4)>0.001)
		{
			strInfo.Format("QuestionModel:%s(%s)", m_strName, m_strModelPath);
			AUX_GetMainFrame()->pFileTest.WriteLine(strInfo);
			strInfo.Format("vr%.6f, %.6f, %.6f", vr.x, vr.y, vr.z);
			AUX_GetMainFrame()->pFileTest.WriteLine(strInfo);
			strInfo.Format("vu%.6f, %.6f, %.6f", vu.x, vu.y, vu.z);
			AUX_GetMainFrame()->pFileTest.WriteLine(strInfo);
			strInfo.Format("vl%.6f, %.6f, %.6f", vl.x, vl.y, vl.z);
			AUX_GetMainFrame()->pFileTest.WriteLine(strInfo);

			if(fabs(tTest4)>0.001)
			{
				strInfo.Format("vs%.6f, %.6f, %.6f", vs.x, vs.y, vs.z);
				AUX_GetMainFrame()->pFileTest.WriteLine(strInfo);
			}
			AUX_GetMainFrame()->pFileTest.WriteLine("");
		}
*/
		
		m_dwCollideFlags = bCollideFlags;
		m_matTrans.SetRow(3,m_vPos);
		m_matRotate.SetRow(2,vr);
		m_matRotate.SetRow(1,vu);
		m_matRotate.SetRow(0,vl);
		m_matScale._11 = vs.x;
		m_matScale._22 = vs.y;
		m_matScale._33 = vs.z;
		UpdateModel();
        SetCollideFlags(m_dwCollideFlags);
    }
	else
    {
        if(!ar.Read(&m_nObjectID,sizeof(int)))
            return false;
        A3DVECTOR3 vr,vu,vl,vs;
        m_strName = AString(ar.ReadString());
        m_strModelPath = AString(ar.ReadString());

        m_pModel = Aux_GetStaticModelFromTempMap(m_nObjectID);
        if(m_pModel==0)
        {
            AString msg;
            msg.Format("CStaticModelObject::Load(), can't get object ptr from object manager(id = %d)",m_nObjectID);
            g_Log.Log(msg);
            g_LogDlg.Log(CString(msg));
        }
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
        if(!ar.Read(&m_bNoShadow,sizeof(bool)))
            return false;
        if(!ar.Read(&m_bReflect,sizeof(bool)))
            return false;
        if(!ar.Read(&m_bRefraction,sizeof(bool)))
            return false;
        if(!ar.Read(&m_bOptimizeEnable,sizeof(bool)))
            return false;
        if(!ar.Read(&m_bIsTree,sizeof(bool)))
            return false;
        if(!ar.Read(&m_dwCollideFlags,sizeof(DWORD)))
            return false;
        if(!ar.Read(&m_bSLAffect,sizeof(bool)))
            return false;
        if(!ar.Read(&m_nCustomLightmapSize,sizeof(int)))
            return false;
        if(!ar.Read(&m_fLitDirectScale,sizeof(float)))
            return false;
        if(!ar.Read(&m_fLitIndirectScale,sizeof(float)))
            return false;
        if(!ar.Read(&m_fLitAOScale,sizeof(float)))
            return false;
        m_strEmissiveMap = AString(ar.ReadString());
        if(!ar.Read(&m_fPower,sizeof(float)))
            return false;
        if(!ar.Read(&m_fRange,sizeof(float)))
            return false;
        if(!ar.Read(&m_fAttenuation0,sizeof(float)))
            return false;
        if(!ar.Read(&m_fAttenuation1,sizeof(float)))
            return false;
        if(!ar.Read(&m_fAttenuation2,sizeof(float)))
            return false;
        if(!ar.Read(&m_nEffectType,sizeof(int)))
            return false;
        if(!ar.Read(&m_nSampleCount,sizeof(int)))
            return false;
        if(!ar.Read(&m_nFeetSound,sizeof(int)))
            return false;
        if(!ar.Read(&m_fLoadRadius,sizeof(float)))
            return false;
        if(!ar.Read(&m_bUseConsistentNormal,sizeof(bool)))
            return false;

        if(!ar.Read(&m_bIgnoreInnerShadow,sizeof(bool)))
            return false;
#ifdef ENABLE_EFFECT_EDIT
        int nStringLen;
        const char* szString;
        szString = ar.ReadString(&nStringLen);
        if(szString != NULL && nStringLen > 0 && strcmp(szString, EFFECT_DESC_BEGIN) == 0) {
            A3DLitModel* pLitModel = m_pModel->pModel->GetA3DLitModel();
            while(strcmp(szString, EFFECT_DESC_BEGIN) == 0)
            {
                int nMesh;
                if( ! ar.Read(&nMesh, sizeof(int))) 
                {
                    return false;
                }

                if(nMesh >= 0 && nMesh < pLitModel->GetNumMeshes()) 
                {
                    A3DLitMesh* pMesh = pLitModel->GetMesh(nMesh);
                    if( ! pMesh)
                    {
                        return false;
                    }

                    AString strHLSLFilename;
                    AString strMacros;
                    szString = ar.ReadString(&nStringLen);
                    if(nStringLen != NULL)
                    {
                        strHLSLFilename = szString;
                    }
                    szString = ar.ReadString(&nStringLen);
                    if(nStringLen != NULL)
                    {
                        strMacros = szString;
                    }
                    A3DTexture* pTexture = pMesh->GetTexturePtr();

                    // 创建Effect对象
                    A3DEffect* pEffect = A3DEffect::CreateHLSLShader(
                        g_Render.GetA3DDevice(), pTexture->GetMapFile(), strHLSLFilename);
                    pEffect->SetMacros(strMacros, NULL);
                    A3DHLSL* pHLSL = pEffect->GetHLSL();

                    int nParams;
                    if( ! ar.Read(&nParams, sizeof(int))) 
                    {
                        return false;
                    }
                    for(int i = 0; i < nParams; i++) 
                    {
                        A3DEffect::MTLPARAM MtlParam;
                        MtlParam.strKey = ar.ReadString(&nStringLen);
                        MtlParam.strValue = ar.ReadString(&nStringLen);

                        const A3DHLSL::CONSTMAPDESC* pConstMapDesc = pHLSL->GetConstMapByName(MtlParam.strKey);
                        if(pConstMapDesc != NULL)
                        {
                            pEffect->SetValueByString(*pConstMapDesc, MtlParam.strValue);
                        }
                    }

                    // 更新对象
                    pMesh->SetTexturePtr(pEffect);
                }
                szString = ar.ReadString(&nStringLen);

            };
        }
#endif // ENABLE_EFFECT_EDIT

/* repair probrom of model direction
		if(AUX_GetMainFrame()->pCInfos[m_nObjectID].bOK)
		{
			CMainFrame::CorrectInfo &pCurInfo = AUX_GetMainFrame()->pCInfos[m_nObjectID];
			vr.x = pCurInfo.r[0];
			vr.y = pCurInfo.r[1];
			vr.z = pCurInfo.r[2];
			vu.x = pCurInfo.u[0];
			vu.y = pCurInfo.u[1];
			vu.z = pCurInfo.u[2];
			vl.x = pCurInfo.l[0];
			vl.y = pCurInfo.l[1];
			vl.z = pCurInfo.l[2];
			vs.x = pCurInfo.s[0];
			vs.y = pCurInfo.s[1];
			vs.z = pCurInfo.s[2];
		}
*/		
        m_matTrans.SetRow(3,m_vPos);
        m_matRotate.SetRow(2,vr);
        m_matRotate.SetRow(1,vu);
        m_matRotate.SetRow(0,vl);
        m_matScale._11 = vs.x;
        m_matScale._22 = vs.y;
        m_matScale._33 = vs.z;
        UpdateModel();
        SetCollideFlags(m_dwCollideFlags);
    }

	m_matAbs = m_matScale * m_matRotate * m_matTrans;

    return true;
}

//	Save data
bool CStaticModelObject::Save(CELArchive& ar,DWORD dwVersion)
{
		A3DVECTOR3 vr,vu,vl,vs;
		vr = m_matRotate.GetRow(2);
		vu = m_matRotate.GetRow(1);
		vl = m_matRotate.GetRow(0);
		vs = A3DVECTOR3(m_matScale._11,m_matScale._22,m_matScale._33);
		ar.Write(&m_nObjectID,sizeof(int));
		ar.WriteString(m_strName);
		ar.WriteString(m_strModelPath);
		
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
		if(!ar.Write(&m_bNoShadow,sizeof(bool)))
			return false;
		if(!ar.Write(&m_bReflect,sizeof(bool)))
			return false;
		if(!ar.Write(&m_bRefraction,sizeof(bool)))
			return false;
		if(!ar.Write(&m_bOptimizeEnable,sizeof(bool)))
			return false;
		if(!ar.Write(&m_bIsTree,sizeof(bool)))
			return false;
		if(!ar.Write(&m_dwCollideFlags,sizeof(DWORD)))
			return false;
		if(!ar.Write(&m_bSLAffect,sizeof(bool)))
			return false;
		if(!ar.Write(&m_nCustomLightmapSize,sizeof(int)))
			return false;

		if(!ar.Write(&m_fLitDirectScale,sizeof(float)))
			return false;
		if(!ar.Write(&m_fLitIndirectScale,sizeof(float)))
			return false;
		if(!ar.Write(&m_fLitAOScale,sizeof(float)))
			return false;
		ar.WriteString(m_strEmissiveMap);
		if(!ar.Write(&m_fPower,sizeof(float)))
			return false;
		if(!ar.Write(&m_fRange,sizeof(float)))
			return false;
		if(!ar.Write(&m_fAttenuation0,sizeof(float)))
			return false;
		if(!ar.Write(&m_fAttenuation1,sizeof(float)))
			return false;
		if(!ar.Write(&m_fAttenuation2,sizeof(float)))
			return false;
		if(!ar.Write(&m_nEffectType,sizeof(int)))
			return false;
		if(!ar.Write(&m_nSampleCount,sizeof(int)))
			return false;
		if(!ar.Write(&m_nFeetSound,sizeof(int)))
			return false;
		if(!ar.Write(&m_fLoadRadius,sizeof(float)))
			return false;
        if(!ar.Write(&m_bUseConsistentNormal, sizeof(bool)))
            return false;
        if(!ar.Write(&m_bIgnoreInnerShadow, sizeof(bool)))
            return false;

#ifdef ENABLE_EFFECT_EDIT
		A3DLitModel* pLitModel = m_pModel->pModel->GetA3DLitModel();
		int nNumMeshs = pLitModel->GetNumMeshes();
		for(int i = 0; i < nNumMeshs; i++)
		{
			A3DLitMesh* pLitMesh = pLitModel->GetMesh(i);
			A3DTexture* pTexture = pLitMesh->GetTexturePtr();
			if(pTexture->GetClassID() == A3D_CID_HLSLSHADER) {
				A3DEffect* pEffect = static_cast<A3DEffect*>(pTexture);
				ar.WriteString(EFFECT_DESC_BEGIN);
				ar.Write(&i, sizeof(int));
				ar.WriteString(pEffect->GetHLSLFilename());

				AString strMacros;
				A3DHLSL* pHLSL = pEffect->GetHLSL();
				if(pHLSL != NULL) {
					pHLSL->GetMacroAdapter(strMacros);
				}
				ar.WriteString(strMacros);
				A3DEffect::ParamArray aParams;
				pEffect->GenerateParam(aParams);	// 这个地方会取到很多绑定数据,有冗余
				int nParams = (int)aParams.size();
				ar.Write(&nParams, sizeof(int));
				for(A3DEffect::ParamArray::iterator it = aParams.begin();
					it != aParams.end(); ++it) {
					ar.WriteString(it->strKey);
					ar.WriteString(it->strValue);
				}
			}
		}
		ar.WriteString(EFFECT_DESC_END);
#endif // ENABLE_EFFECT_EDIT
	return true;
}

void CStaticModelObject::QueryRef(ElementResMan *pResMan)
{
	/*
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	CString strRefName = g_Configs.szCurProjectName;
	strRefName +="\\";
	strRefName +=pMap->GetMapName();
	strRefName = strRefName + "..";
	strRefName = strRefName + CString(m_strName);
	if(m_pModel) pResMan->AddReference(ElementResMan::RES_TYPE_BUILDING,m_strModelPath,strRefName);
	*/
}

//把静态模型设置成只有碰撞没有显示的特殊模型
void CStaticModelObject::SetCollideFlags(DWORD bCollide)
{
	if(m_pModel)
	{
		if(m_pModel->pModel)
		{
			m_pModel->pModel->SetCollideOnly(bCollide == 1);
			//if(bCollide == 2)
			//	m_pModel->pModel->SetChfPath(NULL);
		}
	}
	m_dwCollideFlags = bCollide;
}

DWORD CStaticModelObject::GetCollideFlags()
{
	return m_dwCollideFlags;
}

int	CStaticModelObject::GetTriangleNum()
{
	if(m_pModel)
	{
		if(m_pModel->pModel)
		{
			if(m_pModel->pModel->GetCollideOnly()) return 0;
			A3DLitModel * pLitModel = m_pModel->pModel->GetA3DLitModel();
			int nNumMeshes = pLitModel->GetNumMeshes();
			int nFaces = 0;
			for(int idMesh=0; idMesh<nNumMeshes; idMesh++)
			{
				A3DLitMesh* pMesh = pLitModel->GetMesh(idMesh);
				nFaces += pMesh->GetNumFaces();
			}
			return nFaces;
		}
	}
	return 0;
}

int	CStaticModelObject::GetMeshNum()
{
	if(m_pModel)
	{
		if(m_pModel->pModel)
		{
			if(m_pModel->pModel->GetCollideOnly()) return 0;
			A3DLitModel * pLitModel = m_pModel->pModel->GetA3DLitModel();
			return pLitModel->GetNumMeshes();
		}
	}
	return 0;
}

void CStaticModelObject::GetLitScales(float* fOutDirect, float* fOutIndirect, float* fOutAO)
{
	*fOutDirect = m_fLitDirectScale;
	*fOutIndirect = m_fLitIndirectScale;
	*fOutAO = m_fLitAOScale;
}

void CStaticModelObject::GetEmissiveData(LIGHTDATA* pOutData, AString* pstrMapFile, int* nSamples)
{
	pOutData->m_fRange = m_fRange;
	pOutData->m_fColorScale = m_fPower;
	pOutData->m_fAttenuation0 = m_fAttenuation0;
	pOutData->m_fAttenuation1 = m_fAttenuation1;
	pOutData->m_fAttenuation2 = m_fAttenuation2;
	pOutData->m_nEffectType = m_nEffectType;
	*pstrMapFile = m_strEmissiveMap;
	*nSamples = m_nSampleCount;
}

float CStaticModelObject::ComputeVolume() const
{
    if (m_pModel && m_pModel->pModel)
    {
        A3DVECTOR3 vVolume = m_Obb.Extents;
        vVolume *= m_pModel->pModel->GetScale().x;
        return vVolume.x * vVolume.y * vVolume.z;
    }
    else
    {
        return 0;
    }
}

int CStaticModelObject::GetAdaptLightmapSize(float fScale) const
{
    float fSize = sqrtf(ComputeModelArea() * fScale);

    if (fSize < 20.0f) return 1;
    if (fSize < 40) return 64;
    if (fSize <= 128.0f) return 128;
    if (fSize <= 256.0f) return 256;
    return 512;
    //if (fSize <= 600.0f) return 512;
    //return 1024;
}

float CStaticModelObject::GetLightmapRate() const
{
    if (m_pModel == NULL || m_pModel->pModel == NULL)
        return 0;
    
    float fRate = 0;
    for (int iMesh = 0; iMesh < m_pModel->pModel->GetA3DLitModel()->GetNumMeshes(); iMesh++)
    {
        A3DLitMesh* pMesh = m_pModel->pModel->GetA3DLitModel()->GetMesh(iMesh);
        A3DLIGHTMAPCOORD* pCoord = pMesh->GetLightMapCoords();
        if (pCoord == NULL)
            continue;
        WORD* pIndices = pMesh->GetIndices();
        for (int iFace = 0; iFace < pMesh->GetNumFaces(); iFace++)
        {
            A3DVECTOR3 v1(pCoord[pIndices[iFace * 3 + 0]].u, pCoord[pIndices[iFace * 3 + 0]].v, 0);
            A3DVECTOR3 v2(pCoord[pIndices[iFace * 3 + 1]].u, pCoord[pIndices[iFace * 3 + 1]].v, 0);
            A3DVECTOR3 v3(pCoord[pIndices[iFace * 3 + 2]].u, pCoord[pIndices[iFace * 3 + 2]].v, 0);

            A3DVECTOR3 dv1 = v1 - v2;
            A3DVECTOR3 dv2 = v1 - v3;
            A3DVECTOR3 dv3 = CrossProduct(dv1, dv2);
            fRate += dv3.Magnitude();
        }
    }
    fRate = fRate / 2;

    return fRate;
}

A3DAABB CStaticModelObject::GetRealAABB() const
{
    if (m_pModel == NULL || m_pModel->pModel == NULL)
        return A3DAABB(0, 0);

    A3DAABB aabb;
    const A3DMATRIX4& matModel = m_pModel->pModel->GetA3DLitModel()->GetAbsoluteTM();
    aabb.Clear();
    for (int iMesh = 0; iMesh < m_pModel->pModel->GetA3DLitModel()->GetNumMeshes(); iMesh++)
    {
        A3DLitMesh* pMesh = m_pModel->pModel->GetA3DLitModel()->GetMesh(iMesh);
        A3DLMVERTEX* pVerts = pMesh->GetVerts();
        if (pVerts == NULL)
            continue;
        for (int iVert = 0; iVert < pMesh->GetNumVerts(); iVert++)
        {
            A3DVECTOR3 vPos = matModel * pVerts[iVert].pos;
            aabb.AddVertex(vPos);
        }
    }
    aabb.CompleteCenterExts();
    return aabb;
}

int CStaticModelObject::GetConvexHullFaceNum() const
{
	if(!m_pModel)
		return 0;
	if(!m_pModel->pModel)
		return 0;
	return 0;
//	return m_pModel->pModel->GetConvexHullFaceNum();
}

int CStaticModelObject::GetFaceCount() const
{
    if (!m_pModel)
        return 0;
    if (!m_pModel->pModel)
        return 0;
    if (!m_pModel->pModel->GetA3DLitModel())
        return 0;
    A3DLitModel* pModel = m_pModel->pModel->GetA3DLitModel();
    int nCount = 0;
    for (int i = 0; i < pModel->GetNumMeshes(); i++)
    {
        nCount += pModel->GetMesh(i)->GetNumVerts();
    }
    return nCount;

}

int CStaticModelObject::GetMeshCount() const
{
    if (!m_pModel)
        return 0;
    if (!m_pModel->pModel)
        return 0;
    if (!m_pModel->pModel->GetA3DLitModel())
        return 0;
    A3DLitModel* pModel = m_pModel->pModel->GetA3DLitModel();
    return pModel->GetNumMeshes();
    
}

BOOL CALLBACK LIGHTMAP_SIZE_FUNCS::OnActivate(void)
{
    CLightMapSizeDlg dlg;
    dlg.m_nSize = m_pOwner->m_nCustomLightmapSize;
    if (dlg.DoModal() == IDOK)
    {
        m_pOwner->m_nCustomLightmapSize = dlg.m_nSize;
        OnSetValue(0);
    }
    return TRUE;
}

LPCTSTR CALLBACK LIGHTMAP_SIZE_FUNCS::OnGetShowString(void) const
{
    return m_strText;
}

AVariant CALLBACK LIGHTMAP_SIZE_FUNCS::OnGetValue(void) const
{
    return m_var;
}

void CALLBACK LIGHTMAP_SIZE_FUNCS::OnSetValue(const AVariant& var)
{
    m_var = var;
    int nSize = m_pOwner->m_nCustomLightmapSize;
    if (nSize < 0)
        m_strText.Format("未设置");
    else if (nSize < 64)
        m_strText.Format("使用顶点光");
    else
        m_strText.Format("%d x %d", nSize, nSize);
}


//////////////////////////////////////////////////////////////////////////
void EFFECT_PROPERTY_FUNCS::SetModel(A3DLitModel* pA3DLitModel)
{
	m_pA3DLitModel = pA3DLitModel;
}
BOOL CALLBACK EFFECT_PROPERTY_FUNCS::OnActivate(void)
{	
#ifdef _ANGELICA22
	CDlgLitModelMtlProperty dlg;
	dlg.SetLitModel(m_pA3DLitModel);
	if (dlg.DoModal() == IDOK)
	{
	}
#endif
	return TRUE;
}

LPCTSTR CALLBACK EFFECT_PROPERTY_FUNCS::OnGetShowString(void) const
{
	return m_strText;
}

AVariant CALLBACK EFFECT_PROPERTY_FUNCS::OnGetValue(void) const
{
	return m_var;
}

void CALLBACK EFFECT_PROPERTY_FUNCS::OnSetValue(const AVariant& var)
{
	m_var = var;
}
