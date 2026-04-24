/*
 * FILE: A3DGFXWarp.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Yaochunhui, 2012/4/18
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 Archosaur Studio, All Rights Reserved.
 */

#include "A3D.h"
#include "A3DGFXWarp.h"
#include "A3DRenderTargetMan.h"
#include "A3DHLSL.h"
#include "A3DGFXElement.h"
#include "A3DGFXEx.h"
#include "A3DGFXExMan.h"

static const D3DVERTEXELEMENT9 s_aVertexDecl2D[] =
{
    {0, 0,  D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0},
    {0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
    {0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
    {0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2},
    D3DDECL_END()
};

static const D3DVERTEXELEMENT9 s_aVertexDecl3D[] =
{
    {0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
    {0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
    {0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
    {0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2},
    D3DDECL_END()
};

static const int MAX_FACE = 250;

A3DGFXWarp::A3DGFXWarp()
{
    m_pA3DDevice    = NULL;
    m_pTLWarpStream = NULL;	
    m_nMaxTLWarpVerts   = 0;	
    m_nMaxTLWarpIndices = 0;
    m_pWarpStream       = NULL;
    m_nMaxWarpVerts     = 0;
    m_nMaxWarpIndices   = 0;
    m_pHLSL2D           = NULL;
    m_pHLSL3D           = NULL;
    m_pHLSL2DMask       = NULL;
    m_pHLSL3DMask       = NULL;
    m_pVertDecl2D       = NULL;
    m_pVertDecl3D       = NULL;
}

A3DGFXWarp::~A3DGFXWarp()
{

}

bool A3DGFXWarp::Init(A3DDevice* pA3DDevice)
{
    m_pA3DDevice = pA3DDevice;
    m_nMaxTLWarpVerts = MAX_FACE * 4;
    m_nMaxTLWarpIndices = MAX_FACE * 6;

    m_pVertDecl2D = new A3DVertexDecl;
    if (!m_pVertDecl2D->Init(pA3DDevice, s_aVertexDecl2D))
    {
        a_LogOutput(1, "A3DGFXWarp::Init(), failed to create 2d vertex declaration!");
        return false;
    }

    m_pTLWarpStream = new A3DStream();

    if (!m_pTLWarpStream->Init(m_pA3DDevice, sizeof(A3DTLWARPVERTEX), A3DFVF_A3DTLWARPVERT, 
        m_nMaxTLWarpVerts, m_nMaxTLWarpIndices, A3DSTRM_REFERENCEPTR, A3DSTRM_STATIC) )
    {
        a_LogOutput(1, "A3DGFXWarp::Init(), failed to create warp stream!");
        return false;
    }

    WORD * pIndices;
    if (!m_pTLWarpStream->LockIndexBuffer(0, 0, (BYTE **)&pIndices, 0))
    {
        a_LogOutput(1, "A3DGFXWarp::Init(), failed to lock warp stream's index buffer!");
        return false;
    }
    for (int i = 0; i < MAX_FACE; i++)
    {
        pIndices[i * 6 + 0] = i * 4 + 0;		
        pIndices[i * 6 + 1] = i * 4 + 1;		
        pIndices[i * 6 + 2] = i * 4 + 2;		
        pIndices[i * 6 + 3] = i * 4 + 2;		
        pIndices[i * 6 + 4] = i * 4 + 1;		
        pIndices[i * 6 + 5] = i * 4 + 3;		
    }
    m_pTLWarpStream->UnlockIndexBuffer();

    m_nMaxWarpVerts = MAX_FACE * 4;
    m_nMaxWarpIndices = MAX_FACE * 6;

    m_pVertDecl3D = new A3DVertexDecl;
    if (!m_pVertDecl3D->Init(pA3DDevice, s_aVertexDecl3D))
    {
        a_LogOutput(1, "A3DGFXWarp::Init(), failed to create 3d vertex declaration!");
        return false;
    }

    m_pWarpStream = new A3DStream();
    if (!m_pWarpStream->Init(m_pA3DDevice, sizeof(A3DWARPVERTEX), A3DFVF_A3DWARPVERT, 
        m_nMaxWarpVerts, m_nMaxWarpIndices, A3DSTRM_REFERENCEPTR, A3DSTRM_STATIC) )
    {
        a_LogOutput(1, "A3DGFXWarp::Init(), failed to create warp stream!");
        return false;
    }

    if (!m_pWarpStream->LockIndexBuffer(0, 0, (BYTE **)&pIndices, 0))
    {
        a_LogOutput(1, "A3DGFXWarp::Init(), failed to lock warp stream's index buffer!");
        return false;
    }
    for (int i = 0; i < MAX_FACE; i++)
    {
        pIndices[i * 6 + 0] = i * 4 + 0;		
        pIndices[i * 6 + 1] = i * 4 + 1;		
        pIndices[i * 6 + 2] = i * 4 + 2;		
        pIndices[i * 6 + 3] = i * 4 + 2;		
        pIndices[i * 6 + 4] = i * 4 + 1;		
        pIndices[i * 6 + 5] = i * 4 + 3;		
    }
    m_pWarpStream->UnlockIndexBuffer();

    AString strShaderFile = "shaders\\2.2\\HLSL\\GFX\\Warp.hlsl";

	AString strMacro2D, strMacro3D;

	strMacro2D = "";
	strMacro3D = "_3D_";

    m_pHLSL2D = m_pA3DDevice->GetA3DEngine()->GetA3DHLSLMan()->LoadShader(NULL, NULL, strShaderFile, "ps_main", strMacro2D, 0);
    if (!m_pHLSL2D)
    {
        a_LogOutput(1, "A3DGFXWarp::Init(), failed to load 2d shader");
        return false;
    }
    m_pHLSL3D = m_pA3DDevice->GetA3DEngine()->GetA3DHLSLMan()->LoadShader(strShaderFile, "vs_main", strShaderFile, "ps_main", strMacro3D, 0);
    if (!m_pHLSL3D)
    {
        a_LogOutput(1, "A3DGFXWarp::Init(), failed to load 3d shader");
        return false;
    }
    m_pHLSL2DMask = m_pA3DDevice->GetA3DEngine()->GetA3DHLSLMan()->LoadShader(NULL, NULL, strShaderFile, "ps_main_mask", strMacro2D, 0);
    if (!m_pHLSL2D)
    {
        a_LogOutput(1, "A3DGFXWarp::Init(), failed to load 2d shader");
        return false;
    }
    m_pHLSL3DMask = m_pA3DDevice->GetA3DEngine()->GetA3DHLSLMan()->LoadShader(strShaderFile, "vs_main", strShaderFile, "ps_main_mask", strMacro3D, 0);
    if (!m_pHLSL3D)
    {
        a_LogOutput(1, "A3DGFXWarp::Init(), failed to load 3d shader");
        return false;
    }
    return true;
}

void A3DGFXWarp::Release()
{
    if (m_pA3DDevice)
    {
        if (m_pHLSL2D)
        {
            m_pA3DDevice->GetA3DEngine()->GetA3DHLSLMan()->ReleaseShader(m_pHLSL2D);
            m_pHLSL2D = NULL;
        }
        if (m_pHLSL3D)
        {
            m_pA3DDevice->GetA3DEngine()->GetA3DHLSLMan()->ReleaseShader(m_pHLSL3D);
            m_pHLSL3D = NULL;
        }
        if (m_pHLSL2DMask)
        {
            m_pA3DDevice->GetA3DEngine()->GetA3DHLSLMan()->ReleaseShader(m_pHLSL2DMask);
            m_pHLSL2DMask = NULL;
        }
        if (m_pHLSL3DMask)
        {
            m_pA3DDevice->GetA3DEngine()->GetA3DHLSLMan()->ReleaseShader(m_pHLSL3DMask);
            m_pHLSL3DMask = NULL;
        }
    }
    A3DRELEASE(m_pTLWarpStream);
    A3DRELEASE(m_pWarpStream);
    A3DRELEASE(m_pVertDecl2D);
    A3DRELEASE(m_pVertDecl3D);
}

bool A3DGFXWarp::RenderGFXElementsTL(A3DViewport* pViewport, GfxElementList& elementList)
{
    int iWidth = pViewport->GetParam()->Width;
    int iHeight = pViewport->GetParam()->Height;

    float	rw = 1.0f / iWidth;
    float	rh = 1.0f / iHeight;

    m_pTLWarpStream->Appear(0, false);
    m_pVertDecl2D->Appear();

    for (int i = 0; i < (int)elementList.size(); i++)
    {
        A3DGFXElement * pElement = elementList[i];
        if (pElement && pElement->IsTLVert() && pElement->GetTexture())
        {
            A3DTLWARPVERTEX * pVerts;
            if( !m_pTLWarpStream->LockVertexBufferDynamic(0, 0, (BYTE **)&pVerts, 0) )
            {
                break;
            }
            int nVerts = pElement->DrawToBuffer(pViewport, pVerts, m_nMaxTLWarpVerts, rw, rh);
            m_pTLWarpStream->UnlockVertexBufferDynamic();

            pElement->GetTexture()->Appear(0);
            m_pA3DDevice->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, nVerts, 0, nVerts >> 1);
            pElement->GetTexture()->Disappear(0);
        }
    }

    return true;
}

bool A3DGFXWarp::RenderGFXElements(A3DViewport* pViewport, GfxElementList& elementList)
{
    m_pWarpStream->Appear(0, false);
    m_pVertDecl3D->Appear();

    for (int i = 0; i < (int)elementList.size(); i++)
    {
        A3DGFXElement * pElement = elementList[i];
        if( pElement && !pElement->IsTLVert() && pElement->GetTexture())
        {
            A3DWARPVERTEX * pVerts;
            if( !m_pWarpStream->LockVertexBufferDynamic(0, 0, (BYTE **)&pVerts, 0) )
            {
                break;
            }
            int nVerts = pElement->DrawToBuffer(pViewport, pVerts, m_nMaxWarpVerts);
            m_pWarpStream->UnlockVertexBufferDynamic();

            pElement->GetTexture()->Appear(0);
            m_pA3DDevice->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, nVerts, 0, nVerts >> 1);
            pElement->GetTexture()->Disappear(0);
        }
    }


    return true;
}

bool A3DGFXWarp::Render(A3DViewport* pViewport, A3DRenderTarget* pFrameBuffer, A3DGFXExMan* pGfxMan)
{
    if (!pGfxMan || pGfxMan->GetWarpEleList().empty())
        return true;

    int iWidth = pFrameBuffer->GetWidth();
    int iHeight = pFrameBuffer->GetHeight();

    A3DRenderTarget* pRenderTarget = m_pA3DDevice->GetRenderTargetMan()->RentRenderTargetColor(
        pFrameBuffer->GetWidth(), pFrameBuffer->GetHeight(), pFrameBuffer->GetFormat().fmtTarget);

    if (!pRenderTarget)
	{
		a_LogOutput(1, "A3DGFXWarp::Render, failed to rent renderTarget %d", pFrameBuffer->GetFormat().fmtTarget);
        return false;
	}

    // Copy frame buffer to new render target
    if (FAILED(m_pA3DDevice->StretchRect(
        pFrameBuffer->GetTargetSurface()->m_pD3DSurface, NULL, pRenderTarget->GetTargetSurface()->m_pD3DSurface, NULL, A3DTEXF_NONE)))
    {
		a_LogOutput(1, "A3DGFXWarp::Render, failed to StretchRect");
        m_pA3DDevice->GetRenderTargetMan()->ReturnRenderTarget(pRenderTarget);
        return false;
    } 

    // create mask render target
    A3DRenderTarget* pRenderTargetMask = m_pA3DDevice->GetRenderTargetMan()->RentRenderTargetColor(
        pFrameBuffer->GetWidth(), pFrameBuffer->GetHeight(), A3DFMT_A8R8G8B8);

    if (!pRenderTargetMask)
    {
		a_LogOutput(1, "A3DGFXWarp::Render, failed to rent TargetMask %d", A3DFMT_A8R8G8B8);
        m_pA3DDevice->GetRenderTargetMan()->ReturnRenderTarget(pRenderTarget);
        return false;
    }

    GfxElementList& elementList = pGfxMan->GetWarpEleList();
    pGfxMan->LockWarpEleList();

    // set render states
    m_pA3DDevice->SetLightingEnable(false);
    m_pA3DDevice->SetZTestEnable(true);
    m_pA3DDevice->SetZWriteEnable(false);
    m_pA3DDevice->SetFogEnable(false);
    m_pA3DDevice->SetTextureAddress(1, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
    m_pA3DDevice->SetTextureFilterType(1, A3DTEXF_LINEAR);
    m_pA3DDevice->SetTextureFilterType(2, A3DTEXF_LINEAR);
    A3DCULLTYPE oldCull = m_pA3DDevice->GetFaceCull();
    m_pA3DDevice->SetFaceCull(A3DCULL_NONE);
    

    // draw mask
    pRenderTargetMask->ApplyToDevice();

    m_pA3DDevice->Clear(D3DCLEAR_TARGET, A3DCOLORRGBA(0,0,0,0), 1.0f, 0);

    m_pHLSL2DMask->Appear(APPEAR_SETSHADERONLY);

    RenderGFXElementsTL(pViewport, elementList);

    A3DCameraBase * pCamera = pViewport->GetCamera();

    m_pA3DDevice->SetWorldMatrix(IdentityMatrix());
    A3DMATRIX4 matScale = IdentityMatrix();
    matScale._11 = 0.5f;
    matScale._22 = -0.5f;
    matScale._41 = 0.5f;
    matScale._42 = 0.5f;
    A3DMATRIX4 matProjectedView;
    matProjectedView = pCamera->GetVPTM() * matScale;
    m_pHLSL3DMask->SetConstantMatrix("g_matTex", matProjectedView);
    m_pHLSL3DMask->SetConstantMatrix("g_matViewProj", pCamera->GetVPTM());
    m_pHLSL3DMask->Appear();

    RenderGFXElements(pViewport, elementList);

    pRenderTargetMask->WithdrawFromDevice();

    // Apply new render target
    pRenderTarget->ApplyToDevice();

	float fScreenWidth = (float)pViewport->GetParam()->Width;
	float fScreenHeight = (float)pViewport->GetParam()->Height;
	A3DVECTOR4 g_vScreenSize(fScreenWidth, fScreenHeight, 0.5f / fScreenWidth, 0.5f / fScreenHeight);

    // first TL space warps.
    pFrameBuffer->AppearAsTexture(1);
    pRenderTargetMask->AppearAsTexture(2);

    m_pHLSL2D->Appear(APPEAR_SETSHADERONLY);
	m_pA3DDevice->SetPixelShaderConstants(0, &g_vScreenSize, 1);

    RenderGFXElementsTL(pViewport, elementList);

    // then do 3d warps.
    m_pHLSL3D->SetConstantMatrix("g_matTex", matProjectedView);
    m_pHLSL3D->SetConstantMatrix("g_matViewProj", pCamera->GetVPTM());
	//m_pHLSL3D->SetupVec4("g_vScreenSize", g_vScreenSize);
    m_pHLSL3D->Appear();
    pFrameBuffer->AppearAsTexture(1);
    pRenderTargetMask->AppearAsTexture(2);
	m_pA3DDevice->SetPixelShaderConstants(0, &g_vScreenSize, 1);

    RenderGFXElements(pViewport, elementList);

    pFrameBuffer->DisappearAsTexture(1);
    pRenderTargetMask->DisappearAsTexture(2);
    m_pHLSL3D->Disappear();

    pGfxMan->UnlockWarpEleList();
    pGfxMan->ClearWarpEleList();

    m_pA3DDevice->SetFaceCull(oldCull);
    m_pA3DDevice->SetLightingEnable(true);
    m_pA3DDevice->SetZTestEnable(true);
    m_pA3DDevice->SetZWriteEnable(true);
    m_pA3DDevice->SetTextureAddress(1, A3DTADDR_WRAP, A3DTADDR_WRAP);
    m_pA3DDevice->SetTextureFilterType(1, A3DTEXF_LINEAR);
    m_pA3DDevice->SetTextureFilterType(2, A3DTEXF_LINEAR);

	pRenderTarget->WithdrawFromDevice();

    // copy back to frame buffer
    if(FAILED(m_pA3DDevice->StretchRect(pRenderTarget->GetTargetSurface()->m_pD3DSurface, NULL, 
        pFrameBuffer->GetTargetSurface()->m_pD3DSurface, NULL, A3DTEXF_NONE)))
	{
		a_LogOutput(1, "A3DGFXWarp::Render, failed to StretchRect2");
		m_pA3DDevice->GetRenderTargetMan()->ReturnRenderTarget(pRenderTarget);
		m_pA3DDevice->GetRenderTargetMan()->ReturnRenderTarget(pRenderTargetMask);
		return false;	 
	}

    m_pA3DDevice->GetRenderTargetMan()->ReturnRenderTarget(pRenderTarget);
    m_pA3DDevice->GetRenderTargetMan()->ReturnRenderTarget(pRenderTargetMask);
    return true;

}