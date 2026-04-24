#include "Global.h"
#include "HostPlayer.h"
#include "EC_Model.h"
#include "A3DViewport.h"
#include "MainFrm.h"
#include "ElementMap.h"
#include "Render.h"
#include "A3DSkinMan.h"
#include "Box3D.h"
#include "SceneAIGenerator.h"
#include "SceneObjectManager.h"

CHostPlayer::CHostPlayer(void)
{
	m_bStart = false;
	m_fCamDist = 30.0f;
}

CHostPlayer::~CHostPlayer(void)
{
}

bool CHostPlayer::Init(char* strName)
{
	m_pPlayer = new CECModel;
	bool bRet = m_pPlayer->Load(strName,false,A3DSkinModel::LSF_NOSKIN,true,false);
	AString strSkin(strName);
	strSkin.CutRight(3);
	strSkin += "ski";

	A3DSkinMan* pSkinMan = g_Render.GetA3DEngine()->GetA3DSkinMan();
	A3DSkin* pSkin = pSkinMan->LoadSkinFile(strSkin);
	int iSkin;
	if (pSkin)
	{
		iSkin = m_pPlayer->GetA3DSkinModel()->AddSkin(pSkin,true);
	}
	m_pPlayer->GetA3DSkinModel()->SetPos(A3DVECTOR3(297, 130, 227.2));
	m_pPlayer->ScaleAllRootBonesAndGfx(0.8);

	return bRet;
}

void CHostPlayer::Release()
{
	m_pPlayer->Release();
	delete m_pPlayer;
}

bool CHostPlayer::Tick(int iDeltaTime)
{
	if (!m_bStart)
	{
		return true;
	}

	CViewFrame *pFrame = AUX_GetMainFrame()->GetViewFrame();
	CRenderWnd *pRWnd = pFrame->GetRenderWnd();
	if (pRWnd->GetActiveViewport()!=VIEW_PERSPECTIVE)
		return true;

	// 更新host位置
	A3DVECTOR3 camDir = g_Render.GetA3DEngine()->GetActiveViewport()->GetCamera()->GetDir();
	A3DVECTOR3 camRight = g_Render.GetA3DEngine()->GetActiveViewport()->GetCamera()->GetRight();
	camDir.y = .0f;
	camDir.Normalize();
	camRight.y = .0f;
	camRight.Normalize();
	A3DVECTOR3 deltaTick(0);
	if (GetAsyncKeyState('W')&0x8000)
	{
		deltaTick += camDir * iDeltaTime * g_Configs.fCameraMove / 1000.0f;
	}
	else if (GetAsyncKeyState('S')&0x8000)
	{
		deltaTick -= camDir * iDeltaTime * g_Configs.fCameraMove / 1000.0f;
	}
	if (GetAsyncKeyState('A')&0x8000)
	{
		deltaTick -= camRight * iDeltaTime * g_Configs.fCameraMove / 1000.0f;
	}
	else if (GetAsyncKeyState('D')&0x8000)
	{
		deltaTick += camRight * iDeltaTime * g_Configs.fCameraMove / 1000.0f;
	}
	m_vPos += deltaTick;

	ClipHostPos();

	// 保持角色在地面上
	m_vPos.y = GetStandHeight(m_vPos);
	//const A3DAABB& chAABB = m_pPlayer->GetModelAABB();
	m_pPlayer->SetPos(m_vPos); //A3DVECTOR3(m_vPos.x, y+chAABB.Maxs.y-chAABB.Mins.y, m_vPos.z));
	//m_pPlayer->SetPos(m_vPos);
	m_pPlayer->GetA3DSkinModel()->SetDirAndUp(g_vAxisX, g_vAxisY);
	m_pPlayer->Tick(iDeltaTime);

	// 更新摄像机位置
	camDir = g_Render.GetA3DEngine()->GetActiveViewport()->GetCamera()->GetDir();
	camDir.Normalize();
	A3DVECTOR3 camPos = m_vPos - m_fCamDist*camDir;
	g_Render.GetA3DEngine()->GetActiveViewport()->GetCamera()->SetPos(camPos);

	return true;
}

float CHostPlayer::GetStandHeight(A3DVECTOR3 vPos)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	CTerrain *pTerrain =  pMap->GetTerrain();
	float hRet = pTerrain ->GetHeightAt(m_vPos.x, m_vPos.z);
	CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
	A3DVECTOR3 vStart = vPos + A3DVECTOR3(.0f, 5.0f, .0f);
	A3DVECTOR3 vEnd = vPos + A3DVECTOR3(.0f, -1000.0f, .0f);
	A3DVECTOR3 vInter;
	if(pManager->GetTracePos(vStart, vEnd, vInter))
	{
		if (vInter.y>hRet)
		{
			hRet = vInter.y;
		}
	}

	return hRet;
}

void CHostPlayer::ClipHostPos()
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	CTerrain *pTerrain =  pMap->GetTerrain();
	float fSize = pTerrain->GetTerrainSize();
	if(m_vPos.x>fSize/2)
		m_vPos.x = fSize/2 - 1.0f;
	if(m_vPos.x<-fSize/2)
		m_vPos.x = -fSize/2 + 1.0f;
	if(m_vPos.z>fSize/2)
		m_vPos.z = fSize/2 - 1.0f;
	if(m_vPos.z<-fSize/2)
		m_vPos.z = -fSize/2 + 1.0f;
}

bool CHostPlayer::Render(A3DViewport* pA3DViewport)
{

	CViewFrame *pFrame = AUX_GetMainFrame()->GetViewFrame();
	CRenderWnd *pRWnd = pFrame->GetRenderWnd();
	if (pRWnd->GetActiveViewport()!=VIEW_PERSPECTIVE)
		return true;

	if (m_bStart)
	{
		m_pPlayer->Render(pA3DViewport);
	}
/* 测试代码，显示hostplayer位置
	A3DVECTOR3 vScreenPos;
	g_Render.GetA3DEngine()->GetActiveViewport()->Transform(m_vPos, vScreenPos);
	if(g_Render.GetA3DEngine()->GetActiveCamera()->GetWorldFrustum()->PointInFrustum(m_vPos))
	{
		AString strPos;
		strPos.Format("Pos:%.3f, %.3f, %.3f", m_vPos.x+512, m_vPos.y, m_vPos.z+512);
		g_Render.TextOut(vScreenPos.x,vScreenPos.y,strPos,A3DCOLORRGB(255,255,0));
	}
*/	return true;
}

void CHostPlayer::SetStart(bool bStart)
{
	if (m_bStart==bStart)
		return;

	m_bStart = bStart;
	A3DVECTOR3 vCameraPos = g_Render.GetA3DEngine()->GetActiveCamera()->GetPos();
	A3DVECTOR3 vCameraDir = g_Render.GetA3DEngine()->GetActiveCamera()->GetDir();
	RAYTRACERT rt;
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	CTerrain *pTerrain =  pMap->GetTerrain();
	if(pTerrain->RayTrace(vCameraPos, 1000.0f*vCameraDir, 1.0f, &rt))
		m_vPos=rt.vHitPos;
	else
		m_vPos=A3DVECTOR3(.0f, 800.0f, .0f);
}

void CHostPlayer::DeltaCamDist(float fDelta)
{
	m_fCamDist += fDelta;
	if (m_fCamDist>100.0f)
	{
		m_fCamDist = 100.0f;
	}

	if (m_fCamDist<1.0f)
	{
		m_fCamDist = 1.0f;
	}

}