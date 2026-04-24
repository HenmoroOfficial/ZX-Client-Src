#pragma once

#include "AnmInstance.h"
#include "AnmNPC.h"
#include "AnmPlayer.h"
#include "AnmGFX.h"
#include <GfxCommonHeaders.h>
#include <A3D.h>
#include <A3DFTFontMan.h>
#include <vector>
#include <map>

class CAnmObjLoader;
class CAnmObject;
class CECShadowRender;
class CECFullGlowRender;
class CELBackMusic;
class CECGFXCaster;
class CECInstance;
class CECViewport;
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

	virtual A3DEngine* GetA3DEngine() { return m_pA3DEngine; }

	virtual A3DCamera* GetA3DCamera() { return m_pCamera; }

	virtual A3DViewport* GetA3DViewport() { return m_pViewport; }

	virtual CECFullGlowRender* GetFullGlowRender() { return m_pFullGlowRender; }

	virtual CECWorld* GetWorld() { ASSERT(0); return 0; }

	virtual CECWorld* GetAnmWorld() { return m_pWorld; }

	virtual bool LoadWorld( int iInstID, const A3DVECTOR3& c_vCameraPos );
	virtual void UnloadWorld();

	virtual bool LoadObject( CAnmObject* pObject );
	virtual void UnloadObject( CAnmObject* pObject );

	virtual bool TickObject( CAnmObject* pObject, DWORD dwDeltaTime );
	virtual bool RenderObject( CAnmObject* pObject, bool bRenderShadow );

	void AddHightLightModel( const HLModel& model ) { m_vHLModel.push_back( model ); }
	bool RenderHightLightModelList( A3DViewport* pViewport );

	CECInstance* GetInstance( int iInstID );

	void NotifyWndSizeChanged( int iWndWidth,int iWndHeight );

protected:

	void RenderScreenInfo();

public:

	HINSTANCE			m_hInstance;
	HWND				m_hWnd;
	int					m_iWndWidth;
	int					m_iWndHeight;
	A3DEngine*			m_pA3DEngine;
	A3DDevice*			m_pA3DDevice;
	A3DLight*			m_pDirLight;
	A3DLight*			m_pDynamicLight;
	A3DVertexShader*	m_pvsSMEdge;
	A3DVertexShader*	m_pvsRMEdge;
	A3DVertexShader*	m_pvsObstructSkinModel;
	A3DVertexShader*	m_pvsObstructRigidModel;
	A3DPixelShader*		m_pEdgeObPS;
	A3DPixelShader*		m_pEdgePS;
	A3DPixelShader*		m_pGlowPS;
	A3DShader*			m_pEdgeGlowShader;
	A3DSkinRender*		m_pSkinRender1;
	A3DSkinRender*		m_pSkinRender2;
	CECShadowRender*	m_pShadowRender;
	CECFullGlowRender*	m_pFullGlowRender;
	GfxPSConstVec		m_EdgePsConsts;
	CECGFXCaster*		m_pGFXCaster;
	A3DFTFontMan*		m_pFTFontMan;
	A3DFTFont*			m_pDefaultFont;
	CELBackMusic*		m_pBackMusic;
	A3DGFXExMan*		m_pGfxExMan;
	CECModelMan*		m_pModelMan;
	DWORD				m_dwAmbient;
	bool				m_bFogEnable;
	float				m_fFogStart;
	float				m_fFogEnd;
	float				m_fFogDensity;
	DWORD				m_dwFogColor;
	DWORD				m_dwCammaLevel;
	A3DVECTOR3			m_vDirLightDir;
	bool				m_bMipMapBias;
	bool				m_bUpdateShadowMap;
	bool				m_bOptimizeShadowMap;
	char				m_szWorkDir[ MAX_PATH ];
	DWORD				m_dwTime;
	DWORD				m_dwFrames;
	DWORD				m_dwDeltaTime;
	float				m_fPlayedTime;
	HLModelVector		m_vHLModel;
	InstanceMap			m_mInstance;
	CECViewport*		m_pECViewport;
	A3DViewport*		m_pViewport;
	A3DCamera*			m_pCamera;
	CAnmObjLoader*		m_pObjLoader;
	CECWorld*			m_pWorld;

protected:

	bool InitA3DEngine();
	void ReleaseA3DEngine();

	bool InitFTFontMan();
	void ReleaseFTFontMan();

	bool InitGfxExMan();
	void ReleaseGfxExMan();

	bool InitModelMan();
	void ReleaseModelMan();

	bool InitSkinRender();
	void ReleaseSkinRender();

	bool InitFullGlowRender();
	void ReleaseFullGlowRender();

	bool InitGfxCaster();
	void ReleaseGfxCaster();

	bool InitShadowRender();
	void ReleaseShadowRender();

	bool InitBackMusic();
	void ReleaseBackMusic();

	bool InitInstanceInfo();
	void ReleaseInstanceInfo();

	CECInstance* GetInstance( const char* szFile );

	void TickWorld( DWORD );
	void RenderWorld();

	bool TickNPC( CAnmNPC* pNPC, DWORD dwDeltaTime );
	bool TickPlayer( CAnmPlayer* pPlayer, DWORD dwDeltaTime );
	bool TickGFX( CAnmGFX* pGFX, DWORD dwDeltaTime );

	bool RenderNPC( CAnmNPC* pNPC, bool bRenderShadow );
	bool RenderPlayer( CAnmPlayer* pPlayer, bool bRenderShadow );
	bool RenderGFX( CAnmGFX* pGFX, bool bRenderShadow );
};

CAnmSystem& GetAnmSystem();
#define g_AnmSystem (GetAnmSystem())