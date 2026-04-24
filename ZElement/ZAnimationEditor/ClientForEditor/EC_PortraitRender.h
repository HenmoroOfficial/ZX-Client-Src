/*
 * FILE: EC_PortraitRender.h
 *
 * DESCRIPTION: a class for portrait rendering
 *
 * CREATED BY: Hedi, 2005/2/25
 *
 * HISTORY:
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _EC_PORTRAITRENDER_H_
#define _EC_PORTRAITRENDER_H_

#include "A3DTypes.h"
#include "A3DDevObject.h"
#ifdef BACK_VERSION
#include <d3d8.h>
#else
#include <d3d9.h>
#endif

class A3DDevice;
class A3DCamera;
class A3DTexture;
class A3DViewport;
class A3DRenderTarget;
class A2DSprite;


// portrait rendering callback
typedef bool (*LPRENDERFORPORTRAIT)(A3DViewport *, void * pArg);

// class represent portrait renderer
class CECPortraitRender	: public A3DDevObject
{
private:
	int					m_nPortraitMapSize;			// portrait map size
	A3DViewport *		m_pPortraitMapViewport;		// viewport to render the portrait map
	A3DCamera *			m_pPortraitMapCamera;		// camera to do rendering of portrait
	A3DRenderTarget *	m_pPortraitMapTarget;		// render target to render the portrait
#ifdef BACK_VERSION
	IDirect3DSurface8 *	m_pSysBackTarget;			// render target that can be locked, which mainly in sys-mem
#else
	A3DTexture*			m_pSysBackTarget;
#endif
	A3DFORMAT			m_fmtTexture;				// the texture format for portrait render

public:
	inline int GetPortraitMapSize()					{ return m_nPortraitMapSize; }
	inline A3DFORMAT GetTextureFormat()				{ return m_fmtTexture; }
	inline A3DViewport * GetPortraitViewport()		{ return m_pPortraitMapViewport; }

public:
	CECPortraitRender();
	~CECPortraitRender();

	bool Init(int nPortraitMapSize);
	bool Release();

	bool RenderPortraitToSprite(A3DViewport * pViewport, A2DSprite * pSprite, LPRENDERFORPORTRAIT pfnRenderForPortrait, LPVOID pArg);

	//	Before device reset
	bool BeforeDeviceReset();
	//	After device reset
	bool AfterDeviceReset();
};

#endif//_EC_PORTRAITRENDER_H_