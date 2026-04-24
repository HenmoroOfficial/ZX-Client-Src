#pragma once

#include "AnmInstance.h"
#include "AnmNPC.h"
#include "AnmPlayer.h"
#include "AnmGFX.h"
#include <GfxCommonHeaders.h>
#include <A3D.h>
//#include <A3DFTFontMan.h>
#include <vector>
#include <map>

class CAnmObjLoader;
class CAnmObject;
class CECShadowRender;
class CECFullGlowRender;
class CELBackMusic;
class CECGFXCaster;
class CECInstance;
#ifdef BACK_VERSION
class CECViewport;
#else
class ECViewport;
#define CECViewport ECViewport
#endif
class CECWorld;
class CECModelMan;

class CAnmSystem : public CAnmInstance
{
public:

	struct HLModel
	{
		HLModel() : pModel(0), clr(0xffffffff), fEdgeLen(-1) {}
		HLModel( CECModel* _pModel, A3DCOLOR _clr, float _fEdgeLen = -1 ) : pModel(_pModel) , clr(_clr), fEdgeLen(_fEdgeLen) {}

		CECModel* pModel;
		A3DCOLOR clr;
		float fEdgeLen;
	};
	typedef std::vector< HLModel > HLModelVector;

	typedef std::map< int, CECInstance* > InstanceMap;

public:

	CAnmSystem();

	bool Init( HINSTANCE hInst, HWND hWnd );
	void Release();

	void Tick( DWORD dwDeltaTime );
	void Render();

	virtual A3DEngine* GetA3DEngine();
	
	virtual A3DCamera* GetA3DCamera();
	
	virtual A3DViewport* GetA3DViewport();
	
	virtual CECWorld* GetWorld() { return m_pWorld; }

	virtual CECGameRun* GetGameRun() ;

	virtual bool LoadWorld( int iInstID, const A3DVECTOR3& c_vCameraPos );
	virtual void UnloadWorld();

	virtual bool LoadObject( CAnmObject* pObject );
	virtual void UnloadObject( CAnmObject* pObject );

	virtual bool TickObject( CAnmObject* pObject, DWORD dwDeltaTime );
#ifdef BACK_VERSION
	virtual bool RenderObject( CAnmObject* pObject, bool bRenderShadow );
#else
	virtual bool RenderObject( CAnmObject* pObject, A3DViewport* pA3DViewport, bool bRenderShadow );
#endif

	CECInstance* GetInstance( int iInstID );

	void NotifyWndSizeChanged( int iWndWidth,int iWndHeight );

protected:

	void RenderScreenInfo();

public:

	HINSTANCE			m_hInstance;
	HWND				m_hWnd;
	DWORD				m_dwTime;
	DWORD				m_dwFrames;
	DWORD				m_dwDeltaTime;
	float				m_fPlayedTime;
	CAnmObjLoader*		m_pObjLoader;

protected:
	A3DEngine*				m_pEngine;
	A3DDevice*				m_pDevice;
#ifdef BACK_VERSION
	CECViewport*			m_pECViewport;
#else
	ECViewport*			m_pECViewport;
#endif
	A3DGFXExMan*			m_pGfxExMan;
	CECWorld*				m_pWorld;
	A3DLight*				m_pDirLight;
	A3DCamera*				m_pCamera;
	A3DViewport*			m_pViewport;
	CECGFXCaster*			m_pGfxCaster;
#ifdef BACK_VERSION
	A3DSkinRender*			m_pSkinRender1;
	A3DSkinRender*			m_pSkinRender2;
#else
	A3DSkinRenderBase*		m_pSkinRender1;
	A3DSkinRenderBase*		m_pSkinRender2;
#endif

protected:

	CECInstance* GetInstance( const char* szFile );

	void TickWorld( DWORD );
	void RenderWorld();

	bool TickNPC( CAnmNPC* pNPC, DWORD dwDeltaTime );
	bool TickPlayer( CAnmPlayer* pPlayer, DWORD dwDeltaTime );
	bool TickGFX( CAnmGFX* pGFX, DWORD dwDeltaTime );

#ifdef BACK_VERSION
	bool RenderNPC( CAnmNPC* pNPC, bool bRenderShadow );
	bool RenderPlayer( CAnmPlayer* pPlayer, bool bRenderShadow );
	bool RenderGFX( CAnmGFX* pGFX, bool bRenderShadow );
#else
	bool RenderNPC( CAnmNPC* pNPC, A3DViewport* pA3DViewport, bool bRenderShadow );
	bool RenderPlayer( CAnmPlayer* pPlayer, A3DViewport* pA3DViewport, bool bRenderShadow );
	bool RenderGFX( CAnmGFX* pGFX, A3DViewport* pA3DViewport, bool bRenderShadow );
#endif
};

CAnmSystem& GetAnmSystem();
#define g_AnmSystem (GetAnmSystem())