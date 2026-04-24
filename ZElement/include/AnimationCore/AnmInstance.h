#pragma once

#include <A3D.h>
#include <A3DGFXEx.h>

class CAnmObject;
class CECWorld;
class CECModel;
class CECGameRun;

class CAnmInstance
{
public:
	virtual ~CAnmInstance() {}

	virtual A3DEngine* GetA3DEngine() = 0;

	virtual A3DCamera* GetA3DCamera() = 0;

	virtual A3DViewport* GetA3DViewport() = 0;

	virtual CECWorld* GetWorld() = 0;

	virtual CECGameRun* GetGameRun() = 0;

	virtual bool LoadWorld( int iInstID, const A3DVECTOR3& c_vCameraPos ) { return true; }
	virtual void UnloadWorld() { return; }

	virtual bool LoadObject( CAnmObject* pObject ) = 0;
	virtual void UnloadObject( CAnmObject* pObject ) = 0;

	virtual bool TickObject( CAnmObject* pObject, DWORD dwDeltaTime ) = 0;

	virtual void UpdateText(const ACString* pText) {}
	virtual bool RenderLetterboxAndText() { return false; }
#ifdef BACK_VERSION
	virtual bool RenderObject( CAnmObject* pObject, bool bRenderShadow ) = 0;
#else
	virtual bool RenderObject( CAnmObject* pObject, A3DViewport* pA3DViewport, bool bRenderShadow ) = 0;
#endif
};