#include "A3DDistObjectRender.h"
#include "A3DDistObject.h"
#include "A3DHLSL.h"
#include "A3DFuncs.h"
#include "A3DDevice.h"
#include "A3DEngine.h"
#include "A3DTexture.h"
#include "A3DTextureMan.h"
#include "A3DStream.h"
#include "A3DVertexShader.h"
#include "A3DViewport.h"
#include "A3DCameraBase.h"
#include "A3DRenderTarget.h"
#include "a3drendertargetman.h"
#include "A3DGFXExMan.h"
#include "A3DEnvironment.h"

extern ALog g_A3DErrLog;

static const D3DVERTEXELEMENT9 aVertDecl[] = 
{
    {0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
    {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
    {0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
    {0, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0},

    D3DDECL_END()
};

static const D3DVERTEXELEMENT9 aVertDeclMask[] = 
{
    {0, 0,  D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0},
    {0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},

    D3DDECL_END()
};

static A3DDistantObjectRender g_A3DDistantObjectRender;

A3DDistantObjectRender* AfxGetA3DDistantObjectRender()
{
    return &g_A3DDistantObjectRender;
}

static const char* szShaderBoard = "shaders\\2.2\\HLSL\\Distant\\distant_board.hlsl";
static const char* szShaderMaskSky = "shaders\\2.2\\HLSL\\Distant\\mask_sky_ps.hlsl";

A3DDistantObjectRender::A3DDistantObjectRender()
{
	m_bInited = false;
    m_pA3DDevice = NULL;
    for (int i = 0; i < QUALITY_COUNT; i++)
    {
        m_pHLSLBoard[i][0] = NULL;
        m_pHLSLBoard[i][1] = NULL;
    }

    m_pTexUnitNorm = NULL;
    m_nQuality = 2;
    m_pPSMaskSky = NULL;
    m_bSortByCamera = false;

    m_pVertDecl = NULL;
    m_pVertDeclMask = NULL;
}

A3DDistantObjectRender::~A3DDistantObjectRender()
{

}

bool A3DDistantObjectRender::Init(A3DDevice* pA3DDevice)
{
    m_pA3DDevice = pA3DDevice;
    
    m_pTexUnitNorm = A3DTexture::CreateColorTexture(pA3DDevice, 32, 32, A3DCOLORRGBA(127, 127, 255, 0));

    A3DEnvironment* pEnv = m_pA3DDevice->GetA3DEngine()->GetA3DEnvironment();
    A3DCCDBinder* pBinder = pEnv->GetCCDBinder();

    const char* szMacros[] = { "bHighQuality", "bPixelFog" };
    const char* szMacrosFixed[] = { "bFixedDepth", "bHighQuality", "bPixelFog" };
    
    m_pVertDecl = new A3DVertexDecl;
    
    if (!m_pVertDecl || !m_pVertDecl->Init(pA3DDevice, aVertDecl))
    {
		Release();
        g_A3DErrLog.Log("A3DDistantObjectRender::Init(), failed to create vertex declaration");
        return false;
    }

    m_pVertDeclMask = new A3DVertexDecl;

    if (!m_pVertDeclMask || !m_pVertDeclMask->Init(pA3DDevice, aVertDeclMask))
    {
		Release();
        g_A3DErrLog.Log("A3DDistantObjectRender::Init(), failed to create mask vertex declaration");
        return false;
    }
	
	int nMaxQuality = QUALITY_COUNT;
	if(!m_pA3DDevice->TestVertexShaderVersion(3, 0))
		nMaxQuality = QUALITY_COUNT - 1;

    for (int i = 0; i < nMaxQuality; i++)
    {

        m_pHLSLBoard[i][0] = CreateA3DHLSL_Common(szShaderBoard, "vs_main", szShaderBoard, "ps_main", i, szMacros, pA3DDevice, aVertDecl);

        if (!m_pHLSLBoard[i][0])
        {
			Release();
            g_A3DErrLog.Log("A3DDistantObjectRender::Init(), failed to load shader [%d, 0]!", i);
			return false;
        }
        else
        {
            m_pHLSLBoard[i][0]->BindCommConstData(pBinder);
        }
        m_pHLSLBoard[i][1] = CreateA3DHLSL_Common(szShaderBoard, "vs_main", szShaderBoard, "ps_main", i + 1, szMacrosFixed, pA3DDevice, aVertDecl);

        if (!m_pHLSLBoard[i][1])
        {
			Release();
            g_A3DErrLog.Log("A3DDistantObjectRender::Init(), failed to load shader [%d, 1]!", i);
			return false;
        }
        else
        {
            m_pHLSLBoard[i][1]->BindCommConstData(pBinder);
        }
    }

    m_pPSMaskSky = CreateA3DHLSL_Common(NULL, NULL, szShaderMaskSky, "ps_main", 0, NULL, pA3DDevice, aVertDeclMask);

    if (!m_pPSMaskSky)
    {
		Release();
        g_A3DErrLog.Log("A3DDistantObjectRender::Init(), failed to load mask shader!");
        return false;
    }

	m_bInited = true;
    return true;
}

void A3DDistantObjectRender::Release()
{
    if (!m_pA3DDevice)
        return;
    for (int i = 0; i < QUALITY_COUNT; i++)
    {
        ReleaseA3DHLSL_Common(m_pA3DDevice, m_pHLSLBoard[i][0]);
        ReleaseA3DHLSL_Common(m_pA3DDevice, m_pHLSLBoard[i][1]);
    }
    ReleaseA3DHLSL_Common(m_pA3DDevice, m_pPSMaskSky);
    A3DRELEASE(m_pTexUnitNorm);
    A3DRELEASE(m_pVertDecl);
    A3DRELEASE(m_pVertDeclMask);
    m_pA3DDevice = NULL;
	m_bInited = false;
}

bool A3DDistantObjectRender::RegisterObject(A3DDistantObject* pObject)
{
	if( !m_bInited)
		return false;

    if (pObject->GetType() == A3DDistantObject::TYPE_BOARD)
    {
        A3DDistantBoard* pDistObject = (A3DDistantBoard*)pObject;
        m_aBoards.Add(pDistObject);
    }
    return true;
}

bool A3DDistantObjectRender::Render(A3DViewport* pViewport, bool bMaskSky)
{
	if( !m_bInited)
		return false;

    if (!m_aBoards.GetSize())
        return true;

        
    // 计算Z范围
    float fZFar = pViewport->GetCamera()->GetZBack();
    float fZNear = pViewport->GetCamera()->GetZFront();

    A3DMATRIX4 matView = pViewport->GetCamera()->GetViewTM();

    A3DFrustum Frustum;
    Frustum.Init(6);
    Frustum = *pViewport->GetCamera()->GetWorldFrustum();

    Frustum.EnablePlane(A3DFrustum::VF_FAR, false);

    float fNewZFar = 0;
    float fNewZNear = 100000;
    
    A3DVECTOR3 vCameraDir = pViewport->GetCamera()->GetDir();

    // 中景
    AArray<A3DDistantBoard*> aRenderBoardsMiddle(m_aBoards.GetSize(), 10);
    // 远景
    AArray<A3DDistantBoard*> aRenderBoardsFar(m_aBoards.GetSize(), 10);

    for (int i = 0; i < m_aBoards.GetSize(); i++)
    {
        if (DotProduct(m_aBoards[i]->GetDir(), vCameraDir) > 0)
            continue;

        const A3DAABB& aabb = m_aBoards[i]->GetAABB();
        if (Frustum.AABBInFrustum(aabb) >= 0)
        {
            A3DVECTOR3 vCorners[8];
            const A3DAABB& obb = m_aBoards[i]->GetOBB();
            obb.GetVertices(vCorners, NULL, false);
            A3DMATRIX4 matWorldView = m_aBoards[i]->GetAbsTransform() * matView;
            float fZFarObject = 0;
            float fZNearObject = 100000;
            for (int iVert = 0; iVert < 8; iVert++)
            {
                A3DVECTOR3 vViewSpace = vCorners[iVert] * matWorldView;
                if (vViewSpace.z > fZFarObject)
                    fZFarObject = vViewSpace.z;
                if (vViewSpace.z > 10 && vViewSpace.z < fZNearObject)
                    fZNearObject = vViewSpace.z;
            }
            // 不要中景了 全部都是远景
            //if (fZFarObject > fZFar)
            {
                aRenderBoardsFar.Add(m_aBoards[i]);
                if (fZFarObject > fNewZFar)
                    fNewZFar = fZFarObject;
                if (fZNearObject < fNewZNear)
                    fNewZNear = fZNearObject;
            }
            //else
            //{
            //    aRenderBoardsMiddle.Add(m_aBoards[i]);
            //}
        }
    }
    
    if (aRenderBoardsMiddle.GetSize() == 0 && aRenderBoardsFar.GetSize() == 0)
        return true;

    // 保存渲染状态
    bool bAlphaTestEnable = m_pA3DDevice->GetAlphaTestEnable();
    DWORD dwAlphaRef = m_pA3DDevice->GetAlphaRef();
    bool bAlphaBlendEnable = m_pA3DDevice->GetAlphaBlendEnable();
    A3DCMPFUNC alphaFunc = m_pA3DDevice->GetAlphaFunction();
    A3DCMPFUNC ZFunc = (A3DCMPFUNC)m_pA3DDevice->GetDeviceRenderState(D3DRS_ZFUNC);
    bool bZTestEnable = m_pA3DDevice->GetZTestEnable();
    bool bZWriteEnable = m_pA3DDevice->GetZWriteEnable();
    DWORD dwClipPlane = m_pA3DDevice->GetDeviceRenderState(D3DRS_CLIPPLANEENABLE);

    SetRenderStates();

    

    if (aRenderBoardsFar.GetSize() > 0)
    {
        a_Clamp(fNewZNear, 10.0f, 300.0f);
        a_Clamp(fNewZFar, fNewZNear + 100.0f, 1000000.0f);

        //// 标记天空，之后天空的像素的是可渲染的
        if (bMaskSky && !MaskSky(pViewport))
        {
            return false;
        }

        // 改了摄象机参数，所以要改clip plane
        D3DXPLANE clipPlaneInClip;
        D3DXPLANE clipPlaneInWorld;

        m_pA3DDevice->GetClipPlane(0, (float*)&clipPlaneInClip);

        if (dwClipPlane)
        {
            A3DMATRIX4 matVP_T = pViewport->GetCamera()->GetVPTM();
            matVP_T.Transpose();
            D3DXPlaneTransform(&clipPlaneInWorld, &clipPlaneInClip,(const D3DXMATRIX*) &matVP_T);
        }

        pViewport->GetCamera()->SetZFrontAndBack(fNewZNear, fNewZFar);

        if (dwClipPlane)
        {
            m_pA3DDevice->SetClipPlaneProgrammable(0, &clipPlaneInWorld, &pViewport->GetCamera()->GetVPTM());
        }

        // 从远到近排一下序
        if (m_bSortByCamera)
            SortBoards(pViewport, aRenderBoardsFar);
        RenderList(pViewport, m_pHLSLBoard[m_nQuality][1], aRenderBoardsFar);

        if (dwClipPlane)
        {
            m_pA3DDevice->SetClipPlane(0, (float*)&clipPlaneInClip);
        }

        pViewport->GetCamera()->SetZFrontAndBack(fZNear, fZFar);
    }


    if (aRenderBoardsMiddle.GetSize() > 0)
    {
        // 从远到近排一下序
        if (m_bSortByCamera)
            SortBoards(pViewport, aRenderBoardsMiddle);
        RenderList(pViewport, m_pHLSLBoard[m_nQuality][0], aRenderBoardsMiddle);
    }

/**
    if (GetAsyncKeyState('P') < 0)
    {
        if (m_pHLSLBoard[m_nQuality][0])
            m_pHLSLBoard[m_nQuality][0]->Reload();
        if (m_pHLSLBoard[m_nQuality][1])
            m_pHLSLBoard[m_nQuality][1]->Reload();
        if (m_pPSMaskSky)
            m_pPSMaskSky->Reload();
    }
**/
    m_pA3DDevice->SetAlphaTestEnable(bAlphaTestEnable);
    m_pA3DDevice->SetAlphaRef(dwAlphaRef);
    m_pA3DDevice->SetAlphaBlendEnable(bAlphaBlendEnable);
    m_pA3DDevice->SetAlphaFunction(alphaFunc);
    m_pA3DDevice->SetZFunc(ZFunc);
    m_pA3DDevice->SetZTestEnable(bZTestEnable);
    m_pA3DDevice->SetZWriteEnable(bZWriteEnable);

	if(bMaskSky)
	{
		m_pA3DDevice->Clear(D3DCLEAR_STENCIL, 0, 0, 0);
		m_pA3DDevice->SetDeviceRenderState(D3DRS_STENCILENABLE, FALSE);
		m_pA3DDevice->SetDeviceRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	}    
    return true;
}

void A3DDistantObjectRender::RenderList(A3DViewport* pViewport, A3DHLSL* pHLSL, const AArray<A3DDistantBoard*>& aRenderBoards)
{
    if (!pHLSL)
        return;

    A3DGFXExMan* pGFXMan = AfxGetGFXExMan();
    A3DTexture* pWhiteTex = m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan()->GetWhiteTexture();
    A3DEnvironment* pEnv = m_pA3DDevice->GetA3DEngine()->GetA3DEnvironment();
    pHLSL->Appear(pEnv->GetCommonConstTable());
    pHLSL->SetConstantMatrix("g_matViewProj", pViewport->GetCamera()->GetVPTM());
    m_pVertDecl->Appear();
    for (int i = 0; i < aRenderBoards.GetSize(); i++)
    {
        A3DDistantBoard* pBoard = aRenderBoards[i];
        A3DTexture* pDiffuseTexture = pWhiteTex;
        if (pBoard->GetDiffuseTexture() && pBoard->GetDiffuseTexture()->GetD3DTexture())
        {
            pDiffuseTexture = pBoard->GetDiffuseTexture();
        }
        A3DTexture* pNormalTexture = m_pTexUnitNorm;
        if (pBoard->GetNormalTexture() && pBoard->GetNormalTexture()->GetD3DTexture())
        {
            pNormalTexture = pBoard->GetNormalTexture();
        }
        A3DTexture* pSpecularTexture = pWhiteTex;
        if (pBoard->GetSpecularTexture() && pBoard->GetSpecularTexture()->GetD3DTexture())
        {
            pSpecularTexture = pBoard->GetSpecularTexture();
        }
        A3DTexture* pShadowTexture = pWhiteTex;
        if (pBoard->GetShadowTexture() && pBoard->GetShadowTexture()->GetD3DTexture())
        {
            pShadowTexture = pBoard->GetShadowTexture();
        }
        if (pDiffuseTexture)
            pDiffuseTexture->Appear(0);
        if (pNormalTexture)
            pNormalTexture->Appear(1);
        if (pSpecularTexture)
            pSpecularTexture->Appear(2);
        if (pShadowTexture)
            pShadowTexture->Appear(3);

        A3DMATRIX4 matWorld = pBoard->GetAbsTransform();
        matWorld.Transpose();
        m_pA3DDevice->SetVertexShaderConstants(0, &matWorld, 4);
        float vDepthScale[4] = { pBoard->Size().z / pBoard->Size().x, pBoard->Size().z / pBoard->Size().y, pBoard->Size().z, 0 };
        m_pA3DDevice->SetPixelShaderConstants(0, vDepthScale, 1);

        m_pA3DDevice->SetPixelShaderConstants(1, &pBoard->GetSpecular(), 1);

        pBoard->RenderAtOnce();

        if (pDiffuseTexture)
            pDiffuseTexture->Disappear(0);
        if (pNormalTexture)
            pNormalTexture->Disappear(1);
        if (pSpecularTexture)
            pSpecularTexture->Disappear(2);
        if (pShadowTexture)
            pShadowTexture->Disappear(3);

        bool bRenderedOther = false;

        if (pBoard->GetAttachCount() > 0)
        {
            for (int iAtt = 0; iAtt < pBoard->GetAttachCount(); iAtt++)
            {
                if (pBoard->GetAttach(iAtt)->pGFX)
                {
                    GfxRenderContainer cont;
                    pGFXMan->SetCustomGfxRenderContainer(&cont);
                    pGFXMan->RegisterGfx(pBoard->GetAttach(iAtt)->pGFX);
                    pGFXMan->RenderAllGfx(pViewport);
                    pGFXMan->SetCustomGfxRenderContainer(NULL);
                    bRenderedOther = true;
                }
            }
        }

        if (bRenderedOther)
        {
            pHLSL->Appear();
            SetRenderStates();
        }
    }
    pHLSL->Disappear();

    m_pA3DDevice->SetTextureAddress(0, A3DTADDR_WRAP, A3DTADDR_WRAP);
    m_pA3DDevice->SetTextureAddress(1, A3DTADDR_WRAP, A3DTADDR_WRAP);
    m_pA3DDevice->SetTextureAddress(2, A3DTADDR_WRAP, A3DTADDR_WRAP);
    m_pA3DDevice->SetTextureAddress(3, A3DTADDR_WRAP, A3DTADDR_WRAP);
}

void A3DDistantObjectRender::SortBoards(A3DViewport* pViewport, AArray<A3DDistantBoard*>& aBoards)
{
    AArray<float> aDistance(aBoards.GetSize(), 10);
    A3DVECTOR3 vEyePos = pViewport->GetCamera()->GetPos();
    for (int i = 0; i < aBoards.GetSize(); i++)
    {
        float fDist = (aBoards[i]->GetPos() - vEyePos).SquaredMagnitude();
        aDistance.Add(fDist);
    }

    // bubble sort
    for (int i = 0; i < aBoards.GetSize() - 1; i++)
    {
        for (int j = i + 1; j < aBoards.GetSize(); j++)
        {
            if (aDistance[j] > aDistance[i])
            {
                a_Swap(aDistance[j], aDistance[i]);
                a_Swap(aBoards[j], aBoards[i]);
            }
        }
    }
}

bool A3DDistantObjectRender::MaskSky(A3DViewport* pViewport)
{
    if (!m_pPSMaskSky)
        return false;

    A3DRenderTarget* pOldDepthBuffer = m_pA3DDevice->GetIntZRenderTarget();
    if (!pOldDepthBuffer)
        return false;


    A3DVIEWPORTPARAM* pParam = pViewport->GetParam();
    float pVertices[] = 
    {
        pParam->X - 0.5f, pParam->Y - 0.5f, 0.5f, 1.0f, 0, 0,
        pParam->X + pParam->Width - 0.5f, pParam->Y - 0.5f, 0.5f, 1.0f, 1, 0,
        pParam->X + pParam->Width - 0.5f, pParam->Y + pParam->Height - 0.5f, 0.5f, 1.0f, 1, 1,
        pParam->X - 0.5f, pParam->Y + pParam->Height - 0.5f, 0.5f, 1.0f, 0, 1
    };   

    A3DDevice* pDev = m_pA3DDevice;
    pDev->SetZTestEnable(false);
    pDev->SetZWriteEnable(false);
    pDev->SetAlphaTestEnable(false);
    pDev->Clear(D3DCLEAR_STENCIL, 0, 0, 0);
    pDev->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
    pDev->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, 0);
    pDev->SetDeviceRenderState(D3DRS_STENCILENABLE, TRUE);
    pDev->SetDeviceRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
    pDev->SetDeviceRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
    pDev->SetDeviceRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
    pDev->SetDeviceRenderState(D3DRS_STENCILREF, 1);
    
    m_pVertDeclMask->Appear();
    m_pPSMaskSky->Appear();
    pOldDepthBuffer->AppearAsTexture(0, true);
    pDev->SetFaceCull(A3DCULL_NONE);
    pDev->DrawPrimitiveUP(A3DPT_TRIANGLEFAN, 4, pVertices, sizeof(float) * 6);
    pOldDepthBuffer->DisappearAsTexture(0);
    m_pPSMaskSky->Disappear();

    pDev->SetDeviceRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
    pDev->SetDeviceRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
    pDev->SetDeviceRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
    pDev->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, 
        D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
    //pDev->SetZTestEnable(true);
    pDev->SetZWriteEnable(true);
    pDev->SetAlphaTestEnable(true);
    
    return true;
}

bool A3DDistantObjectRender::MaskDepth(A3DViewport* pViewport, A3DRenderTarget* pDepthBuffer)
{
    if (!m_pPSMaskSky)
        return false;

    A3DVIEWPORTPARAM* pParam = pViewport->GetParam();
    float fZ = 0.999f;
    float pVertices[] = 
    {
        pParam->X - 0.5f, pParam->Y - 0.5f, fZ, 1.0f, 0, 0,
        pParam->X + pParam->Width - 0.5f, pParam->Y - 0.5f, fZ, 1.0f, 1, 0,
        pParam->X + pParam->Width - 0.5f, pParam->Y + pParam->Height - 0.5f, fZ, 1.0f, 1, 1,
        pParam->X - 0.5f, pParam->Y + pParam->Height - 0.5f, fZ, 1.0f, 0, 1
    };   

    A3DDevice* pDev = m_pA3DDevice;
    pDev->SetZTestEnable(false);
    pDev->SetZWriteEnable(true);
    pDev->SetAlphaTestEnable(false);
    pDev->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
    pDev->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, 0);

    m_pPSMaskSky->Appear();
    pDepthBuffer->AppearAsTexture(0, true);
    pDev->SetFaceCull(A3DCULL_NONE);
    pDev->DrawPrimitiveUP(A3DPT_TRIANGLEFAN, 4, pVertices, sizeof(float) * 6);
    pDepthBuffer->DisappearAsTexture(0);
    m_pPSMaskSky->Disappear();

    pDev->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, 
        D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
    pDev->SetZTestEnable(true);
    pDev->SetZWriteEnable(true);
    pDev->SetAlphaTestEnable(true);

    return true;
}


void A3DDistantObjectRender::ClearList()
{
    m_aBoards.RemoveAll(false);
}

void A3DDistantObjectRender::SetRenderStates()
{
    m_pA3DDevice->SetAlphaTestEnable(true);
    m_pA3DDevice->SetAlphaRef(1);
    m_pA3DDevice->SetAlphaBlendEnable(true);
    m_pA3DDevice->SetAlphaFunction(A3DCMP_GREATER);
    m_pA3DDevice->SetSourceAlpha(A3DBLEND_SRCALPHA);
    m_pA3DDevice->SetDestAlpha(A3DBLEND_INVSRCALPHA);
    m_pA3DDevice->SetZFunc(A3DCMP_LESSEQUAL);
    m_pA3DDevice->SetZTestEnable(false);
    m_pA3DDevice->SetZWriteEnable(true);

    m_pA3DDevice->SetTextureFilterType(0, A3DTEXF_LINEAR);
    m_pA3DDevice->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
    m_pA3DDevice->SetTextureFilterType(1, A3DTEXF_LINEAR);
    m_pA3DDevice->SetTextureAddress(1, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
    m_pA3DDevice->SetTextureFilterType(2, A3DTEXF_LINEAR);
    m_pA3DDevice->SetTextureAddress(2, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
    m_pA3DDevice->SetTextureFilterType(3, A3DTEXF_LINEAR);
    m_pA3DDevice->SetTextureAddress(3, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

    m_pA3DDevice->SetFaceCull(A3DCULL_NONE);
}