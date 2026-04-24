/*
 * FILE: EC_SimpleBloomRender.h
 *
 * DESCRIPTION: Simple Bloom render for the Element Client
 *
 * CREATED BY: Hedi, 2006/7/21 
 *
 * HISTORY:
 *
 * Copyright (c) 2006 Archosaur Studio, All Rights Reserved.
 */

#ifndef _EC_SCREENRENDER_H_
#define _EC_SCREENRENDER_H_

#include <A3DTypes.h>
#include <A3DVertex.h>
#include <A3DTexture.h>

class A3DDevice;
class A3DRenderTarget;
class A3DStream;
class A3DTexture;
class A3DViewport;
class A3DCamera;

typedef void (*LPRENDERFORSCREEN)(A3DViewport * pViewport, void * pArg);

class CECScreenRender
{
private:
	A3DDevice *				m_pA3DDevice;
	int						m_nWidth;
	int						m_nHeight;
	
	A3DViewport *			m_pViewport;
	A3DCamera *				m_pShootingCam;

	A3DRenderTarget *		m_pScreenTargets[2];	//	render targets to hold screen content
	int						m_nCurScreen;			//	index of current frame screen
	int						m_nLastScreen;			//	index of last frame screen
	
	LPRENDERFORSCREEN 		m_pRenderForScreen;		// render call back for screen
	void *					m_pRenderForScreenArg;	// render for screen callback argument

	A3DTextureProxy			m_ScreenTexture;

protected:
	bool LoadResource();
	bool ReleaseResource();
	bool FlipScreens();

public:
	CECScreenRender();
	~CECScreenRender();

	bool Init(A3DDevice * pA3DDevice);
	bool Release();

	bool ReloadResource();

	bool Update(int nDeltaTime);
	bool RenderScreen(A3DViewport * pRestoreViewport, LPRENDERFORSCREEN pRenderForScreen, void * pArg);

	inline A3DCamera * GetShootingCam() { return m_pShootingCam; }
	inline A3DTextureProxy * GetScreenTexture() { return &m_ScreenTexture; }
	inline int GetWidth()				{ return m_nWidth; }
	inline int GetHeight()				{ return m_nHeight; }
};

#endif //_EC_SCREENRENDER_H_