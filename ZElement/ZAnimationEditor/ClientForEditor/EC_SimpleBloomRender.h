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

#ifndef _EC_SIMPLEBLOOMRENDER_H_
#define _EC_SIMPLEBLOOMRENDER_H_

#include <A3DTypes.h>
#include <A3DVertex.h>

class A3DDevice;
class A3DRenderTarget;
class A3DStream;
class A3DTexture;
class A3DViewport;

// Vertex for simple bloom area
#define A3DSBVERT_FVF		D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX2
struct A3DSBVERTEX
{
	A3DSBVERTEX() {}
	A3DSBVERTEX(const A3DVECTOR4& _pos, A3DCOLOR _diffuse, float _u1, float _v1, float _u2, float _v2)
	{
		pos = _pos;
		diffuse = _diffuse;
		u1 = _u1;
		v1 = _v1;
		u2 = _u2;
		v2 = _v2;
	}
	 
	A3DVECTOR4	pos;			//	Position on screen
	A3DCOLOR	diffuse;		//	Color
	float		u1, v1;			//	Texture coords of center
	float		u2, v2;			//	Texture coords of offset one
};

typedef void (*LPRENDERFORBLOOM)(A3DViewport * pViewport, void * pArg);

class CECSimpleBloomRender
{
private:
	A3DDevice *				m_pA3DDevice;
	int						m_nWidth;
	int						m_nHeight;
	
	bool					m_bForcePow2;			//	flag indicates whether we use 512 * 512 textures
	A3DRenderTarget *		m_pBloomTarget;			//	render target to hold bloom content
	A3DRenderTarget *		m_pSmallTarget;			//	render target 1/4 size of back buffer
	
	A3DStream *				m_pStreamDownSize;		// stream used to a do down size of bloom target to small target
	A3DSBVERTEX				m_vertDownSize[4];
	A3DStream *				m_pStreamUpSize;		// stream used to a do up size of small target to screen
	A3DSBVERTEX				m_vertUpSize[4];
	
	WORD					m_indices[6];

	LPRENDERFORBLOOM 		m_pRenderForBloom;		// render call back for bloom
	void *					m_pRenderForBloomArg;	// render for bloom callback argument

protected:
	bool LoadResource();
	bool ReleaseResource();

public:
	CECSimpleBloomRender();
	~CECSimpleBloomRender();

	bool Init(A3DDevice * pA3DDevice);
	bool Release();

	bool ReloadResource();

	bool Update(int nDeltaTime);

	bool DoSimpleBloom(A3DViewport * pViewport, LPRENDERFORBLOOM pRenderForBloom, void * pArg);
};

#endif //_EC_SIMPLEBLOOMRENDER_H_