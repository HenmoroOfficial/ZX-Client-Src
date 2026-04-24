/*
 * FILE: Render.h
 *
 * DESCRIPTION: Render class
 *
 * CREATED BY: Duyuxin, 2004/6/8
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#pragma once

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Types and Global variables
//
///////////////////////////////////////////////////////////////////////////

class A3DEngine;
class A3DDevice;	
class A3DLight;	
class A3DFont;
class A3DSkinRender;
class A3DPixelShader;
class A3DRenderTarget;
class A3DStream;
class A3DVertexShader;
#ifdef _ANGELICA22
class PlatformRenderer;
class A3DVolumeTexture;
#endif // #ifdef _ANGELICA22
///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Class CRender
//
///////////////////////////////////////////////////////////////////////////

// Vertex for glow area
#define A3DGLOWVERT_FVF		D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1
struct A3DGLOWVERTEX
{
	A3DGLOWVERTEX() {}
	A3DGLOWVERTEX(const A3DVECTOR4& _pos, A3DCOLOR _diffuse, float _u, float _v)
	{
		pos = _pos;
		diffuse = _diffuse;
		u = _u;
		v = _v;
	}
	 
	A3DVECTOR4	pos;			//	Position on screen
	A3DCOLOR	diffuse;		//	Color
	float		u, v;			//	Texture coords of center
};

class CRender
{
public:		//	Types

public:		//	Constructor and Destructor

	CRender();
	virtual ~CRender() {}

public:		//	Attributes

public:		//	Operations

	//	Initialize game
	bool Init(HINSTANCE hInst, HWND hWnd);
	//	Release game
	void Release();

	//	Resize device
	bool ResizeDevice(int iWid, int iHei);
	//	Begin render
	bool BeginRender(bool bDoWarps);
	//	End render
	bool EndRender(bool bDoWarps);

	//	Output text
	void TextOut(int x, int y, const char* szText, DWORD color);
	//	Draw 2D rectangle
	void Draw2DRect(const RECT& rc, DWORD dwCol, bool bFrame);

	//	Set ambient color and diretional light's direction and color
	void ChangeLight(const A3DVECTOR3& vDir, DWORD dwDirCol, DWORD dwAmbient);
	
	//	Get interface
	A3DEngine* GetA3DEngine() { return m_pA3DEngine; }
	A3DDevice* GetA3DDevice() {	return m_pA3DDevice; }
	A3DLight* GetDirLight() { return m_pDirLight; }
	A3DFont* GetSystemFont() { return m_pFont; }

	//	Engine is ready ?
	bool EngineIsReady() { return m_bEngineOK; }
	
	// Set mono blur
	void BeginMonoBlur(float fMonoBlurLevel, float fMonoBlurLevelSet, int nMonoBlurTime);
	void EndMonoBlur();

	void SetFlashPower(float fMinLum, float vStart, float vEnd, int nTime);
	void Update(int iDeltaTime);
	A3DRenderTarget* GetGlowTarget() { return m_pGlowTarget1;}
#ifdef _ANGELICA22
	void SetColorMappingTex(AString& strTexture);
    AString GetColorMappingTex() { return m_strColorMapping; }
    
    PlatformRenderer* GetPlatformRender() { return m_pPlatformRender; }

	void SetIsPresent(bool bIsPresent) { m_bIsPresent = bIsPresent; }
#endif // #ifdef _ANGELICA22

protected:	//	Attributes

	A3DEngine*		m_pA3DEngine;		//	A3D engine
	A3DDevice*		m_pA3DDevice;		//	A3D device
	A3DLight*		m_pDirLight;		//	Directional light
	A3DFont*		m_pFont;			//	Font object
	A3DRenderTarget*m_pFrameTarget;		//	A3D frame target which may be used as texture

	HINSTANCE		m_hInstance;		//	Handle of instance
	HWND			m_hRenderWnd;		//	Render window's handle
	int				m_iRenderWid;		//	Render size
	int				m_iRenderHei;
	// space warp objects.
	A3DPixelShader *		m_pTLWarpShader;		// TL warp render pixel shader
	A3DStream *				m_pTLWarpStream;		// stream used to show space warps using TL verts
	int						m_nMaxTLWarpVerts;		// max verts number of TL warp stream
	int						m_nMaxTLWarpIndices;	// max indices number of TL warp stream
	A3DPixelShader *		m_pWarpShader;
	A3DStream *				m_pWarpStream;
	int						m_nMaxWarpVerts;
	int						m_nMaxWarpIndices;

	// we do a 5 pixel blur both on x-axis and y-axis
	A3DRenderTarget*		m_pGlowTarget1;		//	A3D frame target which may be used as texture

	A3DStream *				m_pStreamBlur;			// stream used to do bluring on both axis x & y
	A3DGLOWVERTEX			m_vertsBlur[4];			// vertex for blur stream
	A3DVertexShader *		m_pGaussinBlurVS;		// vertex shader for gaussin blur
	WORD					m_indices[6];

	float					m_GlowRadius;		// glow gaussin blur radius in pixels for original screen size
	float					m_GlowWeights[3];	// glow gaussin blur weights of the first 3 pixels from left to center
	A3DCOLOR				m_color;
	A3DPixelShader *		m_pGlow1Shader;
	A3DPixelShader *		m_pGlow2Shader;
#ifdef _ANGELICA22

	A3DPixelShader *        m_pColorMappingPS;
	//IDirect3DVolumeTexture9 *m_pColorMapping;
	A3DVolumeTexture*		m_pColorMappingVolTex;
	AString					m_strColorMapping;
    PlatformRenderer*       m_pPlatformRender;
#endif // #ifdef _ANGELICA22

	float					m_vMonoBlurLevel;
	float					m_vMonoBlurLevelSet;
	float					m_vMonoBlurStep;
	int						m_nMonoBlurTime;
	float					m_vMonoBlurLevelCur;
	A3DCOLOR				m_colorMono;
	bool					m_bStartMono;
	bool					m_bMonoTick;
	DWORD					m_dwLastTime;
	
	float					m_vMotionBlurLevel;
	float					m_vMotionBlurLevelSet;
	float					m_vMotionBlurStep;
	int						m_nMotionBlurTime;
	
	float					m_vFadeLevel;
	float					m_vFadeLevelSet;
	float					m_vFadeStep;
	int						m_nFadeTime;
	
	float					m_vGlowPower;
	float					m_vGlowPowerSet;
	float					m_vGlowPowerStep;
	int						m_nGlowPowerTime;
	
	float					m_vGlowLevel;
	float					m_vGlowLevelSet;
	float					m_vGlowLevelStep;
	int						m_nGlowLevelTime;
	
	float					m_fMinLum;
	float					m_fFlashPower;
	float					m_fFlashPowerSet;
	float					m_fFlashPowerStep;
	int						m_nFlashTime;
	
	bool			m_bEngineOK;		//	Engine is ready
#ifdef _ANGELICA22
	bool			m_bIsPresent;		
#endif // #ifdef _ANGELICA22
	
protected:	//	Operations

	//	Initialize A3D engine
	bool InitA3DEngine();
	//	Release A3D eigine
	void ReleaseA3DEngine();
	//	Check device
	bool CheckDevice();
	bool CreateFrameTarget();
	bool DoWarps();
	bool DoGlow();
	void CalculateGaussian(float radius, float weights[3]);
	void SetupVertexConsts(float cx, float cy, float dx, float dy);
	void PrepareGlow1Pass( float gaussin_weights[3] );
	void PrepareGlow2Pass(A3DCOLOR colorNow, float gaussin_weights[3]);
};

extern CRender	g_Render;

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////

