
#ifdef BACK_VERSION
#include "EC_Viewport.h"
#else
#include "ECViewport.h"
#endif
#include "EC_Global.h"
#include "EC_LandBuild.h"
#include "EC_Game.h"
#include "EC_Model.h"
#include "EC_Utility.h"
#include "EC_PateText.h"
#include "EC_SceneLoader.h"
#include "EC_HostPlayer.h"
#include "EC_Game.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "EC_GameRun.h"
#include "EC_ManPlayer.h"
#include "EC_World.h"
#include "EC_FixedMsg.h"
#include "EC_GPDataType.h"
#include "EC_CDR.h"
#include "EC_Player.h"
#include "DlgBase.h"
#include "elementdataman.h"
#include "EL_Region.h"
#include <A3DViewport.h>
#define new A_DEBUG_NEW

EC_LandBuild::EC_LandBuild() 
{
	m_iCID				= OCID_INTERACTOR;
	m_pDBEssence		= NULL;
	m_pModel = NULL;
	ppBrushes = NULL;
	nBrushes = 0;
	m_pSkin = NULL;
}

EC_LandBuild::~EC_LandBuild()
{

}


 void EC_LandBuild::LoadGuildBuildingObject(int nGuiBuildTempID,int nStatus,int nLevel)
{
	DATA_TYPE DataType;
	Release();
	m_pDBEssence = (BUILDING_ESSENCE *)g_pGame->GetElementDataMan()->
		get_data_ptr(nGuiBuildTempID, ID_SPACE_ESSENCE, DataType);
	if(m_pDBEssence)
	{
		m_pModel = new CECModel;
		char szModelFile[MAX_PATH];
		if(nStatus == 0)
		{
			strcpy(szModelFile,  g_pGame->GetDataPath(m_pDBEssence->constructing_model));
		}
		else
		{
			strcpy(szModelFile,  g_pGame->GetDataPath(m_pDBEssence->up_building[nLevel].building_model));
		}
		g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile((const char*)glb_ReplaceShader, A3DSkinMan::SHADERREPLACE_USERDEFINE);
		if(!m_pModel->Load(szModelFile, true, A3DSkinModel::LSF_NOSKIN, true))
		{
			g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
			a_LogOutput(1, "EC_LandBuild::LoadModel, Failed to load Build model %d", m_pDBEssence->constructing_model);
			m_pModel->Load("models\\error\\error.ecm");
			return;
		}
		//m_pModel->SetAutoUpdateFlag(false);
		//m_pModel->SetTransparent(-1.0f);
		char szSkin[ MAX_PATH ];
		strncpy(szSkin, szModelFile, MAX_PATH);
		glb_ChangeExtension(szSkin, "ski");

		A3DSkinMan* pSkinMan = 	g_pGame->GetA3DEngine()->GetA3DSkinMan();
		m_pSkin = pSkinMan->LoadSkinFile( szSkin );
		if( ! m_pSkin )
		{
			a_LogOutput( 1, "CECNPCLoader::LoadNPCModel, Failed to load skin %s", szSkin );
			m_pSkin = pSkinMan->LoadSkinFile( "Models\\error\\error.ski" );
			if( ! m_pSkin )
			{
				A3DRELEASE( m_pModel );
				g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
				return;
			}
		}
		A3DSkinModel* pSkinModel = m_pModel->GetA3DSkinModel();
		if( ! pSkinModel )
		{
			g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0); 
			return;
		}
		m_pModel->SetAABBType( CECModel::AABB_INITMESH );
		m_pModel->AddSkin( m_pSkin, false );
		g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
	}
}

 void  EC_LandBuild::LandBuildComplete(int nGuiBuildTempID,int nLevel,int nStatus)
 {
	 DATA_TYPE DataType;
	 m_pDBEssence = (BUILDING_ESSENCE *)g_pGame->GetElementDataMan()->
		 get_data_ptr(nGuiBuildTempID, ID_SPACE_ESSENCE, DataType);
	 if(m_pDBEssence)
	 {
		Release();
		 m_pModel = new CECModel;
		 char szModelFile[MAX_PATH];
		 if(nStatus == 0)
		 {
			 strcpy(szModelFile,  g_pGame->GetDataPath(m_pDBEssence->constructing_model));
		 }
		 else
		 {
			 strcpy(szModelFile, g_pGame->GetDataPath(m_pDBEssence->up_building[nLevel].building_model));
		 }
	
		 if(!m_pModel->Load(szModelFile, true, A3DSkinModel::LSF_NOSKIN, true))
		 {
			 g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
			 a_LogOutput(1, "EC_LandBuild::LoadModel, Failed to load Build model %d", m_pDBEssence->up_building[nLevel].building_model);
			 m_pModel->Load("models\\error\\error.ecm");
			 return;
		 }
		 m_pModel->SetAutoUpdateFlag(false);
		 m_pModel->SetTransparent(-1.0f);
		 char szSkin[ MAX_PATH ];
		 strncpy(szSkin, szModelFile, MAX_PATH);
		 glb_ChangeExtension(szSkin, "ski");

		 A3DSkinMan* pSkinMan = 	g_pGame->GetA3DEngine()->GetA3DSkinMan();
		 m_pSkin = pSkinMan->LoadSkinFile( szSkin );
		 if( ! m_pSkin )
		 {
			 a_LogOutput( 1, "CECNPCLoader::LoadNPCModel, Failed to load skin %s", szSkin );
			 m_pSkin = pSkinMan->LoadSkinFile( "Models\\error\\error.ski" );
			 if( ! m_pSkin )
			 {
				 A3DRELEASE( m_pModel );
				 g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
				 return;
			 }
		 }
		 A3DSkinModel* pSkinModel = m_pModel->GetA3DSkinModel();
		 if( ! pSkinModel )
		 {
			 g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0); 
			 return;
		 }
		 m_pModel->SetAABBType( CECModel::AABB_INITMESH );
		 m_pModel->AddSkin( m_pSkin, false );
		 SetFieldIndex(m_nFieldIndex);
	 }
 }

 void EC_LandBuild::SetFieldIndex(int val) 
 {
	 m_nFieldIndex = val;
	 DATA_TYPE DataType;
	 BUILDING_REGION_CONFIG *m_pEssence = (BUILDING_REGION_CONFIG *)g_pGame->GetElementDataMan()->
		 get_data_ptr(805, ID_SPACE_CONFIG, DataType);
	 int nTempID = 0;
	 for(int nIndex = 0; nIndex  < 30; nIndex++)
	 {
		 if(m_nFieldIndex == m_pEssence->build_region[nIndex].region_index && m_pEssence->build_region[nIndex].region_index != 0)
		{
				nTempID = m_pEssence->build_region[nIndex].region_id;
				break;
		 }
	 }
	 int nNum = g_pGame->GetGameRun()->GetWorld()->GetRegionSet()->GetGuildBuildingNum();
	for(int nIndex = 0; nIndex < nNum; nIndex++)
	{
		CELGuildBuilding*pBuildInfo = g_pGame->GetGameRun()->GetWorld()->GetRegionSet()->GetGuildBuilding(nIndex);
		if(pBuildInfo && pBuildInfo->m_iTmplId == nTempID)
		{
			m_pModel->SetPos(A3DVECTOR3(pBuildInfo->m_vPos.x,pBuildInfo->m_vPos.y,pBuildInfo->m_vPos.z));
			A3DVECTOR3 vDir = A3DVECTOR3(pBuildInfo->m_vDir.x,pBuildInfo->m_vDir.y,pBuildInfo->m_vDir.z);
			A3DVECTOR3 vUp = A3DVECTOR3(pBuildInfo->m_vUp.x,pBuildInfo->m_vUp.y,pBuildInfo->m_vUp.z);
			m_pModel->SetDirAndUp(vDir, vUp);
			if( m_pModel && m_pModel->HasCHAABB() )
			{
				ConvexHullDataArray& cdArray = m_pModel->GetConvexHullData();

				A3DMATRIX4 matTM = TransformMatrix(vDir, vUp, A3DVECTOR3(pBuildInfo->m_vPos.x,pBuildInfo->m_vPos.y,pBuildInfo->m_vPos.z));
				if( int(cdArray.size()) != nBrushes )
				{
					nBrushes = cdArray.size();
					ppBrushes = new CCDBrush *[cdArray.size()];

					for(int i=0; i<nBrushes; i++)
					{
						CConvexHullData chData = *cdArray[i];
						chData.Transform(matTM);

						CQBrush qBrush;
						qBrush.AddBrushBevels(&chData);
						CCDBrush * pCDBrush = new CCDBrush();
						qBrush.Export(pCDBrush);

						ppBrushes[i] = pCDBrush;
					}
				}
			}
			break;
		}
	}
 }

 void EC_LandBuild::SetModelShow(bool isShow)
 {
	 if(m_pModel)
	 {
		 m_pModel->Show(isShow);
	 }
 }

 A3DVECTOR3 EC_LandBuild::GetModelPos()
 {
	 if(m_pModel)
	 {
		return m_pModel->GetPos();
	 }
	 return A3DVECTOR3(0);
 }

 bool EC_LandBuild::Tick(DWORD dwDeltaTime)
 {
	 CECObject::Tick(dwDeltaTime);
	 if(m_pModel)
	 {
		 m_pModel->Tick(dwDeltaTime);
	 }
	 return true;
 }

bool EC_LandBuild::Render(CECViewport* pViewport, int iRenderFlag)
{
	if(m_pModel)
	{
		m_pModel->Render(pViewport->GetA3DViewport());
	}
	return true;
}
void EC_LandBuild::Release()
{
	for (int i = 0; i < nBrushes; i++)
	{
		delete ppBrushes[i];
		ppBrushes[i] = NULL;
	}
	delete[] ppBrushes;
	nBrushes = 0;
	if(m_pModel)
	{
		m_pModel->Release();
		delete m_pModel;
		m_pModel = NULL;
	}
	if(m_pSkin)
	{
		g_pGame->ReleaseA3DSkin(m_pSkin, true);
		m_pSkin = NULL;
	}
}

bool EC_LandBuild::TraceWithBrush(BrushTraceInfo * pInfo)
{
	if( !m_pModel || !m_pModel->HasCHAABB() )
		return false;

	// Expand the AABB of the model to handle the case that
	// convex hulls of the model protrude the AABB!
	const A3DAABB& aabb = m_pModel->GetCHAABB();

	A3DVECTOR3 vPoint, vNormal;
	float		fFraction;		//	Fraction

	//if (pInfo->bIsPoint && !CLS_RayToAABB3(pInfo->vStart, pInfo->vDelta, aabb.Mins, aabb.Maxs, vPoint, &fFraction, vNormal ) ) {
	//	return false;
	//}

	//if (!pInfo->bIsPoint && !CLS_AABBAABBOverlap(aabb.Center, aabb.Extents, pInfo->BoundAABB.Center, pInfo->BoundAABB.Extents) ) {
	//	return false;
	//}

	//save original result
	bool		bStartSolid = pInfo->bStartSolid;	//	Collide something at start point
	bool		bAllSolid = pInfo->bAllSolid;		//	All in something
	int			iClipPlane = pInfo->iClipPlane;		//	Clip plane's index

	fFraction = 100.0f;
	vNormal = pInfo->ClipPlane.GetNormal(); //clip plane normal
	float       fDist = pInfo->ClipPlane.GetDist();	//clip plane dist

	// Detect the collision with Convex Hull
	bool bCollide = false;
	for(int i=0;i<nBrushes;i++)
	{
		if (ppBrushes[i]->Trace(pInfo) && (pInfo->fFraction < fFraction)) {
			//update the saving info
			bStartSolid = pInfo->bStartSolid;
			bAllSolid = pInfo->bAllSolid;
			iClipPlane = pInfo->iClipPlane;
			fFraction = pInfo->fFraction;
			vNormal = pInfo->ClipPlane.GetNormal();
			fDist = pInfo->ClipPlane.GetDist();

			bCollide = true;
		}
	}	
	if (bCollide) {
		pInfo->bStartSolid = bStartSolid;
		pInfo->bAllSolid = bAllSolid;
		pInfo->iClipPlane = iClipPlane;
		pInfo->fFraction = fFraction;
		pInfo->ClipPlane.SetNormal(vNormal);
		pInfo->ClipPlane.SetD(fDist);
	}
	return bCollide;
}
