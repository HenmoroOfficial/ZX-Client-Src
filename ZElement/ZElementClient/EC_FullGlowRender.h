/*
 * FILE: EC_FullGlowRender.h
 *
 * DESCRIPTION: Fullscreen glow for the Element Client
 *
 * CREATED BY: Hedi, 2004/10/9 
 *
 * HISTORY:
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#ifndef _EC_FULLGLOWRENDER_H_
#define _EC_FULLGLOWRENDER_H_

#include <A3DTypes.h>
#include <A3DVertex.h>

class A3DDevice;
class A3DRenderTarget;
class A3DStream;
class A3DPixelShader;
class A3DVertexShader;
class A3DTexture;
class A3DViewport;

// Glow types
enum FULLGLOW_TYPE
{
	FULLGLOW_TYPE_NULL = 0,
	FULLGLOW_TYPE_DAYLIGHT = 1,
	FULLGLOW_TYPE_NIGHT = 2,
	FULLGLOW_TYPE_UNDERWATER = 3,
	FULLGLOW_TYPE_DEAD = 4,
	FULLGLOW_TYPE_LOGIN = 5,
};

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

// screen present controller
class CECFullGlowPresenter
{
private:
	float				m_vGlowPower;
	float				m_vGlowPowerSet;
	float				m_vGlowPowerStep;
	int					m_nGlowPowerTime;

	float				m_vGlowLevel;
	float				m_vGlowLevelSet;
	float				m_vGlowLevelStep;
	int					m_nGlowLevelTime;

	float				m_vFadeLevel;
	float				m_vFadeLevelSet;
	float				m_vFadeStep;
	int					m_nFadeTime;

	float				m_vMotionBlurLevel;
	float				m_vMotionBlurLevelSet;
	float				m_vMotionBlurStep;
	int					m_nMotionBlurTime;

	A3DPixelShader *	m_pGlow1Shader;
	A3DPixelShader *	m_pGlow2Shader;

public:
	CECFullGlowPresenter();

	void SetData(A3DPixelShader * pGlow1Shader, A3DPixelShader * pGlow2Shader);
	
	void SetGlowPower(float vStart, float vEnd, int nTime);
	void SetGlowLevel(float vStart, float vEnd, int nTime);

	void SetMotionBlur(float vStart, float vEnd, int nTime);
	void SetFade(float vStart, float vEnd, int nTime);

	void StopMotionBlur();
	void StopFade(bool bToWhite=true);
	
	void Reset();	// reset all parameters to default
	void Update(int nDeltaTime);

	// render states and pixelshader consts managing
	void PrepareGlow1Pass(float gaussin_weights[3]);
	void AfterGlow1Pass();
	void PrepareGlow2Pass(A3DCOLOR colorNow, float gaussin_weights[3]);
	void AfterGlow2Pass();

public:
	bool IsFading();
	bool IsMontionBluring();
};

typedef void (*LPRENDERFORBLOOM)(A3DViewport * pViewport, void * pArg);

class CECFullGlowRender
{
private:
	bool					m_bRenderToBack;		// flag indicates we are rendering into back target of the device instead of glow target
	bool					m_bCanDoFullGlow;		// flag indicates whether we can do full screen glow
	bool					m_bResourceLoaded;		// flag indicates whether resource has been loaded
	bool					m_bGlowOn;				// flag indicates we do glow now
	bool					m_bBloomOn;				// flag indicates we do bloom now
	bool					m_bWarpOn;				// flag indicates we do space warp
	bool					m_bFlareOn;				// flag indicates we do sun flare rendering
	bool					m_bSharpenOn;			// flag indicates we do sharpen or not
	bool					m_bBloomMotionBlur;		// flag indicates we use motion blur for bloom now

	A3DDevice *				m_pA3DDevice;
	int						m_nWidth;
	int						m_nHeight;
	
	FULLGLOW_TYPE			m_glowType;
	A3DCOLOR				m_color;

	CECFullGlowPresenter *	m_pPresenter;			// copy controller

	A3DRenderTarget *		m_pBloomTarget;			//	render target to hold bloom content
	A3DRenderTarget *		m_pBackTarget;			//	render target same size as back buffer
	A3DRenderTarget *		m_pGlowTarget1;			//	render target 1/4 size of back buffer
	A3DRenderTarget *		m_pGlowTarget2;			//	render target 1/4 size of back buffer

	A3DPixelShader *		m_pGlow1Shader;			// glow1 rendering shader
	A3DPixelShader *		m_pGlow2Shader;			// glow2 rendering shader
	A3DPixelShader *		m_pBloom1Shader;		// bloom x blur shader, same as glow1
	A3DPixelShader *		m_pBloom2Shader;		// bloom y blur shader, almost same as glow2
	
	A3DStream *				m_pStreamStretchCopy;	// stream used to stretch copy
	A3DTLVERTEX				m_vertsStretchCopy[4];		

	// we do a 5 pixel blur both on x-axis and y-axis
	float					m_GlowRadius;		// glow gaussin blur radius in pixels for original screen size
	float					m_GlowWeights[3];	// glow gaussin blur weights of the first 3 pixels from left to center
	float					m_BloomRadius;		// bloom gaussin blur radius in pixels for original screen size
	float					m_BloomWeights[3];	// bloom gaussin blur weights of the first 3 pixels from left to center

	A3DStream *				m_pStreamBlur;			// stream used to do bluring on both axis x & y
	A3DGLOWVERTEX			m_vertsBlur[4];			// vertex for blur stream
	A3DVertexShader *		m_pGaussinBlurVS;		// vertex shader for gaussin blur
	
	WORD					m_indices[6];

	LPRENDERFORBLOOM 		m_pRenderForBloom;		// render call back for bloom
	void *					m_pRenderForBloomArg;	// render for bloom callback argument

	// space warp objects.
	A3DPixelShader *		m_pTLWarpShader;		// TL warp render pixel shader
	A3DStream *				m_pTLWarpStream;		// stream used to show space warps using TL verts
	int						m_nMaxTLWarpVerts;		// max verts number of TL warp stream
	int						m_nMaxTLWarpIndices;	// max indices number of TL warp stream

	A3DPixelShader *		m_pWarpShader;			// warp render pixel shader
	A3DVertexShader *		m_pWarpVertexShader;	// 3D warp vertex shader
	A3DStream *				m_pWarpStream;			// stream used to show space warps using 3d verts
	int						m_nMaxWarpVerts;		// max verts number of warp stream
	int						m_nMaxWarpIndices;		// max indices number of warp stream

	A3DPixelShader *		m_pSharpenShader;		// sharpen render pixel shader
	A3DStream *				m_pStreamSharpen;		// stream used to do sharpen

	A3DTexture *			m_pBumpUnderWater;		
	
	//contrast
	bool					m_bContrast;
	A3DVertexShader*		m_pContrastVS;
	A3DPixelShader*			m_pContrastPS;
	float					m_fContrastPower;
	float					m_fContrastMidLum;

public:
	inline bool IsRenderToBack()					{ return m_bRenderToBack; }
	inline bool CanDoFullGlow()						{ return m_bCanDoFullGlow; }
	inline bool IsGlowOn()							{ return m_bGlowOn; }
	inline bool IsBloomOn()							{ return m_bBloomOn; }
	inline bool IsWarpOn()							{ return m_bWarpOn; }
	inline bool IsFlareOn()							{ return m_bFlareOn; }
	inline bool IsSharpenOn()						{ return m_bSharpenOn; }
	inline int GetWidth()							{ return m_nWidth; }
	inline int GetHeight() 							{ return m_nHeight; }
	A3DRenderTarget * GetBackTarget()				{ return m_pBackTarget; }

	//Contrast
	inline void SesContrastOn(bool bFlag)			{ m_bContrast = bFlag;}
	inline bool IsContrastOn()						{ return m_bContrast;}
	inline float GetContrastPower()					{ return m_fContrastPower;}
	inline void SetContrastPower(float fPower)		{ m_fContrastPower = fPower;}
	inline float GetContrastMidLum()				{ return m_fContrastMidLum;}
	inline void SetContrastMidLum(float fMidLum)	{ m_fContrastMidLum = fMidLum;}


protected:
	void CalculateGaussian(float radius, float weights[3]);
	void SetupVertexConsts(float cx, float cy, float dx, float dy);
	void SetupPixelConsts();

	bool LoadResource();
	bool ReleaseResource();

	bool CreateBumpMap();

	bool DoWarps(A3DViewport * pViewport);
	bool DoGlow(A3DViewport * pViewport);
	bool DoBloom(A3DViewport * pViewport);
	bool DoSharpen(A3DViewport * pViewport);

	inline bool GetNeedResource() { return m_bGlowOn || m_bBloomOn || m_bFlareOn || m_bWarpOn || m_bSharpenOn; }

	//Contrast
	bool DoContrast();

public:
	CECFullGlowRender();
	~CECFullGlowRender();

	bool Init(A3DDevice * pA3DDevice);
	bool Release();

	bool SetGlowOn(bool bFlag);
	bool SetBloomOn(bool bFlag, LPRENDERFORBLOOM pRenderForBloom, void * pArg);
	bool SetWarpOn(bool bFlag);
	bool SetFlareOn(bool bFlag);
	bool SetSharpenOn(bool bFlag);

	bool ReloadResource();

	bool Update(int nDeltaTime);

	bool BeginGlow();
	bool EndGlow(A3DViewport * pViewport);

	bool SetGlowType(FULLGLOW_TYPE type, DWORD value);

	void SetMotionBlur(float vStart, float vEnd, int nTime);
	void SetFade(float vStart, float vEnd, int nTime);

	void SetBloomMotionBlur(bool bFlag) { m_bBloomMotionBlur = bFlag; }
};

#endif //_EC_FULLGLOWRENDER_H_