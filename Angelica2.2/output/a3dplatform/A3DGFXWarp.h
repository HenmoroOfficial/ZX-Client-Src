/*
 * FILE: A3DGFXWarp.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Yaochunhui, 2012/4/18
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 Archosaur Studio, All Rights Reserved.
 */

#ifndef _A3D_GFXWARP_H_
#define _A3D_GFXWARP_H_

#include "A3DTypes.h"
#include "A3DGFXEx.h"

class A3DDevice;
class A3DGFXExMan;
class A3DPixelShader;
class A3DHLSL;
class A3DStream;
class A3DRenderTarget;
class A3DVertexDecl;

class A3DGFXWarp
{
public:
    A3DGFXWarp();
    ~A3DGFXWarp();

    bool Init(A3DDevice* pA3DDevice);
    void Release();
    
    bool Render(A3DViewport* pViewport, A3DRenderTarget* pFrameBuffer, A3DGFXExMan* pGfxMan);

private:
    A3DDevice*              m_pA3DDevice;
    A3DStream*				m_pTLWarpStream;		// stream used to show space warps using TL verts
    int						m_nMaxTLWarpVerts;		// max verts number of TL warp stream
    int						m_nMaxTLWarpIndices;	// max indices number of TL warp stream
    A3DStream* 				m_pWarpStream;
    int						m_nMaxWarpVerts;
    int						m_nMaxWarpIndices;
    A3DHLSL*                m_pHLSL3D;
    A3DHLSL*                m_pHLSL2D;
    A3DHLSL*                m_pHLSL3DMask;
    A3DHLSL*                m_pHLSL2DMask;
    A3DVertexDecl*          m_pVertDecl2D;
    A3DVertexDecl*          m_pVertDecl3D;

    bool RenderGFXElementsTL(A3DViewport* pViewport, GfxElementList& elementList);
    bool RenderGFXElements(A3DViewport* pViewport, GfxElementList& elementList);
};

#endif