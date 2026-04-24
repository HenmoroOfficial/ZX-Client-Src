#include "stdafx.h"
#include "Resource.h"

#include "ALog.h"
#include "AMemory.h"
#include "AString.h"
#include "A3DTypes.h"
#include "RenderableObject.h"

class MODELVIEWDataMan;
class DataPathMan;
class A3DCombActDynData;

struct SKILL_ELEMENT
{
	char szName[256];
	unsigned int id;
};


extern ALog			g_Log;
extern char			g_szWorkDir[];
extern char         g_szIniFile[];
extern A3DVECTOR3	g_vOrigin;
extern A3DVECTOR3	g_vAxisX;
extern A3DVECTOR3	g_vAxisY;
extern A3DVECTOR3	g_vAxisZ;
extern bool         g_bPause;
extern bool         g_bStop;
extern bool         g_bLoop;
extern float        g_fPlaySpeed;
extern CRenderableObject* g_pRenderableObject;
extern MODELVIEWDataMan g_DataMan;
extern DataPathMan    g_DataPathMan;
extern A3DCombActDynData*   g_pCurComAct;
extern bool        g_bFashion;
extern bool		   g_bShowAABB;
extern bool		   g_bShowGrid;
extern bool		   g_bShowWire;
extern bool        g_bUsedPath;
extern bool        g_bUseDefaultCamPos;
extern A3DVECTOR3  g_vLR;
extern A3DVECTOR3  g_vLPos;
extern abase::vector<SKILL_ELEMENT> g_ListSkill;

extern DWORD      g_dwClearClr;
extern DWORD      g_dwDirClr;
extern DWORD      g_dwAmbClr;

void Aux_SetObject( CRenderableObject *pnew);

void Aux_ReleaseObject();