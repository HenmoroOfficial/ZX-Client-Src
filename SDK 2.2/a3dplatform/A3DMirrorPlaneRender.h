/*
 * FILE: d:\Dev\Projects2.2\A3DPlatform\A3DMirrorPlane.h
 *
 * DESCRIPTION: The renderer used to draw mirror plane objects.
 *
 * CREATED BY: LiChen, 2012/8/9
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 Perfect World Entertainment Inc, All Rights Reserved.
 */

#ifndef _A3D_MIRROR_PLANE_RENDER_H_
#define _A3D_MIRROR_PLANE_RENDER_H_


#include "A3DMirrorRender.h"


class A3DEngine;
class A3DVertexDecl;
class A3DHLSL;


class A3DMirrorPlaneRender
{
public:
	friend class A3DMirrorPlane;

public:
	A3DMirrorPlaneRender();
	explicit A3DMirrorPlaneRender( A3DMirrorPlaneRender& Other );
	~A3DMirrorPlaneRender();

	bool Init( A3DEngine* pA3DEngine );

	void Release();

	// Registers a mirror plane which is ready for rendering.
	bool RegisterRenderMirrorPlane( A3DViewport* pViewport, A3DMirrorPlane* pMirrorPlane );

	// Renders all mirror planes.
	void Render( A3DViewport* pViewport, LPRENDERFORMIRROR fnRenderMirrorCallback, void* pParam );

private:
	// Creates all device resources used to render the mirror plane mesh.
	bool _CreateDeviceResources();

	// Release all device resources used to render the mirror plane mesh.
	void _ReleaseDeviceResources();

protected:
	A3DEngine*						m_pA3DEngine;
	A3DMirrorRender*				m_pMirrorRender;					// Renderer used to draw reflection target.

	APtrArray<A3DMirrorPlane*>		m_PtrArrayMirrorPlanes;				// All mirror planes which are ready for rendering.

	A3DHLSL*						m_pShaderVertexColor;				// mesh shader with vertex color
	A3DHLSL*						m_pShaderVertexColorWithAlphaMask;	// mesh shader with vertex color and alpha mask texture
	A3DHLSL*						m_pShaderLightMap;					// mesh shader with light map
	A3DHLSL*						m_pShaderLightMapWithAlphaMask;		// mesh shader with light map and alpha mask texture

	A3DVertexDecl*					m_pVertDecl;						// vertex declaration
};


#endif // #ifndef _A3D_MIRROR_PLANE_RENDER_H_
