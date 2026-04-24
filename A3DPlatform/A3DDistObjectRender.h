/*
 * FILE: A3DDistObjectRender.h
 *
 * DESCRIPTION: Class for rendering distant scenery objects
 *
 * CREATED BY: Yaochunhui, 2012/10/23
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
 */

#ifndef _A3DDISTOBJECTRENDER_H_
#define _A3DDISTOBJECTRENDER_H_

#include "A3DVertex.h"
#include "A3DGeometry.h"
#include "AString.h"
#include "AArray.h"
#include "A3DVertexShader.h"
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

class A3DDevice;
class A3DCameraBase;
class A3DViewport;
class A3DTexture;
class A3DStream;
class A3DDistantObject;
class A3DDistantBoard;
class A3DHLSL;
class A3DVertexDecl;
class A3DRenderTarget;

///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//
//	Class A3DDistantObjectRender
//
///////////////////////////////////////////////////////////////////////////

class A3DDistantObjectRender
{
public:

    A3DDistantObjectRender();

    virtual ~A3DDistantObjectRender();

public:		//	Attributes

public:		//	Operations
    
    virtual bool Init(A3DDevice* pA3DDevice);

    virtual void Release();		//	Release

    virtual bool RegisterObject(A3DDistantObject* pObject);
    virtual bool Render(A3DViewport* pViewport, bool bMaskSky);

    virtual void ClearList();

    // quality in [0..2]
    int GetQuality() const { return m_nQuality; }
    void SetQuality(int nQuality) { m_nQuality = nQuality; }

    bool GetSortByCamera() const { return m_bSortByCamera; }
    void SetSortByCamera(bool bSort) { m_bSortByCamera = bSort; }
    
protected:	//	Attributes
	bool					m_bInited;
    AArray<A3DDistantBoard*> m_aBoards;
    int m_nQuality;
    A3DDevice* m_pA3DDevice;
    bool m_bSortByCamera;
    A3DVertexDecl* m_pVertDecl;
    A3DVertexDecl* m_pVertDeclMask;

    enum { QUALITY_COUNT = 3 };
    A3DHLSL*    m_pHLSLBoard[QUALITY_COUNT][2];
    A3DHLSL*    m_pPSMaskSky;
    A3DTexture* m_pTexUnitNorm;

    void SortBoards(A3DViewport* pViewport, AArray<A3DDistantBoard*>& aBoards);
    bool MaskSky(A3DViewport* pViewport);
    bool MaskDepth(A3DViewport* pViewport, A3DRenderTarget* pDepthBuffer);
    void RenderList(A3DViewport* pViewport, A3DHLSL* pHLSL, const AArray<A3DDistantBoard*>& aRenderBoards);
    void SetRenderStates();
};

A3DDistantObjectRender* AfxGetA3DDistantObjectRender();

#endif