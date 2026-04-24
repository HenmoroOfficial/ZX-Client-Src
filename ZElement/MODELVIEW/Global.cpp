#include "Global.h"
#include "..\ZElementData\DataPathMan.h"
#include "MODELVIEWDataMan.h"

#define new A_DEBUG_NEW

ALog		g_Log;
char		g_szWorkDir[MAX_PATH];
char        g_szIniFile[MAX_PATH];
A3DVECTOR3	g_vOrigin(0.0f);
A3DVECTOR3	g_vAxisX(1.0f, 0.0f, 0.0f);
A3DVECTOR3	g_vAxisY(0.0f, 1.0f, 0.0f);
A3DVECTOR3	g_vAxisZ(0.0f, 0.0f, 1.0f);
bool        g_bPause = false;
bool        g_bStop = true;
bool        g_bLoop = false;
float       g_fPlaySpeed = 1.0f;
bool		g_bShowAABB = false;
bool		g_bShowGrid = true;
bool		g_bShowWire = false;
bool        g_bUsedPath = false;
CRenderableObject *g_pRenderableObject = NULL;
MODELVIEWDataMan g_DataMan;
DataPathMan    g_DataPathMan;
A3DCombActDynData*   g_pCurComAct = NULL;
bool        g_bFashion = false;
bool        g_bUseDefaultCamPos = true;

A3DVECTOR3 g_vLR = g_vOrigin;
A3DVECTOR3 g_vLPos = g_vOrigin;
DWORD      g_dwClearClr = RGB(128,64,0);
DWORD      g_dwDirClr =   RGB(255, 255, 255);
DWORD      g_dwAmbClr =   RGB(200, 200, 200);

abase::vector<SKILL_ELEMENT> g_ListSkill;

void Aux_ReleaseObject()
{
	if(g_pRenderableObject)
	{
		g_pRenderableObject->GetRotateAndPos(g_vLR, g_vLPos);
		g_pRenderableObject->SaveConfig();
		g_pRenderableObject->Release();
		delete g_pRenderableObject;
		g_pRenderableObject = NULL;
	}
}

void Aux_SetObject( CRenderableObject *pnew)
{
	if(pnew==0) return;
	Aux_ReleaseObject();
	g_pRenderableObject = pnew;
}
