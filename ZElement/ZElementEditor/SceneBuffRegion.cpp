// SceneBuffRegion.cpp: implementation of the CSceneBuffRegion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "elementeditor.h"
#include "SceneBuffRegion.h"
#include "MainFrm.h"
#include "SceneObjectManager.h"
#include "BezierSelDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void init_buff_region_data( BUFF_REGION_DATA& dat)
{
//	dat.strBuffRegion = "";
	dat.nRegionTemplID = 1;
	dat.nMonsterID = 0;
	dat.nPathID = 0;
	dat.strPathName = "";
}

CSceneBuffRegion::CSceneBuffRegion()
{
	init_buff_region_data(m_Data);
	m_nObjectType = SO_TYPE_BUFF_REGION;
	m_pBIDSelFuncs = new BEZIER_ID_FUNCS3;
	m_nCopy = 0;
	
	BuildProperty();
}

CSceneBuffRegion::~CSceneBuffRegion()
{
	if(m_pBIDSelFuncs)
	{
		delete m_pBIDSelFuncs;
		m_pBIDSelFuncs = NULL;
	}

}

void CSceneBuffRegion::UpdateProperty(bool bGet)
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
				m_pProperty->SetPropVal(1,m_Data.nRegionTemplID);
				m_pProperty->SetPropVal(2,m_Data.nMonsterID);
				m_pBIDSelFuncs->OnSetValue(m_Data.strPathName);
				m_pProperty->SetPropVal(4,m_nCopy);
				AUX_GetMainFrame()->GetToolTabWnd()->UpdatePropertyData(false);
				return;
			}
		}
		m_strName	        = temp;
		m_Data.nRegionTemplID	= m_pProperty->GetPropVal(1);
		m_Data.nMonsterID		= m_pProperty->GetPropVal(2);
		m_nCopy					= m_pProperty->GetPropVal(4);
	}else
	{
		m_pProperty->SetPropVal(0,m_strName);
		m_pProperty->SetPropVal(1,m_Data.nRegionTemplID);
		m_pProperty->SetPropVal(2,m_Data.nMonsterID);
		m_pBIDSelFuncs->OnSetValue(m_Data.strPathName);
		m_pProperty->SetPropVal(4,m_nCopy);
	}
}

void CSceneBuffRegion::BuildProperty()
{
	if(m_pProperty)
	{
		m_pProperty->DynAddProperty(AVariant(m_strName), "名称");
		m_pProperty->DynAddProperty(AVariant(m_Data.nRegionTemplID), "区域模板ID");
		m_pProperty->DynAddProperty(AVariant(m_Data.nMonsterID), "关联怪物ID");
		m_pProperty->DynAddProperty(AVariant(m_Data.strPathName), "关联曲线", (ASet*)m_pBIDSelFuncs, NULL, WAY_CUSTOM);
		m_pProperty->DynAddProperty(AVariant(m_nCopy), "第几套数据");
	}
}

void CSceneBuffRegion::Translate(A3DVECTOR3 vDelta)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap==NULL) return;
	if(!pMap->GetTerrain()) return;

	m_vPos += vDelta;
	m_vPos.y = pMap->GetTerrain()->GetPosHeight(m_vPos) + 0.1f;

	m_matTrans.SetRow(3,m_vPos);
	m_matAbs = m_matScale * m_matRotate * m_matTrans;
}

//Load data
bool CSceneBuffRegion::Load(CELArchive& ar,DWORD dwVersion,int iLoadFlags)
{
	if(dwVersion >= 0x4b)
	{
		m_strName = ar.ReadString();
		m_Data.strPathName = ar.ReadString();

		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nRegionTemplID,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nMonsterID,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nPathID,sizeof(int)))
			return false;
		if(!ar.Read(&m_nCopy,sizeof(int)))
			return false;
	}
	return true;
}

//Save data
bool CSceneBuffRegion::Save(CELArchive& ar,DWORD dwVersion)
{
	ar.WriteString(m_strName);	
	ar.WriteString(m_Data.strPathName);
	
	if(!ar.Write(&m_nObjectID,sizeof(int)))
		return false;
	if(!ar.Write(&m_Data.nRegionTemplID,sizeof(int)))
		return false;
	if(!ar.Write(&m_Data.nMonsterID,sizeof(int)))
		return false;
	if(!ar.Write(&m_Data.nPathID,sizeof(int)))
		return false;
	if(!ar.Write(&m_nCopy,sizeof(int)))
		return false;	
	return true;
}

BOOL CALLBACK BEZIER_ID_FUNCS3::OnActivate(void)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(pManager->m_listSelectedObject.GetCount()==1)
		{
			CSceneObject *pObj = (CSceneObject*)pManager->m_listSelectedObject.GetHead();
			if(pObj->GetObjectType()==CSceneObject::SO_TYPE_BUFF_REGION)
			{
				CBezierSelDlg dlg;
				BUFF_REGION_DATA data = ((CSceneBuffRegion*)pObj)->GetProperty();
				dlg.m_nBezierID = data.nPathID;
				dlg.m_nObjectType = CSceneObject::SO_TYPE_BEZIER;
				ASSERT(pObj);
				if(dlg.DoModal()==IDOK)
				{
					m_strText = dlg.m_strBezierName;
					data.nPathID = dlg.m_nBezierID;
					data.strPathName = dlg.m_strBezierName;
					((CSceneBuffRegion*)pObj)->SetProperty(data);
				}
			}
		}
	}
	
	return TRUE;
}

LPCTSTR CALLBACK BEZIER_ID_FUNCS3::OnGetShowString(void) const
{
	return m_strText;
}

AVariant CALLBACK BEZIER_ID_FUNCS3::OnGetValue(void) const
{
	return m_var;
}

void CALLBACK BEZIER_ID_FUNCS3::OnSetValue(const AVariant& var)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(pManager->m_listSelectedObject.GetCount()==1)
		{
			CSceneObject* pObj = (CSceneObject*)pManager->m_listSelectedObject.GetHead();
			if(pObj->GetObjectType()==CSceneObject::SO_TYPE_BUFF_REGION)
			{
				BUFF_REGION_DATA data = ((CSceneBuffRegion*)pObj)->GetProperty();
				m_strText = data.strPathName;
			}
		}
	}
}