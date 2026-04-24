#pragma once

//	Multi-thread loaded EC model type
enum
{
	MTL_ECM_PLAYER = 0,
	MTL_ECM_NPC,
	MTL_ECM_MATTER,
	MTL_ECM_SCNMODEL,		//	EC model in scene
	MTL_ECM_PLAYER_EQUIP,
	MTL_ECM_PET,
	MTL_ECM_CHANGEDMODEL,
	MTL_ECM_TRANSFIGUREDMODEL,	// transfigured model
	MTL_ECM_AIRCRAFT,
};

class CECSceneThreadObj;
class CECModel;

bool InitLoaderThread();
void StartLoaderThread();
void ExitLoaderThread();

void QueueLoadInThread(CECSceneThreadObj* pObj);
void QueueReleaseInThread(CECSceneThreadObj* pObj);
void QueueTerrainBlock(int r, int c, int iBlock);
void QueueECModelForLoad(int iType, int idObject, DWORD dwBornStamp, const A3DVECTOR3& vPos, const char* szFile, const char* szFile2,
						 int p1=0, int p2=0, int p3=0, int* p4 = NULL, int p5 =0, int p6 =0, int p7=0, int p8=0, int p9=0);
void QueueECModelForRelease(CECModel* pModel);
void QueuePlayerUndoLoad(int cid, DWORD dwBornStamp, bool bPlayer = false);

void SuspendLoadThread();
void LoadInMainThread(bool bForceLoad);
void ThreadRemoveAllLoaded();

bool IsLoadThreadReady();
