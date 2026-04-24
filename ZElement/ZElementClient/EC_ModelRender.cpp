// Filename	:	DlgDailyPromp.cpp
// Creator	:	Fu Chonggang
// Date		:	Sep 2, 2010
#include "EC_ModelRender.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_PortraitRender.h"
#include "EC_Model.h"
#include "EC_GameUIMan.h"
#include "EC_UIManager.h"
#include "EC_LoginPlayer.h"
#include "EC_World.h"
#include "DlgFengshenSoul.h"

#include "a3dengine.h"
#include "a3ddevice.h"
#include "a3dviewport.h"
#include "a3dcamera.h"
#include "a3dskillgfxevent.h"
#include "a3dcombinedaction.h"

static const A3DVECTOR3 camPos[MRT_NUM] = 
{ 
	A3DVECTOR3(100.0f, 0.0f, -6.0f),
	A3DVECTOR3(0.0f, 2.0f, -12.0f),
	A3DVECTOR3(-100.0f, 0.0f, -6.0f),
	A3DVECTOR3(0.0f, 2.0f, -12.0f),
	A3DVECTOR3(0.0f, 2.0f, -15.0f),
};

void DlgModelRender(const A3DRECT &rc, DWORD param1, DWORD param2, DWORD param3)
{
	if( param1 == 0 )
		return;
	
	CECModel* pModel = (CECModel*)param1;
	int index = param2;
	float vDeg = DEG2RAD(param3);

	A3DViewport * pCurViewport = g_pGame->GetA3DEngine()->GetActiveViewport();
	A3DViewport * pPortraitViewport = g_pGame->GetPortraitRender()->GetPortraitViewport();
	A3DCameraBase * pCamera = pPortraitViewport->GetCamera();
	A3DVIEWPORTPARAM viewParam = *pPortraitViewport->GetParam();
	viewParam.X = rc.left;
	viewParam.Y = rc.top;
	viewParam.Width = rc.right - rc.left;
	viewParam.Height = rc.bottom - rc.top;
	pPortraitViewport->SetParam(&viewParam);
	((A3DCamera *) pCamera)->SetProjectionParam(-1.0f, -1.0f, 1200.0f, ((float)viewParam.Width) / viewParam.Height);
	
	A3DMATRIX4 mat = a3d_RotateY(vDeg);
	A3DVECTOR3 vecPos = camPos[index];
	A3DVECTOR3 vecDir = A3DVECTOR3(0, 0, 1.0f);
	A3DVECTOR3 vecUp = A3DVECTOR3(0, 1.0f, 0);
	// dailypromp 自适应计算摄像机
	if (index==MRT_DAILYPROMP)
	{
		const A3DAABB& chAABB = pModel->HasCHAABB() ? pModel->GetCHAABB() : pModel->GetModelAABB();
		const A3DAABB& mapAABB = pModel->GetModelAABB();
		vecPos = A3DVECTOR3(mapAABB.Center.x, mapAABB.Center.y, -chAABB.Center.z-chAABB.Extents.z-3.5f*(chAABB.Maxs.y-chAABB.Mins.y ));
	}
	else if(index == MRT_LITTLEPET)
	{
		const A3DAABB& chAABB = pModel->HasCHAABB() ? pModel->GetCHAABB() : pModel->GetModelAABB();
		const A3DAABB& mapAABB = pModel->GetModelAABB();
		vecPos = A3DVECTOR3(mapAABB.Center.x, mapAABB.Center.y, -chAABB.Center.z-chAABB.Extents.z-5.5f*(chAABB.Maxs.y-chAABB.Mins.y ));
	}
	else if (index==MRT_PET)
	{
		const A3DAABB& chAABB = pModel->HasCHAABB() ? pModel->GetCHAABB() : pModel->GetModelAABB();
		const A3DAABB& mapAABB = pModel->GetModelAABB();
		vecPos = A3DVECTOR3(chAABB.Center.x, chAABB.Center.y, -3-chAABB.Center.z-chAABB.Extents.z-3.5f*(chAABB.Maxs.y-chAABB.Mins.y ));
	}
	
	pCamera->SetPos(vecPos);
	pCamera->SetDirAndUp(vecDir, vecUp);
	vecDir = A3DVECTOR3(0, 0, -1.0f) * mat;
	pModel->SetDirAndUp(vecDir, vecUp);
	pPortraitViewport->Active();
	pPortraitViewport->ClearDevice();
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(false);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(true);
	
	pModel->Render(pPortraitViewport, true);
	
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(false);
	AfxGetGFXExMan()->RenderAllGfx(pPortraitViewport);
	
	pCurViewport->Active();
}

void DlgModelRenderFixDist(const A3DRECT &rc, DWORD param1, DWORD param2, DWORD param3)
{
	if( param1 == 0 )
		return;
	
	CECModel* pModel = (CECModel*)param1;
	float fSize = *(float*)param2;
	float vDeg = DEG2RAD(param3);

	A3DViewport * pCurViewport = g_pGame->GetA3DEngine()->GetActiveViewport();
	A3DViewport * pPortraitViewport = g_pGame->GetPortraitRender()->GetPortraitViewport();
	A3DCameraBase * pCamera = pPortraitViewport->GetCamera();
	A3DVIEWPORTPARAM viewParam = *pPortraitViewport->GetParam();
	viewParam.X = rc.left;
	viewParam.Y = rc.top;
	viewParam.Width = rc.right - rc.left;
	viewParam.Height = rc.bottom - rc.top;
	pPortraitViewport->SetParam(&viewParam);
	((A3DCamera *) pCamera)->SetProjectionParam(-1.0f, -1.0f, 1200.0f, ((float)viewParam.Width) / viewParam.Height);
	
	A3DMATRIX4 mat = a3d_RotateY(vDeg);
	A3DVECTOR3 vecPos = A3DVECTOR3(0, 0.25*fSize, -14*fSize);// camPos[index];
	A3DVECTOR3 vecDir = A3DVECTOR3(0, 0, 1.0f);
	A3DVECTOR3 vecUp = A3DVECTOR3(0, 1.0f, 0);
	// dailypromp 自适应计算摄像机
/*	if (index==MRT_DAILYPROMP)
	{
		const A3DAABB& chAABB = pModel->HasCHAABB() ? pModel->GetCHAABB() : pModel->GetModelAABB();
		const A3DAABB& mapAABB = pModel->GetModelAABB();
		vecPos = A3DVECTOR3(mapAABB.Center.x, mapAABB.Center.y, -chAABB.Center.z-chAABB.Extents.z-3.5f*(chAABB.Maxs.y-chAABB.Mins.y ));
	}
	else if(index == MRT_LITTLEPET)
	{
		const A3DAABB& chAABB = pModel->HasCHAABB() ? pModel->GetCHAABB() : pModel->GetModelAABB();
		const A3DAABB& mapAABB = pModel->GetModelAABB();
		vecPos = A3DVECTOR3(mapAABB.Center.x, mapAABB.Center.y, -chAABB.Center.z-chAABB.Extents.z-5.5f*(chAABB.Maxs.y-chAABB.Mins.y ));
	}
	else if (index==MRT_PET)
	{
		const A3DAABB& chAABB = pModel->HasCHAABB() ? pModel->GetCHAABB() : pModel->GetModelAABB();
		const A3DAABB& mapAABB = pModel->GetModelAABB();
		vecPos = A3DVECTOR3(chAABB.Center.x, chAABB.Center.y, -3-chAABB.Center.z-chAABB.Extents.z-3.5f*(chAABB.Maxs.y-chAABB.Mins.y ));
	}
*/	
	pCamera->SetPos(vecPos);
	pCamera->SetDirAndUp(vecDir, vecUp);
	vecDir = A3DVECTOR3(0, 0, -1.0f) * mat;
	pModel->SetDirAndUp(vecDir, vecUp);
	pPortraitViewport->Active();
	pPortraitViewport->ClearDevice();
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(false);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(true);
	
	pModel->Render(pPortraitViewport, true);
	
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(false);
	AfxGetGFXExMan()->RenderAllGfx(pPortraitViewport);
	
	pCurViewport->Active();
}

void DlgDeityModelRender(const A3DRECT &rc, DWORD param1, DWORD param2, DWORD param3)
{
	if( param1 == 0 )
		return;
	
	CECModel* pModel = (CECModel*)param1;
	int index = param2;
	
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	CDlgFengshenSoul* pDlg = (CDlgFengshenSoul*)pGameUI->GetDialog(param3==0?"Win_Fengshen_Soul":"Win_Fengshen_Soul_Check");
	A3DViewport * pCurViewport = g_pGame->GetA3DEngine()->GetActiveViewport();
	A3DViewport * pPortraitViewport = pDlg->GetPortrait()->GetPortraitViewport();
	A3DCameraBase * pCamera = pPortraitViewport->GetCamera();
	A3DVIEWPORTPARAM viewParam = *pPortraitViewport->GetParam();
	viewParam.X = rc.left;
	viewParam.Y = rc.top;
	viewParam.Width = rc.right - rc.left;
	viewParam.Height = rc.bottom - rc.top;
	pPortraitViewport->SetParam(&viewParam);
	((A3DCamera *) pCamera)->SetProjectionParam(-1.0f, -1.0f, 1200.0f, ((float)viewParam.Width) / viewParam.Height);
	
	A3DVECTOR3 vecPos = camPos[index];
	A3DVECTOR3 vecDir = A3DVECTOR3(0, 0, 1.0f);
	A3DVECTOR3 vecUp = A3DVECTOR3(0, 1.0f, 0);
	// dailypromp 自适应计算摄像机
	if (index==MRT_DAILYPROMP)
	{
		const A3DAABB& aabb = pModel->GetModelAABB();
		vecPos = A3DVECTOR3(aabb.Center.x, aabb.Center.y, -5.0f-3*(aabb.Maxs.y-aabb.Mins.y ));
	}
	
	pCamera->SetPos(vecPos);
	pCamera->SetDirAndUp(vecDir, vecUp);
	pPortraitViewport->Active();
	pPortraitViewport->ClearDevice();
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(false);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(true);
	
	pModel->Render(pPortraitViewport, true);
	
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(false);
	AfxGetGFXExMan()->RenderAllGfx(pPortraitViewport);
	
	pCurViewport->Active();
}

void DlgModelMountWingRender(const A3DRECT &rc, DWORD param1, DWORD param2, DWORD param3)
{
//	static const float _dist[4][2] = { {-6.0f, -5.8f}, {-5.9f, -5.7f}, {-6.3f, -6.0f}, {-7.3f, -6.2f},  };
//	static const float _height[4][2] = { {0.9f, 0.9f}, {0.9f, 0.8f}, {1.3f, 1.2f}, {1.1f, 0.95f},  };
	
	if( param1 == 0 )
		return;
	
	CECPlayer * pPlayer = (CECPlayer *) param1;
	float vDeg = DEG2RAD(param2);
	
	A3DViewport * pCurViewport = g_pGame->GetA3DEngine()->GetActiveViewport();
	
	A3DViewport * pPortraitViewport = g_pGame->GetPortraitRender()->GetPortraitViewport();
	A3DCameraBase * pCamera = pPortraitViewport->GetCamera();
	
	A3DVIEWPORTPARAM viewParam = *pPortraitViewport->GetParam();
	viewParam.X = rc.left;
	viewParam.Y = rc.top;
	viewParam.Width = rc.right - rc.left;
	viewParam.Height = rc.bottom - rc.top;
	pPortraitViewport->SetParam(&viewParam);
	((A3DCamera *) pCamera)->SetProjectionParam(-1.0f, -1.0f, -1.0f, ((float)viewParam.Width) / viewParam.Height);
	
	int nGender = pPlayer->GetGender();
	int nPhysique	= pPlayer->GetPhysique();
	if (param3>100)
		param3 = 100;
	A3DMATRIX4 mat = a3d_RotateY(vDeg);
	A3DVECTOR3 vecPos = A3DVECTOR3(0, 1.3f, -15.0f) * mat;
	A3DVECTOR3 vecDir = A3DVECTOR3(0, 0, 1.0f) * mat;
	A3DVECTOR3 vecUp = A3DVECTOR3(0, 1.0f, 0);
	
	pCamera->SetPos(vecPos);
	pCamera->SetDirAndUp(vecDir, vecUp);
	pPortraitViewport->Active();
	pPortraitViewport->ClearDevice();
	
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(false);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(true);
	CECModel* pModel = pPlayer->GetPetModel() ? pPlayer->GetPetModel() : pPlayer->GetPlayerModel();
	pModel->SetGfxUseLOD(false);
	pModel->Render(pPortraitViewport, true);
		// 	CECViewport viewport;
	// 	viewport.InitFromA3D(pPortraitViewport);
	// 	pPlayer->RenderEquipBloom(&viewport);
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(false);
	AfxGetGFXExMan()->RenderAllGfx(pPortraitViewport);
	
	pCurViewport->Active();
}

void ReLoadModel(CECLoginPlayer*& pPlayer)
{
	A3DRELEASE(pPlayer);
	pPlayer = new CECLoginPlayer(g_pGame->GetGameRun()->GetWorld()->GetPlayerMan());
	const GNET::RoleInfo& info = g_pGame->GetGameRun()->GetSelectedRoleInfo();
	
	if (!pPlayer->Load(CECPlayer::GetPhysiqueByProf(info.occupation), info.occupation, info.faceid, info.hairid, info.gender, NULL))
	{
		pPlayer->Release();
		delete pPlayer;
		pPlayer = NULL;
	}
	else
	{
		pPlayer->SetPos(A3DVECTOR3(0));
		pPlayer->SetDirAndUp(A3DVECTOR3(0, 0, -1.0f), A3DVECTOR3(0, 1.0f, 0));
		pPlayer->GetPlayerModel()->SetAutoUpdateFlag(false);
	}
}
