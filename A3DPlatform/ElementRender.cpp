#include "A3D.h"
#include "A3DCamera.h"
#include "A3DTerrain2Blk.h"
#include "A3DShadowMap.h"
#include "A3DSunMoon.h"
#include "A3DTerrainOutline2.h"
#include "A3DGrassland.h"
#include "A3DGFXExMan.h"
#include "A3DHLSL.h"
#include "A3DSkinRenderBase.h"
#include "A3DOcclusionMan.h"
#include "AGPAPerfIntegration.h"

#include "A3DPostSystem.h"
#include "A3DPostCC.h"
#include "A3DSSAO.h"
#include "ElementRender.h"
#include "ECManager.h"
#include "ECViewport.h"
#include "ECRObjBase.h"
#include "ECRShadowRender.h"
#include "A3DTerrainWater2.h"
#include "A3DTerrainWater2Render.h"
#include "A3DEnvironment.h"
#include "A3DRenderTargetMan.h"
#include "A3DGFXWarp.h"
#include "A3DPlatformMacro.h"
#include "A3DTerrainWaterFFT.h"
#include "A3DMirrorPlaneRender.h"
#include "A3DDistObjectRender.h"
#include "A3DPostSilhouette.h"
#include "A3DPostAA.h"

//////////////////////////////////////////////////////////////////////////
void WorldRenderOnSky(A3DViewport * pViewport, void * pArg);
void WorldRenderForReflected(A3DViewport * pViewport, void * pArg);
void WorldRenderForRefracted(A3DViewport * pViewport, void * pArg);
void WorldRenderForMirrorReflected(A3DViewport * pViewport, void * pArg);
//////////////////////////////////////////////////////////////////////////
extern ALog g_A3DErrLog;

class ElementRenderObject: public ElementRender
{
public:
    ElementRenderObject() : ElementRender() {};
};

ElementRenderObject g_ElementRender;

ElementRender* AfxGetElementRender()
{
    return &g_ElementRender;
}

ElementRender::ElementRender()
: m_pA3DDevice				(NULL)
, m_pPostSystem				(NULL)
, m_pTerrainRT				(NULL)
, m_pHLSLGBuffer			(NULL)
, m_pHLSLGBufferAlpha		(NULL)
, m_pHLSLGBufferSkinMesh	(NULL)
, m_pHLSLGBufferRigidMesh	(NULL)
, m_bNeedReset				(false)
, m_pFrameBuffer			(NULL)
, m_pZRt					(NULL)
, m_pUnLinearZRt			(NULL)
, m_bIsLinearZReady			(false)
, m_pLitModelRenderExtra	(NULL)
, m_pMirrorPlaneRender		(NULL)
, m_bSupportSM20			(false)
, m_pParams					(NULL)
, m_pGfxWarp				(NULL)
, m_pWaterCubeMap			(NULL)
, m_pSSAO					(NULL)
, m_bInitialized            (false)
, m_bAlphaModelWriteZ		(false)
{
	memset(&m_GameObjs, 0, sizeof(m_GameObjs));
    memset(&m_WorldObjs, 0, sizeof(m_WorldObjs));
    m_strWaterCubeMap = "";
}

ElementRender::~ElementRender()
{

}

bool ElementRender::Init(A3DDevice* pA3DDevice)
{
    if (m_bInitialized)
    {
        // init twice
        ASSERT(0);
        return false;
    }
    m_pA3DDevice = pA3DDevice;
	if(!InitFrameBuffer())
	{
		a_LogOutput(0, "ElementRender::Init, Failed to Create FrameBuffer!");
		return false;
	}
	
	m_bSupportSM20 = m_pA3DDevice->TestPixelShaderVersion(2, 0) && m_pA3DDevice->TestVertexShaderVersion(2, 0);

	A3DEngine* pA3DEngine = m_pA3DDevice->GetA3DEngine();
	//测试是否支持浮点纹理格式
	bool bSupport8bitRGBART = SUCCEEDED(pA3DEngine->GetD3D()->CheckDeviceFormat(D3DADAPTER_DEFAULT,
		(D3DDEVTYPE)m_pA3DDevice->GetDevType(),
		(D3DFORMAT)m_pA3DDevice->GetDevFormat().fmtAdapter, 
		D3DUSAGE_RENDERTARGET,
		D3DRTYPE_TEXTURE,
		D3DFMT_A8R8G8B8));

	bool bSupport16bitRGBART = SUCCEEDED(pA3DEngine->GetD3D()->CheckDeviceFormat(D3DADAPTER_DEFAULT,
		(D3DDEVTYPE)m_pA3DDevice->GetDevType(),
		(D3DFORMAT)m_pA3DDevice->GetDevFormat().fmtAdapter, 
		D3DUSAGE_RENDERTARGET,
		D3DRTYPE_TEXTURE,
		D3DFMT_A16B16G16R16F));
	bool bSupport32bitRGBART = SUCCEEDED(pA3DEngine->GetD3D()->CheckDeviceFormat(D3DADAPTER_DEFAULT,
		(D3DDEVTYPE)m_pA3DDevice->GetDevType(),
		(D3DFORMAT)m_pA3DDevice->GetDevFormat().fmtAdapter, 
		D3DUSAGE_RENDERTARGET,
		D3DRTYPE_TEXTURE,
		D3DFMT_A32B32G32R32F));
	bool bSupport16bitRRT = SUCCEEDED(pA3DEngine->GetD3D()->CheckDeviceFormat(D3DADAPTER_DEFAULT,
		(D3DDEVTYPE)m_pA3DDevice->GetDevType(),
		(D3DFORMAT)m_pA3DDevice->GetDevFormat().fmtAdapter, 
		D3DUSAGE_RENDERTARGET,
		D3DRTYPE_TEXTURE,
		D3DFMT_R16F));
	bool bSupport32bitRRT = SUCCEEDED(pA3DEngine->GetD3D()->CheckDeviceFormat(D3DADAPTER_DEFAULT,
		(D3DDEVTYPE)m_pA3DDevice->GetDevType(),
		(D3DFORMAT)m_pA3DDevice->GetDevFormat().fmtAdapter, 
		D3DUSAGE_RENDERTARGET,
		D3DRTYPE_TEXTURE,
		D3DFMT_R32F));

    ReloadWaterCubeMap("");

	bool b;
	m_pPostSystem = new A3DPostSystem();
	b = m_pPostSystem->Init(m_pA3DDevice, this);

	//初始化GBufferRT, GBuffer HLSL
	
	A3DRenderTarget::RTFMT fmt;
	fmt.iWidth     = 0;		//大小设置为与BackBuffer一样. 不需要再考虑设备丢失问题.
	fmt.iHeight    = 0;
	fmt.fmtTarget  = A3DFMT_UNKNOWN;
	fmt.fmtDepth   = A3DFMT_UNKNOWN;

	if(bSupport16bitRRT)
		fmt.fmtTarget  = (A3DFORMAT)D3DFMT_R16F;
	else if(bSupport32bitRRT)
		fmt.fmtTarget = (A3DFORMAT)D3DFMT_R32F;

	//测试用
	//fmt.fmtTarget = (A3DFORMAT)D3DFMT_A32B32G32R32F;

	if(fmt.fmtTarget != A3DFMT_UNKNOWN)
	{
		m_pZRt = new A3DRenderTarget();
		if(!m_pZRt->Init(m_pA3DDevice, fmt, true, false))
		{
			g_A3DErrLog.Log("A3DPostSystem::CreateResource create m_pZRt failed!");
		}

		m_pUnLinearZRt = new A3DRenderTarget();
		if(!m_pUnLinearZRt->Init(m_pA3DDevice, fmt, true, false))
		{
			g_A3DErrLog.Log("A3DPostSystem::CreateResource create m_pUnLinearZRt failed!");
		}
	}

	m_pHLSLGBuffer = m_pA3DDevice->GetA3DEngine()->GetA3DHLSLMan()->LoadShader(
		"shaders\\2.2\\HLSL\\LitModel\\GBufferVS.hlsl", 
        "shaders\\2.2\\HLSL\\LitModel\\GBufferPS.hlsl", NULL);

	m_pHLSLGBufferAlpha = m_pA3DDevice->GetA3DEngine()->GetA3DHLSLMan()->LoadShader(
		"shaders\\2.2\\HLSL\\LitModel\\GBufferVS.hlsl", 
		"shaders\\2.2\\HLSL\\LitModel\\GBufferPS.hlsl", "_ALPHA_TEST_");

    if (!m_pHLSLGBuffer || !m_pHLSLGBufferAlpha)
    {
        g_A3DErrLog.Log("A3DPostSystem::CreateResource create GBuffer shader failed!");
        return false;
    }

    m_pHLSLGBufferSkinMesh = m_pA3DDevice->GetA3DEngine()->GetA3DHLSLMan()->LoadShader(
		"shaders\\2.2\\HLSL\\SkinModel\\GBufferSkinVS.hlsl", 
		"shaders\\2.2\\HLSL\\SkinModel\\GBufferSkinPS.hlsl", "_ALPHA_TEST_");

	m_pHLSLGBufferRigidMesh = m_pA3DDevice->GetA3DEngine()->GetA3DHLSLMan()->LoadShader(
		"shaders\\2.2\\HLSL\\SkinModel\\GBufferSkinVS.hlsl", 
		"shaders\\2.2\\HLSL\\SkinModel\\GBufferSkinPS.hlsl", "_ALPHA_TEST_;_RIGID_MESH_");
	
    if (!m_pHLSLGBufferSkinMesh || !m_pHLSLGBufferRigidMesh)
    {
        g_A3DErrLog.Log("A3DPostSystem::CreateResource create GBufferSkin shader failed!");
        return false;
    }

	m_pHLSLGBufferSkinMesh_LinearDepth = m_pA3DDevice->GetA3DEngine()->GetA3DHLSLMan()->LoadShader(
		"shaders\\2.2\\HLSL\\SkinModel\\GBufferSkinVS.hlsl", 
		"shaders\\2.2\\HLSL\\SkinModel\\GBufferSkinPS.hlsl", "_ALPHA_TEST_;_SILHOUETTE_DEPTH_");

	m_pHLSLGBufferRigidMesh_LinearDepth = m_pA3DDevice->GetA3DEngine()->GetA3DHLSLMan()->LoadShader(
		"shaders\\2.2\\HLSL\\SkinModel\\GBufferSkinVS.hlsl", 
		"shaders\\2.2\\HLSL\\SkinModel\\GBufferSkinPS.hlsl", "_ALPHA_TEST_;_RIGID_MESH_;_SILHOUETTE_DEPTH_");

	if (!m_pHLSLGBufferSkinMesh_LinearDepth || !m_pHLSLGBufferRigidMesh_LinearDepth)
	{
		g_A3DErrLog.Log("A3DPostSystem::CreateResource create GBufferSkin shader failed!");
		return false;
	}


    const D3DPRESENT_PARAMETERS& d3dpp = m_pA3DDevice->GetD3DPP();

	m_pLitModelRenderExtra = new A3DLitModelRender;
	if (!m_pLitModelRenderExtra->Init(m_pA3DDevice->GetA3DEngine(), 60000, 60000 * 2) )
	{
		a_LogOutput(1, "ElementRender::Init, lit model render init fail!");
		return false;
	}

	m_pMirrorPlaneRender = new A3DMirrorPlaneRender;
	if( !m_pMirrorPlaneRender->Init( m_pA3DDevice->GetA3DEngine() ) )
	{
		g_A3DErrLog.Log( __FUNCTION__ " Failed to initialize mirror plane renderer." );
		return false;
	}

	fmt.iWidth = 0;
	fmt.iHeight = 0;
	fmt.fmtTarget = (A3DFORMAT)d3dpp.BackBufferFormat;
	m_pTerrainRT = new A3DRenderTarget;
	b = m_pTerrainRT->Init(m_pA3DDevice, fmt, true, false);
	if(b == false)
	{
		a_LogOutput(1, "ElementRender::Init, can not create terrain RT!");
		return false;
	}

	RECT rc;
	rc.left = 0;
	rc.right = 1024;
	rc.top = 0;
	rc.bottom = 1024;

	m_pA3DDevice->AddUnmanagedDevObject(this);
    
    m_pGfxWarp = new A3DGFXWarp;
    if (!m_pGfxWarp->Init(m_pA3DDevice))
    {
        A3DRELEASE(m_pGfxWarp);
    }

    AfxGetA3DDistantObjectRender()->Init(pA3DDevice);

#if _ENABLE_SSAO_
 	m_pSSAO = new A3DSSAO;
 	if(!m_pSSAO->Init(m_pA3DDevice))
 	{
 		a_LogOutput(1, "ElementRender::Init, can not init SSAO!");
 		delete m_pSSAO;
 		m_pSSAO = NULL;
 	}
#endif
    m_bInitialized = true;

	return true;
}

bool ElementRender::InitFrameBuffer()
{
	const D3DPRESENT_PARAMETERS& d3dpp = m_pA3DDevice->GetD3DPP();

	A3DRenderTarget::RTFMT fmt;
	fmt.iWidth = 0;
	fmt.iHeight = 0;
	fmt.fmtTarget = m_pA3DDevice->GetDevFormat().fmtAdapter;
	fmt.fmtDepth = A3DFMT_UNKNOWN;

	A3DRELEASE(m_pFrameBuffer);
	m_pFrameBuffer = new A3DRenderTarget;
	if(!m_pFrameBuffer->Init(m_pA3DDevice, fmt, true, false))
	{
		a_LogOutput(0, "ElementRender::InitFrameBuffer create m_pFrameBuffer failed!");
		ASSERT(0);
		return false;
	}

	return true;

}

//	Before device reset
bool ElementRender::BeforeDeviceReset()
{
	return true;
}

//	After device reset
bool ElementRender::AfterDeviceReset()
{
	m_bNeedReset = true;
	return true;
}

void ElementRender::Release()
{
	A3DRELEASE(m_pTerrainRT);
	A3DRELEASE(m_pMirrorPlaneRender);
	A3DRELEASE(m_pLitModelRenderExtra);	
	A3DRELEASE(m_pPostSystem);
	A3DRELEASE(m_pZRt);
	A3DRELEASE(m_pUnLinearZRt);

	m_pA3DDevice->GetA3DEngine()->GetA3DHLSLMan()->ReleaseShader(m_pHLSLGBuffer);
	m_pA3DDevice->GetA3DEngine()->GetA3DHLSLMan()->ReleaseShader(m_pHLSLGBufferAlpha);
	m_pA3DDevice->GetA3DEngine()->GetA3DHLSLMan()->ReleaseShader(m_pHLSLGBufferSkinMesh);
	m_pA3DDevice->GetA3DEngine()->GetA3DHLSLMan()->ReleaseShader(m_pHLSLGBufferRigidMesh);
	m_pA3DDevice->GetA3DEngine()->GetA3DHLSLMan()->ReleaseShader(m_pHLSLGBufferSkinMesh_LinearDepth);
	m_pA3DDevice->GetA3DEngine()->GetA3DHLSLMan()->ReleaseShader(m_pHLSLGBufferRigidMesh_LinearDepth);

	A3DRELEASE(m_pFrameBuffer);
	A3DRELEASE(m_pGfxWarp);
    A3DRELEASE(m_pSSAO);

    m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan()->ReleaseTexture(m_pWaterCubeMap);
    AfxGetA3DDistantObjectRender()->Release();
    m_bInitialized = false;
}

void ElementRender::RenderManagers(ECViewport* pViewport, bool bPost)
{
	A3D_ITTX_SCOPED_TASK_NAMED(AGPAPerfIntegration::GetInstance().GetA3DDomain(), TEXT(__FUNCTION__));
	for (int i = 0; i < m_aManagers.GetSize(); i++)
	{
		if (m_aManagers[i])
        {
            if (bPost)
                m_aManagers[i]->RenderPost(pViewport);
            else   
                m_aManagers[i]->Render(pViewport);
        }
	}
}

bool ElementRender::RenderUI(const ELEMENT_RENDER_PARAM* pParam)
{
	A3D_ITTX_SCOPED_TASK_NAMED(AGPAPerfIntegration::GetInstance().GetA3DDomain(), TEXT(__FUNCTION__));
	if (!pParam->bRenderUI)
		return true;

	if (!m_GameObjs.pUIManager)
		return true;

	m_pA3DDevice->SetZBias(0);

	ECRENDERPARAM ECRParam(*pParam, m_pViewport);
	if (m_GameObjs.pImgRes)
		m_GameObjs.pImgRes->Render(&ECRParam);

	A3DDEVFMT devFmt = m_pA3DDevice->GetDevFormat();
	A3DVIEWPORTPARAM viewParam;
	viewParam.X = 0;
	viewParam.Y = 0;
	viewParam.Width  = devFmt.nWidth;
	viewParam.Height = devFmt.nHeight;
	viewParam.MinZ = 0.0f;
	viewParam.MaxZ = 1.0f;
	m_pA3DDevice->SetViewport(&viewParam);

	bool bGlobalFog = m_pA3DDevice->GetA3DEngine()->GetA3DEnvironment()->GetGlobalFogEnable();
	m_pA3DDevice->GetA3DEngine()->GetA3DEnvironment()->SetGlobalFogEnable(false);
	m_GameObjs.pUIManager->Render(&ECRParam);
	m_pA3DDevice->GetA3DEngine()->GetA3DEnvironment()->SetGlobalFogEnable(bGlobalFog);

	return true;
}

bool ElementRender::Render(ECViewport* pViewport, const ELEMENT_RENDER_PARAM* pParam, A3DRenderTarget* pFrameBuffer)
{
    A3D_ITTX_SCOPED_TASK_NAMED(AGPAPerfIntegration::GetInstance().GetA3DDomain(), TEXT(__FUNCTION__));
    A3DEngine* pA3DEngine = m_pA3DDevice->GetA3DEngine();
    const D3DPRESENT_PARAMETERS& d3dpp = m_pA3DDevice->GetD3DPP();
    A3DViewport* pA3DViewport = pViewport->GetA3DViewport();
    bool bIsScreen = pFrameBuffer == NULL && (pA3DViewport->GetParam()->Width == d3dpp.BackBufferWidth)
        && (pA3DViewport->GetParam()->Height == d3dpp.BackBufferHeight);

    m_pViewport = pViewport;
    m_pParams = pParam;

    if (bIsScreen)
    {
        if (m_bNeedReset)
        {
            InitFrameBuffer();
            m_bNeedReset = false;
        }
        pFrameBuffer = m_pFrameBuffer;
        //	遮挡剪裁
        A3DOcclusionMan* pOccMan = m_pA3DDevice->GetA3DEngine()->GetA3DOcclusionMan();
        if (pOccMan && g_pA3DConfig->RT_GetUseOcclusionCullingFlag())
        {
            A3DRenderTarget* pINTZ = m_pA3DDevice->GetIntZRenderTarget();
            if (pINTZ && pINTZ->GetDepthTexture())
            {
                pOccMan->TestAll(0);
            }
            else
            {
                g_pA3DConfig->RT_SetUseOcclusionCullingFlag(false);
            }
        }

    }

	m_pViewport = pViewport;
	ECRENDERPARAM ECRParam(*pParam, m_pViewport);

	//渲染场景到FrameBuffer
	if (bIsScreen)
		m_pFrameBuffer->ApplyToDevice();

	RenderToRenderTarget(pViewport, pParam, pFrameBuffer, bIsScreen);
	if (pParam->bRenderSkinModel && (pParam->paramPostEffect.dwPostEffectMask & A3DPOST_EFFECT_BloomLocal))
		RenderBloomSkinModel(pViewport);


	if (bIsScreen)
	{
		if (pParam->bRenderPostProcess)
			m_pPostSystem->ApplyParam(&pParam->paramPostEffect);

		if(true && pParam->bRenderSkinModel
			&& m_GameObjs.pSkinRenderSilhouette 
			&& m_pPostSystem->GetPlayerSilhousetteRT()) //如果开启了勾边
		{
			
			RenderPlayerDepthForSilhouette(m_GameObjs.pSkinRenderSilhouette, pViewport, m_pPostSystem->GetPlayerSilhousetteRT());
		}

		//进行后处理
		m_pPostSystem->Render(m_pFrameBuffer, pViewport->GetA3DViewport(), pParam->bRenderZToRenderTarget == 0, pParam->bRenderPostProcess);
	
		// 后处理完成后再进行GfxWarp，让勾边能随着Gfx一起扭动。by panyupin 2013.3.13
		// 这块暂时屏蔽, 部分玩家带着特效, 调用这块之后会出现不显示界面/姓名板的问题
		// pParam->bRenderUI为false时会调用, 仅以后调试用
		if (m_pGfxWarp && pParam->bRenderGFX && !pParam->bRenderUI && m_GameObjs.pA3DGFXExMan)
		{
			m_pGfxWarp->Render(pA3DViewport, m_pFrameBuffer, m_GameObjs.pA3DGFXExMan);
		}
	}

	RenderManagers(pViewport, true);

	//	Render wire and Flat
	//RenderWireFlat();

	if (bIsScreen)
	{
		//	Render UI
		RenderUI(pParam);
	}

	if (bIsScreen)
		m_pFrameBuffer->WithdrawFromDevice();


	if (bIsScreen)
	{
		m_pPostSystem->RenderAfterUI(m_pFrameBuffer, pViewport->GetA3DViewport(), pParam->bRenderZToRenderTarget == 0, pParam->bRenderPostProcess);
	}

    return true;
}

bool ElementRender::RenderOnSky(const ELEMENT_RENDER_PARAM* pParam)
{

	if( m_WorldObjs.pSunMoon )
		m_WorldObjs.pSunMoon->Render(m_pViewport->GetA3DViewport());

	if (m_WorldObjs.pTerrainOutline)
	{
		m_WorldObjs.pTerrainOutline->SetDNFactor(pParam->fDNFactor);
		m_WorldObjs.pTerrainOutline->Render2(m_pViewport->GetA3DViewport(), pParam->bRenderShadow && m_pA3DDevice->GetA3DEngine()->GetA3DShadowMap());
	}

	return true;
}

//	Render grasses
bool ElementRender::RenderGrasses(const ELEMENT_RENDER_PARAM* pParam, bool bRenderAlpha)
{
	if( !pParam->bRenderGrass )
		return true;

	if (m_WorldObjs.pGrassLand)
	{
		m_WorldObjs.pGrassLand->SetDNFactor(pParam->fDNFactor);
		m_WorldObjs.pGrassLand->Render(m_pViewport->GetA3DViewport(), bRenderAlpha);
	}

	return true;
}

bool ElementRender::RenderNatureObjects(const ELEMENT_RENDER_PARAM* pParam)
{
	ECViewport* pViewport = m_pViewport;
	A3DVECTOR3 vecCamPos = pViewport->GetA3DViewport()->GetCamera()->GetPos();
	bool bCamBelowWater = false;
	if (m_WorldObjs.pA3DTerrainWater2)
		bCamBelowWater = m_WorldObjs.pA3DTerrainWater2->IsUnderWater(vecCamPos);
	// see if camera is under water, if so, just return without rendering these particles.
	if( bCamBelowWater )
		return true;

	if( m_WorldObjs.pRain )
		m_WorldObjs.pRain->RenderParticles(pViewport->GetA3DViewport());

	if( m_WorldObjs.pSnow )
		m_WorldObjs.pSnow->RenderParticles(pViewport->GetA3DViewport());

	return true;
}


bool ElementRender::RenderForReflected(const ELEMENT_RENDER_PARAM* pParam, A3DViewport* pReflectedViewport)
{
	ECViewport ecViewport;
	ecViewport.InitFromA3D(pReflectedViewport); 
	A3DEngine* pA3DEngine = m_pA3DDevice->GetA3DEngine();

    A3DSkinRenderBase* pSkinRenderReflect = m_GameObjs.pSkinRender2;
	A3DSkinRenderBase* pSkinRenderOld = pA3DEngine->GetA3DSkinMan()->GetCurSkinRender();
	pA3DEngine->GetA3DSkinMan()->SetCurSkinRender(pSkinRenderReflect);

	A3DLitModelRender* pLitModelRenderOld = pA3DEngine->GetA3DLitModelRender();
    A3DLitModelRender* pLitModelRenderReflect = m_pLitModelRenderExtra;
	pA3DEngine->SetA3DLitModelRender(pLitModelRenderReflect);
    pLitModelRenderReflect->SetClipParam(10.0f, 0.1f);
	GfxRenderContainer rc;
	m_GameObjs.pA3DGFXExMan->SetCustomGfxRenderContainer(&rc);

	bool bOldAlphaBlendEnable = m_pA3DDevice->GetAlphaBlendEnable();
	bool bOldAlphaTestEnable = m_pA3DDevice->GetAlphaTestEnable();
	A3DCMPFUNC oldAlphaFunc = m_pA3DDevice->GetAlphaFunction();
	DWORD dwAlphaRef = m_pA3DDevice->GetAlphaRef();

	m_pA3DDevice->SetAlphaBlendEnable(false);
	m_pA3DDevice->SetAlphaTestEnable(true);
	m_pA3DDevice->SetAlphaFunction(A3DCMP_GREATER);
	m_pA3DDevice->SetAlphaRef(84);

	for (int i = 0; i < m_aManagers.GetSize(); i++)
	{
		if (m_aManagers[i])
		{
    		m_aManagers[i]->RenderForReflect(&ecViewport);
		}
	}

	pA3DEngine->GetA3DLitModelRender()->Render(pReflectedViewport, false, pParam->bModelLightMap, false);
	pA3DEngine->GetA3DLitModelRender()->ResetRenderInfo(false);

	pSkinRenderReflect->Render(pReflectedViewport, false);
	pSkinRenderReflect->ResetRenderInfo(false);

	if (pParam->nWaterEffect == 3)
		RenderGrasses(pParam, false);

	m_pA3DDevice->SetAlphaTestEnable(false);
	m_pA3DDevice->SetAlphaBlendEnable(true);


	pA3DEngine->GetA3DLitModelRender()->RenderAlpha(pReflectedViewport);
	pA3DEngine->GetA3DLitModelRender()->ResetRenderInfo(true);

	pSkinRenderReflect->RenderAlpha(pReflectedViewport);
	pSkinRenderReflect->ResetRenderInfo(true);

	// last flush gfx
	m_GameObjs.pA3DGFXExMan->RenderAllSkinModels(pReflectedViewport);
	//	Render skin models from gfx
	pSkinRenderReflect->Render(pReflectedViewport, false);
	pSkinRenderReflect->ResetRenderInfo(false);
	pSkinRenderReflect->RenderAlpha(pReflectedViewport);
	pSkinRenderReflect->ResetRenderInfo(true);
	m_GameObjs.pA3DGFXExMan->RenderAllGfx(pReflectedViewport);

	//	Restore skin render
	pA3DEngine->GetA3DSkinMan()->SetCurSkinRender(pSkinRenderOld);
	pA3DEngine->SetA3DLitModelRender(pLitModelRenderOld);
	m_GameObjs.pA3DGFXExMan->SetCustomGfxRenderContainer(NULL);

	//还原设备状态
	m_pA3DDevice->SetAlphaBlendEnable(bOldAlphaBlendEnable);
	m_pA3DDevice->SetAlphaTestEnable(bOldAlphaTestEnable);
	m_pA3DDevice->SetAlphaFunction(oldAlphaFunc);
	m_pA3DDevice->SetAlphaRef(dwAlphaRef);

	return true;
}

bool ElementRender::RenderForRefracted(const ELEMENT_RENDER_PARAM* pParam, A3DViewport* pRefractedViewport)
{
	//从水下往水上看的时候会调用到
	ECViewport ecViewport;
	ecViewport.InitFromA3D(pRefractedViewport);
	ECRENDERPARAM ECRParam(*pParam, m_pViewport);

	A3DEngine* pA3DEngine = m_pA3DDevice->GetA3DEngine();

	//	Use a temporary skin render so that we don't break world's main rendering
	A3DSkinRenderBase* psr = m_GameObjs.pSkinRender2;//g_pGame->GetSkinRender2();
	A3DSkinRenderBase* poldsr = pA3DEngine->GetA3DSkinMan()->GetCurSkinRender();
	pA3DEngine->GetA3DSkinMan()->SetCurSkinRender(psr);

	A3DLitModelRender* poldlmr = pA3DEngine->GetA3DLitModelRender();
	A3DLitModelRender* plmr = m_pLitModelRenderExtra;//g_pGame->GetLitModelRender1();
	pA3DEngine->SetA3DLitModelRender(plmr);

    // 用一个临时的container，避免修改原始GFX列表
	GfxRenderContainer rc;
	m_GameObjs.pA3DGFXExMan->SetCustomGfxRenderContainer(&rc);

	A3DVECTOR3 vecCamPos = pRefractedViewport->GetCamera()->GetPos();

	bool bCamBelowWater = m_WorldObjs.pA3DTerrainWater2->IsUnderWater(vecCamPos);

	bool bOldAlphaBlendEnable = m_pA3DDevice->GetAlphaBlendEnable();
	bool bOldAlphaTestEnable = m_pA3DDevice->GetAlphaTestEnable();
	A3DCMPFUNC oldAlphaFunc = m_pA3DDevice->GetAlphaFunction();
	DWORD dwAlphaRef = m_pA3DDevice->GetAlphaRef();

	m_pA3DDevice->SetAlphaBlendEnable(false);
	m_pA3DDevice->SetAlphaTestEnable(true);
	m_pA3DDevice->SetAlphaFunction(A3DCMP_GREATER);
	m_pA3DDevice->SetAlphaRef(84);

	for (int i=0; i < m_aManagers.GetSize(); i++)
	{
		if (m_aManagers[i])
			m_aManagers[i]->RenderForRefract(&ecViewport);
	}

	// render critters.
    if (m_WorldObjs.pScene)
	    m_WorldObjs.pScene->Render(&ECRParam);

	pA3DEngine->GetA3DLitModelRender()->Render(pRefractedViewport, false, pParam->bModelLightMap, false);
	pA3DEngine->GetA3DLitModelRender()->ResetRenderInfo(false);

	psr->Render(pRefractedViewport, false);
    psr->ResetRenderInfo(false);

	if (pParam->nWaterEffect == 3)
	{
		RenderGrasses(pParam, false);
	}

	m_pA3DDevice->SetAlphaBlendEnable(true);
	m_pA3DDevice->SetAlphaTestEnable(false);

	pA3DEngine->GetA3DLitModelRender()->RenderAlpha(pRefractedViewport);
	pA3DEngine->GetA3DLitModelRender()->ResetRenderInfo(true);

	psr->RenderAlpha(pRefractedViewport);
   	psr->ResetRenderInfo(true);

	if (pParam->nWaterEffect == 3)
		RenderGrasses(pParam, true);

	// last flush gfx
	m_GameObjs.pA3DGFXExMan->RenderAllSkinModels(pRefractedViewport);
	//	Render skin models from gfx
	psr->Render(pRefractedViewport, false);
   	psr->ResetRenderInfo(false);
	psr->RenderAlpha(pRefractedViewport);
	
    if (psr != poldsr)
        psr->ResetRenderInfo(true);

    m_GameObjs.pA3DGFXExMan->SetCustomGfxRenderContainer(NULL);
    m_GameObjs.pA3DGFXExMan->RenderAllGfxWater(pRefractedViewport, 
        m_WorldObjs.pA3DTerrainWater2->GetWaterHeight(pRefractedViewport->GetCamera()->GetPos()), false);

	//	Restore skin render
	pA3DEngine->GetA3DSkinMan()->SetCurSkinRender(poldsr);
	pA3DEngine->SetA3DLitModelRender(poldlmr);


	m_pA3DDevice->SetAlphaBlendEnable(bOldAlphaBlendEnable);
	m_pA3DDevice->SetAlphaTestEnable(bOldAlphaTestEnable);
	m_pA3DDevice->SetAlphaFunction(oldAlphaFunc);
	m_pA3DDevice->SetAlphaRef(dwAlphaRef);

	return true;
}

bool ElementRender::RenderForMirrorReflected(const MirrorReflectedRenderParam* pMirrorReflectedParam, A3DViewport* pReflectedViewport)
{
	// 天空渲染
	if( pMirrorReflectedParam->pElementRenderParam->bRenderSky )
	{
		if (m_WorldObjs.pA3DSky)
		{
			A3DCamera* pCamera = pMirrorReflectedParam->pECRenderParam->pViewport->GetA3DCamera();
			m_WorldObjs.pA3DSky->SetCamera( pCamera );
			m_WorldObjs.pA3DSky->Render();
		}
	}

	// 地形渲染
	if( pMirrorReflectedParam->pElementRenderParam->bRenderTerrain )
	{
		if (m_WorldObjs.pA3DTerrain)
		{
			m_WorldObjs.pA3DTerrain->SetDNFactor( pMirrorReflectedParam->pElementRenderParam->fDNFactor );
			m_WorldObjs.pA3DTerrain->Render(pReflectedViewport, false, m_pTerrainRT, 0.0f, 0.0f, false, false, 
				pMirrorReflectedParam->pElementRenderParam->bRenderShadow && m_pA3DDevice->GetA3DEngine()->GetA3DShadowMap() );
		}

		if (m_WorldObjs.pMaskModifier)
		{
			m_WorldObjs.pMaskModifier->Render( pMirrorReflectedParam->pECRenderParam );
		}

		if (m_WorldObjs.pTerrainOutline)
		{
			m_WorldObjs.pTerrainOutline->SetDNFactor( pMirrorReflectedParam->pElementRenderParam->fDNFactor );
			m_WorldObjs.pTerrainOutline->Render2( m_pViewport->GetA3DViewport(), 
				pMirrorReflectedParam->pElementRenderParam->bRenderShadow && m_pA3DDevice->GetA3DEngine()->GetA3DShadowMap());
		}
	}

	RenderForReflected( pMirrorReflectedParam->pElementRenderParam, pReflectedViewport );

	return true;
}

bool ElementRender::ZPrePass(const ZPREPASS_PARAM* pParam)
{
	if( !pParam->bRenderScene)
		return false;

	A3DEngine* pA3DEngine = m_pA3DDevice->GetA3DEngine();	
	
	bool b = false;
	A3DCameraBase* pCamera = pParam->pViewport->GetA3DCameraBase();

	if(pCamera == NULL)
		return false;

	//先把当前状态保存下来.
	DWORD dwState;
	bool bOldAlphaBlendEnable;
	DWORD dwOldColorWriteEnable;
	bool bOldAlphaTestEnable;
	bool bOldZTestEnable;
	bool bOldZWriteEnable;
	A3DCULLTYPE oldCullType;
	A3DCMPFUNC oldCmpFunc;
	DWORD dwOldAlphaRef;

	dwState = m_pA3DDevice->GetDeviceRenderState(D3DRS_ALPHABLENDENABLE);
	bOldAlphaBlendEnable = (dwState != 0 ? true : false);
	dwOldColorWriteEnable = m_pA3DDevice->GetDeviceRenderState(D3DRS_COLORWRITEENABLE);
	bOldAlphaTestEnable = m_pA3DDevice->GetAlphaTestEnable();
	dwState = m_pA3DDevice->GetDeviceRenderState(D3DRS_ZENABLE);
	bOldZTestEnable = (dwState != 0 ? true : false);
	dwState = m_pA3DDevice->GetDeviceRenderState(D3DRS_ZWRITEENABLE);
	bOldZWriteEnable = (dwState != 0 ? true : false);
	oldCullType = m_pA3DDevice->GetFaceCull();
	oldCmpFunc = m_pA3DDevice->GetAlphaFunction();	
	dwOldAlphaRef = m_pA3DDevice->GetAlphaRef();


	//设置必要的Rendertarget
	m_pA3DDevice->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, 
		D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);

	m_pA3DDevice->SetAlphaBlendEnable(false);
	m_pA3DDevice->SetAlphaTestEnable(false);
	m_pA3DDevice->SetZTestEnable(true);
	m_pA3DDevice->SetZWriteEnable(true);
	m_pA3DDevice->SetFaceCull(A3DCULL_CCW);

	//渲染地形
	if(pParam->bRenderTerrain)
	{
		b = m_pHLSLGBuffer->SetConstantMatrix("matWorldView", pCamera->GetViewTM());
		b = m_pHLSLGBuffer->SetConstantMatrix("matWorldViewProj", pCamera->GetVPTM());

		int iFVF;
		if (m_WorldObjs.pA3DTerrain->UseLightmapTech())
			iFVF = A3DTRN2VERT1_FVF;
		else
			iFVF = A3DTRN2VERT2_FVF;

		m_pA3DDevice->SetFVF(iFVF);


		m_pHLSLGBuffer->Appear();
		m_WorldObjs.pA3DTerrain->RenderRawMesh(pParam->pViewport->GetA3DViewport(), true, false);
	}
	
	m_pA3DDevice->SetFaceCull(A3DCULL_CCW);

	//渲染LitModel
	if(pParam->bRenderLitmodel)
	{
		b = m_pHLSLGBufferAlpha->SetConstantMatrix("matWorldView", pCamera->GetViewTM());
		b = m_pHLSLGBufferAlpha->SetConstantMatrix("matWorldViewProj", pCamera->GetVPTM());

		m_pA3DDevice->SetAlphaBlendEnable(false);
		m_pA3DDevice->SetAlphaTestEnable(true);
		m_pA3DDevice->SetAlphaFunction(A3DCMP_GREATER);
		m_pA3DDevice->SetAlphaRef(90); // 84 这里稍微增大一点, 不然由于Shader用的版本不同, 会出现斑点

		A3DLitModelRender* pLitModelRender = pA3DEngine->GetA3DLitModelRender();
		m_pHLSLGBufferAlpha->Appear();

		pLitModelRender->RenderRaw(pParam->pViewport->GetA3DViewport());
	}
	m_pA3DDevice->SetFaceCull(A3DCULL_CCW);

	//渲染SkinModel
	if(pParam->pSkinRender && pParam->bRenderSkin)
	{
 		m_pA3DDevice->SetAlphaBlendEnable(false);
 		m_pA3DDevice->SetAlphaTestEnable(true);
 		m_pA3DDevice->SetAlphaFunction(A3DCMP_GREATER);
 		m_pA3DDevice->SetAlphaRef(84);

 		pParam->pSkinRender->RenderRaw(pParam->pViewport->GetA3DViewport(), m_pHLSLGBufferSkinMesh, m_pHLSLGBufferRigidMesh);
	}
	
	//还原RenderState
	m_pA3DDevice->SetFaceCull(A3DCULL_CCW);

	m_pA3DDevice->ClearVertexShader();
	m_pA3DDevice->ClearPixelShader();

	m_pA3DDevice->SetFVF(A3DFVF_A3DVERTEX);

	m_pA3DDevice->SetAlphaBlendEnable(bOldAlphaBlendEnable);
	m_pA3DDevice->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, dwOldColorWriteEnable);
	m_pA3DDevice->SetAlphaTestEnable(bOldAlphaTestEnable);
	m_pA3DDevice->SetZTestEnable(bOldZTestEnable);
	m_pA3DDevice->SetZWriteEnable(bOldZWriteEnable);
	m_pA3DDevice->SetFaceCull(oldCullType);
	m_pA3DDevice->SetAlphaFunction(oldCmpFunc);
	m_pA3DDevice->SetAlphaRef(dwOldAlphaRef);
	
	return true;
}

bool ElementRender::ShadowPass( SHADOWPASS_PARAM* pParam )
{
	A3DEngine* pA3DEngine = m_pA3DDevice->GetA3DEngine();	

	bool b = false;
	A3DCameraBase* pCamera = pParam->pViewport->GetA3DCameraBase();


	if(pCamera == NULL || pParam->pSkinRender == NULL)
		return false;

	//先把当前状态保存下来.
	DWORD dwState;
	bool bOldAlphaBlendEnable;
	DWORD dwOldColorWriteEnable;
	bool bOldAlphaTestEnable;
	bool bOldZTestEnable;
	bool bOldZWriteEnable;
	A3DCULLTYPE oldCullType;
	A3DCMPFUNC oldCmpFunc;
	DWORD dwOldAlphaRef;

	dwState = m_pA3DDevice->GetDeviceRenderState(D3DRS_ALPHABLENDENABLE);
	bOldAlphaBlendEnable = (dwState != 0 ? true : false);
	dwOldColorWriteEnable = m_pA3DDevice->GetDeviceRenderState(D3DRS_COLORWRITEENABLE);
	bOldAlphaTestEnable = m_pA3DDevice->GetAlphaTestEnable();
	dwState = m_pA3DDevice->GetDeviceRenderState(D3DRS_ZENABLE);
	bOldZTestEnable = (dwState != 0 ? true : false);
	dwState = m_pA3DDevice->GetDeviceRenderState(D3DRS_ZWRITEENABLE);
	bOldZWriteEnable = (dwState != 0 ? true : false);
	oldCullType = m_pA3DDevice->GetFaceCull();
	oldCmpFunc = m_pA3DDevice->GetAlphaFunction();	
	dwOldAlphaRef = m_pA3DDevice->GetAlphaRef();


	A3DSkinRenderBase* pSkinRenderOld = pA3DEngine->GetA3DSkinMan()->GetCurSkinRender();
	pA3DEngine->GetA3DSkinMan()->SetCurSkinRender(pParam->pSkinRender);
	GfxRenderContainer rc;
	m_GameObjs.pA3DGFXExMan->SetCustomGfxRenderContainer(&rc);

	//注册
	pParam->pSkinRender->ResetRenderInfo(false);
	pParam->pSkinRender->ResetRenderInfo(true);

	for (int i = 0; i < m_aManagers.GetSize(); i++)
	{
		if (m_aManagers[i])
		{
			m_aManagers[i]->RenderForShadow(pParam->pViewport);
		}
	}



	//设置必要的Rendertarget
	m_pA3DDevice->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, 
		D3DCOLORWRITEENABLE_RED);

	m_pA3DDevice->SetAlphaBlendEnable(false);
	m_pA3DDevice->SetAlphaTestEnable(false);
	m_pA3DDevice->SetZTestEnable(true);
	m_pA3DDevice->SetZWriteEnable(true);
	m_pA3DDevice->SetFaceCull(A3DCULL_CCW);
	m_pA3DDevice->Clear(D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
	
	//渲染地形
	/*
	m_pA3DDevice->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, 
		D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
	if(pParam->bRenderTerrain)
	{
		b = m_pHLSLGBuffer->SetConstantMatrix("matWorldView", pCamera->GetViewTM());
		b = m_pHLSLGBuffer->SetConstantMatrix("matWorldViewProj", pCamera->GetVPTM());
		//b = m_pHLSLGBuffer->SetValue1f("g_zFar", pCamera->GetZBack());

		int iFVF;
		if (m_pA3DTerrain->UseLightmapTech())
			iFVF = A3DTRN2VERT1_FVF;
		else
			iFVF = A3DTRN2VERT2_FVF;

		m_pA3DDevice->SetDeviceVertexShader(iFVF);

		m_pHLSLGBuffer->Appear();
		m_pA3DTerrain->RenderRawMesh(pParam->pViewport->GetA3DViewport(), true, false);
	}
	
	

	//渲染LitModel
	m_pA3DDevice->SetFaceCull(A3DCULL_CCW);
	m_pA3DDevice->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, 
		D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
	if(pParam->bRenderLitmodel)
	{
		b = m_pHLSLGBufferAlpha->SetConstantMatrix("matWorldView", pCamera->GetViewTM());
		b = m_pHLSLGBufferAlpha->SetConstantMatrix("matWorldViewProj", pCamera->GetVPTM());

		m_pA3DDevice->SetAlphaBlendEnable(false);
		m_pA3DDevice->SetAlphaTestEnable(true);
		m_pA3DDevice->SetAlphaFunction(A3DCMP_GREATER);
		m_pA3DDevice->SetAlphaRef(90); // 84 这里稍微增大一点, 不然会出现斑点

		A3DLitModelRender* pLitModelRender = pA3DEngine->GetA3DLitModelRender();
		m_pA3DDevice->SetDeviceVertexShader(A3DLMVERTEX_FVF);
		m_pHLSLGBufferAlpha->Appear();

		pLitModelRender->RenderRaw(pParam->pViewport->GetA3DViewport());
	}
	*/

	//渲染SkinModel
	m_pA3DDevice->SetFaceCull(A3DCULL_CCW);
	m_pA3DDevice->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, 
		D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);

	if(pParam->pSkinRender && pParam->bRenderSkin)
	{
		m_pA3DDevice->SetAlphaBlendEnable(false);
		m_pA3DDevice->SetAlphaTestEnable(true);
		m_pA3DDevice->SetAlphaFunction(A3DCMP_GREATER);
		m_pA3DDevice->SetAlphaRef(84);

		pParam->pSkinRender->RenderRaw(pParam->pViewport->GetA3DViewport(), m_pHLSLGBufferSkinMesh, m_pHLSLGBufferRigidMesh);
	}



	//还原SkinRender
	pA3DEngine->GetA3DSkinMan()->SetCurSkinRender(pSkinRenderOld);
	
	//清空所有可能填充的内容, 例如LitModela Gfx等等
	pA3DEngine->GetA3DSkinMan()->GetCurSkinRender()->ResetRenderInfo(false);
	pA3DEngine->GetA3DSkinMan()->GetCurSkinRender()->ResetRenderInfo(true);
	pA3DEngine->GetA3DLitModelRender()->ResetRenderInfo(false);
	pA3DEngine->GetA3DLitModelRender()->ResetRenderInfo(true);
	m_GameObjs.pA3DGFXExMan->SetCustomGfxRenderContainer(NULL);
	
	
	//还原RenderState
	m_pA3DDevice->SetFaceCull(A3DCULL_CCW);

	m_pA3DDevice->ClearVertexShader();
	m_pA3DDevice->ClearPixelShader();

	m_pA3DDevice->SetFVF(A3DFVF_A3DVERTEX);
	m_pA3DDevice->SetAlphaBlendEnable(bOldAlphaBlendEnable);
	m_pA3DDevice->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, dwOldColorWriteEnable);
	m_pA3DDevice->SetAlphaTestEnable(bOldAlphaTestEnable);
	m_pA3DDevice->SetZTestEnable(bOldZTestEnable);
	m_pA3DDevice->SetZWriteEnable(bOldZWriteEnable);
	m_pA3DDevice->SetFaceCull(oldCullType);
	m_pA3DDevice->SetAlphaFunction(oldCmpFunc);
	m_pA3DDevice->SetAlphaRef(dwOldAlphaRef);
	
	return true;
}


bool ElementRender::RenderToRenderTarget(ECViewport* pViewport, const ELEMENT_RENDER_PARAM* pParam, A3DRenderTarget* pFrameBuffer, bool bIsScreen)
{
    A3D_ITTX_SCOPED_TASK_NAMED(AGPAPerfIntegration::GetInstance().GetA3DDomain(), TEXT(__FUNCTION__));
    A3DEngine* pA3DEngine = m_pA3DDevice->GetA3DEngine();
    pA3DEngine->GetA3DEnvironment()->SetDNFactor(pParam->fDNFactor);
    
	m_pA3DDevice->SetAlphaRef(84);
    m_pA3DDevice->SetAlphaTestEnable(true);
    m_pA3DDevice->SetZWriteEnable(true);
    m_pA3DDevice->SetZTestEnable(true);

    ECRENDERPARAM ECRParam(*pParam, m_pViewport);
    A3DViewport* pA3DViewport = pViewport->GetA3DViewport();
    const D3DPRESENT_PARAMETERS& d3dpp = m_pA3DDevice->GetD3DPP();

    bool bEnablePostEffect = pParam->bRenderPostProcess && (pA3DViewport->GetParam()->Width == d3dpp.BackBufferWidth)
        && (pA3DViewport->GetParam()->Height == d3dpp.BackBufferHeight);
	bool bEnableSSAO = pParam->bRenderSSAO && (m_pSSAO != NULL);
   
    
	
	
	
	//	后处理系统准备
    m_pPostSystem->BeforeRender();
	
	//	渲染阴影
	A3DShadowMap* pShadowMap = m_pA3DDevice->GetA3DEngine()->GetA3DShadowMap();
	bool bUseShadowMap = (pShadowMap != NULL);
	if (pParam->bRenderShadow && bUseShadowMap)
    {
        A3D_ITTX_SCOPED_TASK_NAMED(AGPAPerfIntegration::GetInstance().GetA3DDomain(), TEXT("RenderShadowMap"));     
      
		
		A3DVECTOR3 lightDir = pParam->paramLight.Direction;

        lightDir.Normalize();
        float fRadius = 10.f;		

		int nShadowMapSize;
		A3D_SHADOW_MAP_SIZE eShadowMapSize = pParam->eShadowMapSize;
		if(eShadowMapSize == A3D_SHADOW_MAP_SIZE_LARGE)
			nShadowMapSize = 2048;
		else if(eShadowMapSize == A3D_SHADOW_MAP_SIZE_MEDIUM)
			nShadowMapSize = 1024;
		else
			nShadowMapSize = 512;
		if(pShadowMap->GetShadowMapSize() != nShadowMapSize)
			pShadowMap->SetShadowMapSize(nShadowMapSize);

		int nShadowSampleNum;
		A3D_SHADOW_MAP_SAMPLE_NUM eShadowSampleNum = pParam->eShadowSampleNum;
		if(eShadowSampleNum == A3D_SHADOW_MAP_SAMPLE_NUM_HIGH)
			nShadowSampleNum = 1;
		else if(eShadowSampleNum == A3D_SHADOW_MAP_SAMPLE_NUM_NORMAL)
			nShadowSampleNum = 1;
		else
			nShadowSampleNum = 0;

		pShadowMap->SetSampleLevel(nShadowSampleNum);

		float fDis = pParam->fShadowDistance;
		fDis = max(2.f, fDis);
		fDis = min(100.f, fDis);

		A3DVECTOR3 vShadowCenter = pA3DViewport->GetCamera()->GetPos();
		if(pParam->bUseCustomShadowCenter)
			vShadowCenter = pParam->vCustomShadowCenter;

        pShadowMap->SetTSM(pA3DViewport->GetCamera(), vShadowCenter, fDis, lightDir);

        m_pA3DDevice->Clear(D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, 0xffffffff, 1.0f, 0);

		SHADOWPASS_PARAM ShadowParam;
		ECViewport ECShadowViewport;

		ECShadowViewport.InitFromA3D(pShadowMap->GetShadowViewport()); 
		ShadowParam.pViewport = &ECShadowViewport;
		ShadowParam.pSkinRender = m_GameObjs.pSkinRenderShadow;
		ShadowParam.bRenderTerrain = false;
		ShadowParam.bRenderLitmodel = false;
		ShadowParam.bRenderSkin = true;	

        ShadowPass(&ShadowParam);	//此函数之前需要设置好RenderTarget, 之后恢复Rendertarget. 此函数内部负责渲染状态设置和还原.

        pShadowMap->Restore();

  		//if(GetAsyncKeyState(VK_F8) & 0x8000)
  		//{
  		//	D3DXSaveSurfaceToFileA("ShadowMap.dds", D3DXIFF_DDS, pShadowMap->GetShadowMapRT()->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
  		//}
		//pShadowMap->BlurVSM();
    }


    //	各种Manager的注册, 确保所有LitModel和Skin都注册进
    RenderManagers(pViewport, false);

    if( pParam->bRenderGFX)
    {
        if (m_GameObjs.pGFXCaster) 
            m_GameObjs.pGFXCaster->Render(&ECRParam);
    }


    pViewport->Activate(false);

    m_pA3DDevice->Clear(D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, pParam->crClear, 1.f, 0);

    //ZPrePass  暂时不开启
    m_bUnLinearOnRT = false;
    if (pParam->bRenderZPressPass && false)
    { 
		A3D_ITTX_SCOPED_TASK_NAMED(AGPAPerfIntegration::GetInstance().GetA3DDomain(), TEXT("ZPrePass"));
        //渲染ZPrepass
        ZPREPASS_PARAM ZPrePassParam;
        ZPrePassParam.pViewport = pViewport;
        ZPrePassParam.pSkinRender = m_GameObjs.pSkinRender;
        ZPrePassParam.bRenderScene = pParam->bRenderScene;
        ZPrePassParam.bRenderTerrain = pParam->bRenderTerrain;
        ZPrePassParam.bRenderLitmodel = pParam->bRenderLitModel;
        ZPrePassParam.bRenderSkin = pParam->bRenderSkinModel;

        //渲染出后处理需要用到深度
        if(pParam->bRenderZToRenderTarget == 1 && GetUnLinearZRT() != NULL)
        {
            m_pPostSystem->ClearRenderTarget(GetUnLinearZRT(), 
                m_pPostSystem->GetWidth(), m_pPostSystem->GetHeight(), A3DVECTOR4(1.f, 1.f, 1.f, 1.f));

            GetUnLinearZRT()->ApplyToDevice();
            m_bUnLinearOnRT = true;
        }

        ZPrePass(&ZPrePassParam);//此函数之前设置好RenderTarget, 之后恢复Rendertarget. 此函数内部负责将渲染状态还原.

        if(pParam->bRenderZToRenderTarget == 1 && GetUnLinearZRT() != NULL)
        {
            GetUnLinearZRT()->WithdrawFromDevice();
        }
        m_pA3DDevice->Clear(D3DCLEAR_TARGET, pParam->crClear, 1.0f, 0);
	}
   
    if(pParam->bWireframeMode)
        m_pA3DDevice->SetDeviceRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    else
        m_pA3DDevice->SetDeviceRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

    {
        A3D_ITTX_SCOPED_TASK_NAMED(AGPAPerfIntegration::GetInstance().GetA3DDomain(), TEXT("RenderScene"));
        bool bUnLinearOnRT = false;
        // 天空渲染
        if(pParam->bRenderSky && !pParam->bUnderWater)
        {
            if (m_WorldObjs.pA3DSky)
            {
                m_WorldObjs.pA3DSky->SetCamera(pViewport->GetA3DCamera());
                m_WorldObjs.pA3DSky->Render();
            }
        }

        if (AfxGetA3DDistantObjectRender())
        {
            AfxGetA3DDistantObjectRender()->Render(pA3DViewport, false);
            AfxGetA3DDistantObjectRender()->ClearList();
        }

        if (pParam->bRenderScene)
        {
            ECRENDERPARAM ECRParam(*pParam, pViewport);
            A3DViewport* pA3DViewport = pViewport->GetA3DViewport();

            bool bRenderTerrain = true;
			bool bRenderFog = pA3DEngine->GetA3DEnvironment()->GetGlobalFogEnable();

			if(bEnableSSAO)
				pA3DEngine->GetA3DEnvironment()->SetGlobalFogEnable(false);

            // 生物群组的渲染
            if( pParam->bRenderSceneObject)
            {
                if(m_WorldObjs.pScene != NULL)
                    m_WorldObjs.pScene->Render(&ECRParam);
            }

            m_pA3DDevice->SetAlphaBlendEnable(false);
            m_pA3DDevice->SetAlphaTestEnable(true);
            m_pA3DDevice->SetAlphaFunction(A3DCMP_GREATER);
            m_pA3DDevice->SetAlphaRef(84);

            // LitModel 渲染(无 Alpha)
            if(pParam->bRenderLitModel)
            {
                m_pA3DDevice->SetAlphaBlendEnable(false);
                m_pA3DDevice->SetAlphaTestEnable(true);

                bool bForceNoHLSL = false;
                bool bUseLightMap = true;
                bUseShadowMap = true;

                pA3DEngine->GetA3DLitModelRender()->SetDNFactor(pParam->fDNFactor);

                pA3DEngine->GetA3DLitModelRender()->Render(pA3DViewport, false, 
				    pParam->bModelLightMap && bUseLightMap, 
				    pParam->bRenderShadow && bUseShadowMap);

                pA3DEngine->GetA3DLitModelRender()->ResetRenderInfo(false);
            }

            // SkinModel 的渲染(无 Alpha)
            if(pParam->bRenderSkinModel && m_GameObjs.pSkinRender)
            {
                m_GameObjs.pSkinRender->Render(pA3DViewport, false);
                m_GameObjs.pSkinRender->ResetRenderInfo(false);
			}

            // 地形渲染
            if( pParam->bRenderTerrain)
            {
                // 多层纹理地形的渲染
                if (m_WorldObjs.pA3DTerrain)
                {
                    m_WorldObjs.pA3DTerrain->SetDNFactor(pParam->fDNFactor);
                    m_WorldObjs.pA3DTerrain->Render(pA3DViewport, false, m_pTerrainRT, 0.0f, 0.0f, false, false, 
                        pParam->bRenderShadow && bUseShadowMap);
                }
                if (m_WorldObjs.pMaskModifier)
                {
                    m_WorldObjs.pMaskModifier->Render(&ECRParam);
                }
                if (m_WorldObjs.pTerrainOutline)
                {
                    m_WorldObjs.pTerrainOutline->SetDNFactor(pParam->fDNFactor);
                    m_WorldObjs.pTerrainOutline->Render2(m_pViewport->GetA3DViewport(), 
                        pParam->bRenderShadow && bUseShadowMap);
                }
			}

            // 草的渲染
            if( pParam->bRenderGrass)
            {
                RenderGrasses(pParam, false);
			}

			// Draw mirror planes
			if( pParam->bRenderMirrorPlane )
			{
				MirrorReflectedRenderParam sMirrorReflectedRenderParam;
				sMirrorReflectedRenderParam.pECRenderParam = &ECRParam;
				sMirrorReflectedRenderParam.pElementRenderParam = pParam;
				m_pMirrorPlaneRender->Render( pA3DViewport, WorldRenderForMirrorReflected, (void*)&sMirrorReflectedRenderParam );
			}

            if (bIsScreen)
            {
                //遮挡剪裁
                A3DOcclusionMan* pOccMan = m_pA3DDevice->GetA3DEngine()->GetA3DOcclusionMan();
                if (pOccMan && g_pA3DConfig->RT_GetUseOcclusionCullingFlag())	// 这时的Zbuffer是不透明物体的Zbuffer
                {
                    A3DRenderTarget* pINTZ = m_pA3DDevice->GetIntZRenderTarget();
                    if (pINTZ && pINTZ->GetDepthTexture())
                    {
                        pOccMan->PrepareCoverageMap(pViewport->GetA3DViewport(), pINTZ, true);
                    }
                    else
                    {
                        g_pA3DConfig->RT_SetUseOcclusionCullingFlag(false);
                    }
                }
			}
            pA3DEngine->FlushCachedAlphaMesh(pA3DViewport);
            m_pA3DDevice->SetAlphaTestEnable(false);
            m_pA3DDevice->SetAlphaBlendEnable(true);

            m_pA3DDevice->SetTextureColorOP(1, A3DTOP_DISABLE);
            m_pA3DDevice->SetTextureAlphaOP(1, A3DTOP_DISABLE);
			m_pA3DDevice->ClearTexture(1);

            // 阴影片的渲染  已经关闭
            //if( pParam->bRenderShadow && !bUseShadowMap)
            //{
                //RenderShadows(pParam);
            //}
			
			pA3DEngine->GetA3DEnvironment()->SetGlobalFogEnable(bRenderFog);
			//渲染SSAO
			if(bEnableSSAO && m_pSSAO)
			{
				m_pSSAO->Render(pA3DViewport, pFrameBuffer);
			}

			float fWaterHeight = 0.0f;
			if (m_WorldObjs.pA3DTerrainWater2)
				fWaterHeight = m_WorldObjs.pA3DTerrainWater2->GetWaterHeight(pA3DViewport->GetCamera()->GetPos());
            // 水面渲染
            if (pParam->bRenderWater && pFrameBuffer && m_WorldObjs.pA3DTerrainWater2 
                && m_WorldObjs.pA3DTerrainWater2->GetNumWaterAreas()
                && pParam->nWaterEffect != 4)
            {
                // 渲染水之前的ALPHA
                if (!pParam->bUnderWater)
                    RenderAlphaObjects(pA3DViewport, pParam, !pParam->bUnderWater, fWaterHeight, true);

                
				int nWaterLevel = 1;
				
				switch (pParam->nWaterEffect)
				{
				case 1:         nWaterLevel = A3DTerrainWater2::WRL_NORMAL_CUBE_REFLECT; break;
				case 2: case 3: nWaterLevel = A3DTerrainWater2::WRL_NORMAL; break;
				default:        nWaterLevel = A3DTerrainWater2::WRL_SIMPLE;
				}
				//如果没有IntZ则采用WRL_SIMPLE的方法渲染
				if(!m_pA3DDevice->GetIntZRenderTarget())
					nWaterLevel = A3DTerrainWater2::WRL_SIMPLE;
				
				A3DRenderTargetMan* pRTMan = m_pA3DDevice->GetRenderTargetMan();
                A3DRenderTarget* pTempFrame = NULL;
				
				if(nWaterLevel != A3DTerrainWater2::WRL_SIMPLE)
				{
					pTempFrame = pRTMan->RentRenderTargetColor(pA3DViewport->GetParam()->Width, pA3DViewport->GetParam()->Height, A3DFMT_A8R8G8B8);
					m_pA3DDevice->StretchRect(pFrameBuffer->GetTargetSurface()->m_pD3DSurface, NULL, 
						pTempFrame->GetTargetSurface()->m_pD3DSurface, NULL, A3DTEXF_NONE);
				}

                A3DTerrainWater2Render::RenderParam renderParam;
                renderParam.pCurViewport = pA3DViewport;
                renderParam.pTerrain = m_WorldObjs.pA3DTerrain;
                renderParam.pfnReflectCallBack = WorldRenderForReflected;
                renderParam.pfnRefractCallBack = WorldRenderForRefracted;
                renderParam.pArg = (void*) pParam;//  NULL;//&g_Game;
                renderParam.pFrameRT = pTempFrame;
                renderParam.pDepthRT = m_pA3DDevice->GetIntZRenderTarget();
                if (m_pWaterCubeMap)
                {
                    ASSERT(m_pWaterCubeMap->GetClassID() == A3D_CID_TEXTURECUBE);
                }
                renderParam.pCubeReflect = (A3DCubeTexture*) m_pWaterCubeMap;
                if (pParam->bRenderSky)
                    renderParam.pSky = m_WorldObjs.pA3DSky;
                else
                    renderParam.pSky = NULL;

                m_WorldObjs.pA3DTerrainWater2->SetWaterRenderLevel(nWaterLevel);
                m_WorldObjs.pA3DTerrainWater2->Render(&renderParam);

				if(nWaterLevel != A3DTerrainWater2::WRL_SIMPLE)
					pRTMan->ReturnRenderTarget(pTempFrame);

                pTempFrame = NULL;
			}
#if _ENABLE_FFT_WATER_
			if(pParam->bRenderWater && m_WorldObjs.pA3DTerrainWaterFFT && pFrameBuffer)
			{
				A3DRenderTargetMan* pRTMan = m_pA3DDevice->GetRenderTargetMan();
				A3DRenderTarget* pTempFrame = pRTMan->RentRenderTargetColor(pA3DViewport->GetParam()->Width, pA3DViewport->GetParam()->Height, A3DFMT_A8R8G8B8);
				m_pA3DDevice->StretchRect(pFrameBuffer->GetTargetSurface()->m_pD3DSurface, NULL, 
					pTempFrame->GetTargetSurface()->m_pD3DSurface, NULL, A3DTEXF_NONE);

				pTempFrame->ApplyToDevice();

				float fWaterHeightBias = 0.35f * m_WorldObjs.pA3DTerrainWaterFFT->GetParam().m_fWavesSize;
				if(pParam->bUnderWater)
					fWaterHeightBias = -fWaterHeightBias;
				
				m_pA3DDevice->SetAlphaBlendEnable(true);
				m_pA3DDevice->SetZTestEnable(true);
				m_pA3DDevice->SetZWriteEnable(false);

				RenderAlphaObjects(pA3DViewport, pParam, !pParam->bUnderWater, fWaterHeight + fWaterHeightBias, true);

				pTempFrame->WithdrawFromDevice();

				A3DCamera* pCamera = dynamic_cast<A3DCamera*>(pA3DViewport->GetCamera());
				if(pCamera)
				{
					A3DVECTOR3 vSunDir;
					if(m_WorldObjs.pSunMoon)
						vSunDir = m_WorldObjs.pSunMoon->ComputeSunDir();
					else
						vSunDir = m_pA3DDevice->GetA3DEngine()->GetA3DEnvironment()->GetDirLightParam()->Direction;
					
					 
					A3DTerrainWaterFFT::RenderParam renderParam;
					if (pParam->bRenderSky)
						renderParam.pSky = m_WorldObjs.pA3DSky;
					else
						renderParam.pSky = NULL;
				
					renderParam.pCamera = pCamera;
					renderParam.pInputRT = pTempFrame;
					renderParam.vecSunDir = vSunDir;
					renderParam.pCurrentViewport = pA3DViewport;
					renderParam.pTerrain = m_WorldObjs.pA3DTerrain;
					renderParam.pTerrainOutline = NULL;
					renderParam.pReflectCallBack = WorldRenderForReflected;
					renderParam.pArg = (void*)pParam;
					renderParam.pfnRenderOnSky = NULL;
					renderParam.pArgSky = NULL;
					renderParam.pBloomRT = NULL;

					m_pA3DDevice->SetAlphaBlendEnable(false);
					m_pA3DDevice->SetZTestEnable(true);
					m_pA3DDevice->SetZWriteEnable(true);

					m_WorldObjs.pA3DTerrainWaterFFT->Render(&renderParam);

				}
				pRTMan->ReturnRenderTarget(pTempFrame);

			}
#endif
			m_bIsLinearZReady = false;
			//把渲染线性深度的操作移到渲染水之后
			if (bEnablePostEffect || bEnableSSAO)
			{    
				if(m_pA3DDevice->GetIntZRenderTarget())
				{
					if(RenderLinearZ(pA3DViewport, true))
						m_bIsLinearZReady = true;
				}
				else if(bUnLinearOnRT)
				{
					if(RenderLinearZ(pA3DViewport, false))
						m_bIsLinearZReady = true;
				}
				else
				{
					m_bIsLinearZReady = false;
				}
			}


            // 渲染水之后的ALPHA
			bool bAlphaBlendEnable = m_pA3DDevice->GetAlphaBlendEnable();
			bool bZTestEnable = m_pA3DDevice->GetZTestEnable();
			bool bZWriteEnable = m_pA3DDevice->GetZWriteEnable();

			m_pA3DDevice->SetAlphaBlendEnable(true);
			m_pA3DDevice->SetZTestEnable(true);
			m_pA3DDevice->SetZWriteEnable(false);

			RenderAlphaObjects(pA3DViewport, pParam, pParam->bUnderWater, fWaterHeight, false);

            // clear litmodel and skinmodel render
            if(pParam->bRenderLitModel)
            {
                pA3DEngine->GetA3DLitModelRender()->ResetRenderInfo(true);
			}

            if(pParam->bRenderSkinModel && m_GameObjs.pSkinRender)
            {
                m_GameObjs.pSkinRender->ResetRenderInfo(true);
			}

            // 自然物(雨,雪粒子渲染)
			RenderNatureObjects(pParam);

            // 草地(带Alpha)
            if( pParam->bRenderGrass)
            {
                RenderGrasses(pParam, true);
            }

            if (m_WorldObjs.pCloudRender)
            {
                m_WorldObjs.pCloudRender->Render(&ECRParam);
            }

            //	Render post GFX
            if( pParam->bRenderGFX)
            {
                if (m_GameObjs.pA3DGFXExMan)
                {
                    if (m_GameObjs.pA3DGFXExMan->IsDelayedEleExists())
                    {
                        A3DRenderTarget* pTargetTemp = m_pA3DDevice->GetRenderTargetMan()->RentRenderTargetColor(
                            m_pFrameBuffer->GetWidth(), m_pFrameBuffer->GetHeight(), m_pFrameBuffer->GetFormat().fmtTarget);
                        if (pTargetTemp)
                        {
                            m_pA3DDevice->StretchRect(
                                m_pFrameBuffer->GetTargetSurface()->m_pD3DSurface, NULL, 
                                pTargetTemp->GetTargetSurface()->m_pD3DSurface, NULL, A3DTEXF_NONE);
                            m_GameObjs.pA3DGFXExMan->RenderDelayedEles(pA3DViewport, pTargetTemp);
                            m_pA3DDevice->GetRenderTargetMan()->ReturnRenderTarget(pTargetTemp);
                        }
                    }
                    
                    m_GameObjs.pA3DGFXExMan->ClearAllGfxLst();
                }
            }

			/*
			//	Render Gfx Caster
			if( pParam->bRenderGFX)
			{
				if (m_GameObjs.pGFXCaster) 
					m_GameObjs.pGFXCaster->Render(&ECRParam);

// 				g_pGame->GetGFXCaster()->Render(pViewport);
// 				pA3DEngine->RenderGFX(pViewport->GetA3DViewport(), 0xffffffff);
// 
// 				if (g_pA3DConfig->RT_GetShowBoundBoxFlag())
// 					g_pGame->GetA3DGFXExMan()->RenderAllGfxAABB();

				m_GameObjs.pA3DGFXExMan->RenderAllSkinModels(pViewport->GetA3DViewport());
				A3DSkinRenderBase* psr1 = m_GameObjs.pSkinRender;

				//	Render non alpha skin models
				psr1->Render(pViewport->GetA3DViewport(), false);
				psr1->ResetRenderInfo(false);
				psr1->RenderAlpha(pViewport->GetA3DViewport());
				psr1->ResetRenderInfo(true);

				if(pParam->bRenderSkinModel && m_GameObjs.pSkinRender)
				{
					m_GameObjs.pSkinRender->ResetRenderInfo(true);
				}

				float fWaterHeightBias = pParam->bUnderWater ? 0.5f : -0.5f;
				m_GameObjs.pA3DGFXExMan->RenderAllGfxWater(pViewport->GetA3DViewport(), fWaterHeight + fWaterHeightBias, pParam->bUnderWater);

				m_GameObjs.pA3DGFXExMan->ClearAllGfxLst();
			}
			*/




			m_pA3DDevice->SetAlphaBlendEnable(bAlphaBlendEnable);
			m_pA3DDevice->SetZTestEnable(bZTestEnable);
			m_pA3DDevice->SetZWriteEnable(bZWriteEnable);
        }

    }

	m_pA3DDevice->SetDeviceRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    AfxGetA3DDistantObjectRender()->ClearList();

    return true;
}



//////////////////////////////////////////////////////////////////////////
bool ElementRender::Tick(DWORD dwDeltaTime)
{
	return true;
}

void ElementRender::RegisterManager(ECManager* pManager)
{
    if (m_aManagers.Find(pManager) >= 0)
        return;
    m_aManagers.Add(pManager);
}

void ElementRender::UnRegisterManager(ECManager* pManager)
{
    int iIndex = m_aManagers.Find(pManager);
    if (iIndex >= 0)
        m_aManagers.RemoveAt(iIndex);
}

bool ElementRender::RenderLinearZ(A3DViewport* pViewport, bool bUseIntZ)
{
	A3DRenderTarget* pUnLinearZRT = NULL;
	if(m_bUnLinearOnRT)
		pUnLinearZRT = m_pUnLinearZRt;
	return m_pPostSystem->RenderLinearZ(pViewport, m_pZRt, m_pUnLinearZRt, bUseIntZ);
}

void ElementRender::RegisterGameObjects(const GameObjects& objs)
{
    m_GameObjs = objs;
}

void ElementRender::RegisterWorldObjects(const WorldObjects& objs)
{
    m_WorldObjs = objs;
}

void ElementRender::ReloadWaterCubeMap(const AString& strFileName)
{
    A3DTextureMan* pTextureMan = m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan();
    A3DTexture* pTexture = NULL;
    pTextureMan->ReleaseTexture(m_pWaterCubeMap);
    m_strWaterCubeMap = strFileName;

    if (strFileName == "" || !pTextureMan->LoadTextureFromFile(m_strWaterCubeMap, &pTexture, A3DTF_TEXTURECUBE))
    {
		if (strFileName != "")
			a_LogOutput(1, "ElementRender::RegisterWorldObjects, Failed to load cube texture: %s, default used", strFileName);
        m_strWaterCubeMap = "shaders\\textures\\Water\\CubeTexture.dds";
        pTextureMan->LoadTextureFromFile(m_strWaterCubeMap, &pTexture, A3DTF_TEXTURECUBE);
    }
    m_pWaterCubeMap = pTexture;
}

void ElementRender::SetWaterReflectMap(const char* szFileName)
{
    ReloadWaterCubeMap(szFileName);
}

bool ElementRender::RenderAlphaObjects(A3DViewport* pViewport, const ELEMENT_RENDER_PARAM* pParam,
									   bool bUnderWater, float fWaterHeight, bool bEnableClipPlane)
{
    A3DEngine* pA3DEngine = m_pA3DDevice->GetA3DEngine();
    
    // turn on alpha test, and set ref value to a small value to mask off pure transparent area.
    m_pA3DDevice->SetAlphaTestEnable(true);
    m_pA3DDevice->SetAlphaRef(0x1);

    if (m_WorldObjs.pA3DTerrainWater2 && m_WorldObjs.pA3DTerrainWater2->GetNumWaterAreas())
    {
        // Set clip plane
        D3DXPLANE p;
        if (bUnderWater)
            p = D3DXPLANE(0.0f, -1.0f, 0.0f, fWaterHeight);
        else
            p = D3DXPLANE(0.0f, 1.0f, 0.0f, -fWaterHeight);

		if(bEnableClipPlane)
		{
			m_pA3DDevice->SetClipPlaneProgrammable(0, &p, &pViewport->GetCamera()->GetVPTM());
			m_pA3DDevice->SetDeviceRenderState(D3DRS_CLIPPLANEENABLE, D3DCLIPPLANE0);
		}
    }

	bool bOldWriteZ = m_pA3DDevice->GetZWriteEnable();
	m_pA3DDevice->SetZWriteEnable(m_bAlphaModelWriteZ);
    // LitModel渲染(带Alpha)
    if(pParam->bRenderLitModel)
    {
        pA3DEngine->GetA3DLitModelRender()->RenderAlpha(pViewport);
    }
    // SkinModel 渲染(带Alpha)
    if (pParam->bRenderSkinModel && m_GameObjs.pSkinRender)
    {
        m_GameObjs.pSkinRender->RenderAlpha(pViewport);
    }
	m_pA3DDevice->SetZWriteEnable(bOldWriteZ);	


    if (pParam->bRenderGFX && m_GameObjs.pA3DGFXExMan)
    {
        float fWaterHeightBias = bUnderWater ? 0.5f : -0.5f;
        m_GameObjs.pA3DGFXExMan->RenderAllGfxWater(pViewport, fWaterHeight + fWaterHeightBias, bUnderWater);
    }
    
    if (m_WorldObjs.pA3DTerrainWater2)
    {
        m_pA3DDevice->SetDeviceRenderState(D3DRS_CLIPPLANEENABLE, 0);
    }

    // turn off alpha test again
    m_pA3DDevice->SetAlphaTestEnable(false);
    m_pA3DDevice->SetAlphaRef(84);

    return true;
}

bool ElementRender::CreateCCVolumeTexture( const char* szOutputTextureFileName,
	const A3DPOST_EFFECT_CC_PARAM::ManualParam& manualParam)
{
	return A3DPostCC::CreateCCVolumeTexture(m_pA3DDevice, szOutputTextureFileName, NULL, manualParam);
}

void ElementRender::RenderWireFlat()
{
	m_pA3DDevice->SetZTestEnable(true);
	m_pA3DDevice->SetZWriteEnable(true);
	m_pA3DDevice->GetA3DEngine()->GetA3DWireCollector()->Flush();
	m_pA3DDevice->GetA3DEngine()->GetA3DFlatCollector()->Flush();
}

bool ElementRender::IsAlphaModelWriteZ()
{
	return m_bAlphaModelWriteZ;
}

void ElementRender::SetAlphaModelWriteZ( bool bAlphaModelWriteZ )
{
	m_bAlphaModelWriteZ = bAlphaModelWriteZ;
}

bool ElementRender::RenderPlayerDepthForSilhouette(A3DSkinRenderBase* pSkinRender, ECViewport* pECViewport, A3DRenderTarget* pTargetRT)
{
	//先把当前状态保存下来.
	DWORD dwState;
	bool bOldAlphaBlendEnable;
	DWORD dwOldColorWriteEnable;
	bool bOldAlphaTestEnable;
	bool bOldZTestEnable;
	bool bOldZWriteEnable;
	A3DCULLTYPE oldCullType;
	A3DCMPFUNC oldCmpFunc;
	DWORD dwOldAlphaRef;

	dwState = m_pA3DDevice->GetDeviceRenderState(D3DRS_ALPHABLENDENABLE);
	bOldAlphaBlendEnable = (dwState != 0 ? true : false);
	dwOldColorWriteEnable = m_pA3DDevice->GetDeviceRenderState(D3DRS_COLORWRITEENABLE);
	bOldAlphaTestEnable = m_pA3DDevice->GetAlphaTestEnable();
	dwState = m_pA3DDevice->GetDeviceRenderState(D3DRS_ZENABLE);
	bOldZTestEnable = (dwState != 0 ? true : false);
	dwState = m_pA3DDevice->GetDeviceRenderState(D3DRS_ZWRITEENABLE);
	bOldZWriteEnable = (dwState != 0 ? true : false);
	oldCullType = m_pA3DDevice->GetFaceCull();
	oldCmpFunc = m_pA3DDevice->GetAlphaFunction();	
	dwOldAlphaRef = m_pA3DDevice->GetAlphaRef();

	A3DEngine* pA3DEngine = m_pA3DDevice->GetA3DEngine();
	A3DSkinRenderBase* pSkinRenderOld = pA3DEngine->GetA3DSkinMan()->GetCurSkinRender();
	pA3DEngine->GetA3DSkinMan()->SetCurSkinRender(pSkinRender);
	GfxRenderContainer rc;
	m_GameObjs.pA3DGFXExMan->SetCustomGfxRenderContainer(&rc);

	//设置 SkinRender 正在渲染勾边的标记
	const DWORD nOldSkinRenderFlags = pSkinRender->GetRenderFlags();
	pSkinRender->SetRenderFlags(A3DSkinRenderBase::RENDER_FLAG_FOR_SILHOUETTE);

	//注册
	pSkinRender->ResetRenderInfo(false);
	pSkinRender->ResetRenderInfo(true);

	for (int i = 0; i < m_aManagers.GetSize(); i++)
	{
		if (m_aManagers[i])
		{
			m_aManagers[i]->RenderForSilhouette(pECViewport);
		}
	}

	//设置必要的Rendertarget
	m_pA3DDevice->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN
		| D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);



	
	m_pA3DDevice->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, 
		D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);

	
	m_pA3DDevice->SetAlphaBlendEnable(false);
	m_pA3DDevice->SetAlphaTestEnable(true);
	m_pA3DDevice->SetAlphaFunction(A3DCMP_GREATER);
	m_pA3DDevice->SetAlphaRef(84);
	m_pA3DDevice->SetZTestEnable(true);
	m_pA3DDevice->SetZWriteEnable(false);		//不写入Z!
	m_pA3DDevice->SetZFunc(A3DCMP_LESSEQUAL);
	m_pA3DDevice->SetFaceCull(A3DCULL_NONE);

	pTargetRT->ApplyToDevice();
	m_pA3DDevice->Clear(D3DCLEAR_TARGET, 0xffffffff, 1.0f, 0);

	bool bRenderLinearDepth = true;
	if(bRenderLinearDepth)
		pSkinRender->RenderRaw(pECViewport->GetA3DViewport(), m_pHLSLGBufferSkinMesh_LinearDepth, m_pHLSLGBufferRigidMesh_LinearDepth);
	else
		pSkinRender->RenderRaw(pECViewport->GetA3DViewport(), m_pHLSLGBufferSkinMesh, m_pHLSLGBufferRigidMesh);

	//恢复 SkinRender 渲染标记
	pSkinRender->SetRenderFlags(nOldSkinRenderFlags);


	pTargetRT->WithdrawFromDevice();

	//还原SkinRender
	pA3DEngine->GetA3DSkinMan()->SetCurSkinRender(pSkinRenderOld);
	
	//清空所有可能填充的内容, 例如LitModela Gfx等等
	pA3DEngine->GetA3DSkinMan()->GetCurSkinRender()->ResetRenderInfo(false);
	pA3DEngine->GetA3DSkinMan()->GetCurSkinRender()->ResetRenderInfo(true);
	pA3DEngine->GetA3DLitModelRender()->ResetRenderInfo(false);
	pA3DEngine->GetA3DLitModelRender()->ResetRenderInfo(true);
	m_GameObjs.pA3DGFXExMan->SetCustomGfxRenderContainer(NULL);
	
	//还原RenderState
	m_pA3DDevice->ClearVertexShader();
	m_pA3DDevice->ClearPixelShader();

	m_pA3DDevice->SetFVF(A3DFVF_A3DVERTEX);
	m_pA3DDevice->SetAlphaBlendEnable(bOldAlphaBlendEnable);
	m_pA3DDevice->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, dwOldColorWriteEnable);
	m_pA3DDevice->SetAlphaTestEnable(bOldAlphaTestEnable);
	m_pA3DDevice->SetZTestEnable(bOldZTestEnable);
	m_pA3DDevice->SetZWriteEnable(bOldZWriteEnable);
	m_pA3DDevice->SetFaceCull(oldCullType);
	m_pA3DDevice->SetAlphaFunction(oldCmpFunc);
	m_pA3DDevice->SetAlphaRef(dwOldAlphaRef);

	return true;
}

bool ElementRender::RenderSilhouetteByAlpha(A3DViewport* pViewport, A3DRenderTarget* pTargetRT)
{
    if (!m_pPostSystem)
        return false;

    bool bSilhouette = m_pPostSystem->IsActive(m_pPostSystem->m_pPostSilhouette);
    bool bAA = m_pPostSystem->IsActive(m_pPostSystem->m_pPostAA);
    if (!bSilhouette && !bAA)
        return false;

    A3DRenderTarget* pTempRT = m_pA3DDevice->GetRenderTargetMan()->RentRenderTargetColor(pTargetRT->GetWidth(), 
        pTargetRT->GetHeight(), pTargetRT->GetFormat().fmtTarget);
    if (!pTempRT)
        return false;

    m_pA3DDevice->CopySurface(pTempRT->GetTargetSurface()->m_pD3DSurface, NULL, pTargetRT->GetTargetSurface()->m_pD3DSurface, NULL, D3DX_FILTER_NONE);
    
    //D3DXSaveSurfaceToFileA("before.dds", D3DXIFF_DDS, pTargetRT->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
    //D3DXSaveSurfaceToFileA("temp.dds", D3DXIFF_DDS, pTempRT->GetTargetSurface()->m_pD3DSurface, NULL, NULL);

    //先把当前状态保存下来.
    bool bOldAlphaBlendEnable = m_pA3DDevice->GetAlphaBlendEnable();
    bool bOldAlphaTestEnable = m_pA3DDevice->GetAlphaTestEnable();
    bool bOldZTestEnable = m_pA3DDevice->GetZTestEnable();
    bool bOldZWriteEnable = m_pA3DDevice->GetZWriteEnable();
    A3DCULLTYPE oldCullType = m_pA3DDevice->GetFaceCull();
    DWORD dwOldColorWrite = m_pA3DDevice->GetDeviceRenderState(D3DRS_COLORWRITEENABLE);
    bool bOldFogEnable = m_pA3DDevice->GetFogEnable();

    m_pA3DDevice->SetAlphaBlendEnable(false);
    m_pA3DDevice->SetAlphaTestEnable(false);
    m_pA3DDevice->SetZTestEnable(false);
    m_pA3DDevice->SetZWriteEnable(false);
    m_pA3DDevice->SetFaceCull(A3DCULL_NONE);
    m_pA3DDevice->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, 
        D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN);
    m_pA3DDevice->SetFogEnable(false);

    m_pA3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
    m_pA3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
    m_pA3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    m_pA3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

    m_pPostSystem->m_pStream->Appear(0, false);
    m_pPostSystem->m_pStreamDecl->Appear();

    if (bSilhouette)
    {
        m_pPostSystem->m_pPostSilhouette->RenderForRT(pViewport, pTempRT, pTargetRT, A3DVECTOR4(0, 0, 0, 0.01f));
        if (bAA)
        {
            m_pA3DDevice->CopySurface(pTempRT->GetTargetSurface()->m_pD3DSurface, NULL, pTargetRT->GetTargetSurface()->m_pD3DSurface, NULL, D3DX_FILTER_NONE);
        }
    }
    
    if (bAA)
    {
        m_pPostSystem->m_pPostAA->RenderForRT(pViewport, pTempRT, pTargetRT);
    }

    //还原渲染状态
    m_pA3DDevice->SetAlphaBlendEnable(bOldAlphaBlendEnable);
    m_pA3DDevice->SetAlphaTestEnable(bOldAlphaTestEnable);
    m_pA3DDevice->SetZTestEnable(bOldZTestEnable);
    m_pA3DDevice->SetZWriteEnable(bOldZWriteEnable);
    m_pA3DDevice->SetFaceCull(oldCullType);
    m_pA3DDevice->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, dwOldColorWrite);
    m_pA3DDevice->SetFogEnable(bOldFogEnable);
    m_pA3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    m_pA3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    m_pA3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
    m_pA3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);


//    D3DXSaveSurfaceToFileA("after.dds", D3DXIFF_DDS, pTargetRT->GetTargetSurface()->m_pD3DSurface, NULL, NULL);

    m_pA3DDevice->GetRenderTargetMan()->ReturnRenderTarget(pTempRT);

    return true;
}

bool ElementRender::RenderBloomSkinModel(ECViewport* pViewport)
{
	A3DRenderTarget* pRT = m_pPostSystem->m_pBloomMeshRT;
	if (NULL == pRT || NULL == pViewport)
		return false;
	A3DViewport* pAViewport = pViewport->GetA3DViewport();
	if (NULL == pAViewport)
		return false;

	pRT->ApplyToDevice();
	m_pA3DDevice->Clear(D3DCLEAR_TARGET, 0x0, 1.0f, 0);
	//	Save states
	bool bOldAlphaBlendEnable = m_pA3DDevice->GetAlphaBlendEnable();
	bool bOldAlphaTestEnable = m_pA3DDevice->GetAlphaTestEnable();
	bool bOldZTestEnable = m_pA3DDevice->GetZTestEnable();
	bool bOldZWriteEnable = m_pA3DDevice->GetZWriteEnable();
	A3DCULLTYPE oldCullType = m_pA3DDevice->GetFaceCull();
	DWORD dwOldColorWrite = m_pA3DDevice->GetDeviceRenderState(D3DRS_COLORWRITEENABLE);
	bool bOldFogEnable = m_pA3DDevice->GetFogEnable();

	m_pA3DDevice->SetAlphaBlendEnable(false);
	m_pA3DDevice->SetAlphaTestEnable(true);
	m_pA3DDevice->SetZTestEnable(true);
	m_pA3DDevice->SetZWriteEnable(true);
	m_pA3DDevice->SetFaceCull(A3DCULL_CCW);
	m_pA3DDevice->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, 
		D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN);
	m_pA3DDevice->SetFogEnable(false);

	m_pA3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
	m_pA3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
	m_pA3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_pA3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	m_GameObjs.pSkinRender->RenderBloomMeshes(pAViewport, m_pParams->paramPostEffect.bloomParam.fBrightScale);

	//	Restore
	m_pA3DDevice->SetAlphaBlendEnable(bOldAlphaBlendEnable);
	m_pA3DDevice->SetAlphaTestEnable(bOldAlphaTestEnable);
	m_pA3DDevice->SetZTestEnable(bOldZTestEnable);
	m_pA3DDevice->SetZWriteEnable(bOldZWriteEnable);
	m_pA3DDevice->SetFaceCull(oldCullType);
	m_pA3DDevice->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, dwOldColorWrite);
	m_pA3DDevice->SetFogEnable(bOldFogEnable);
	m_pA3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	m_pA3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	m_pA3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pA3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	pRT->WithdrawFromDevice();
	return true;
}

void ElementRender::SetPostEffectParam(const A3DPOST_EFFECT_PARAM& paramPost)
{
    if (m_pPostSystem)
        m_pPostSystem->ApplyParam(&paramPost);
}
//////////////////////////////////////////////////////////////////////////
ECRENDERPARAM::ECRENDERPARAM(const ELEMENT_RENDER_PARAM& ERParam, ECViewport* pECViewport):  
    pViewport(pECViewport)
    , paramLight		(ERParam.paramLight)
	, fDNFactor			(ERParam.fDNFactor)
	, nWaterEffect		(ERParam.nWaterEffect)
	, bUnderWater		(ERParam.bUnderWater)
	, bModelLightMap	(ERParam.bModelLightMap)
{

}
//////////////////////////////////////////////////////////////////////////
void WorldRenderOnSky(A3DViewport * pViewport, void * pArg)
{
	const ELEMENT_RENDER_PARAM* pParam = (const ELEMENT_RENDER_PARAM*)pArg;
	pParam->pSelf->RenderOnSky(pParam);
}

void WorldRenderForReflected(A3DViewport * pViewport, void * pArg)
{
	const ELEMENT_RENDER_PARAM* pParam = (const ELEMENT_RENDER_PARAM*)pArg;
	pParam->pSelf->RenderForReflected(pParam, pViewport);
}

void WorldRenderForRefracted(A3DViewport * pViewport, void * pArg)
{
	const ELEMENT_RENDER_PARAM* pParam = (const ELEMENT_RENDER_PARAM*)pArg;
	pParam->pSelf->RenderForRefracted(pParam, pViewport);
}

void WorldRenderForMirrorReflected(A3DViewport * pViewport, void * pArg)
{
	const ElementRender::MirrorReflectedRenderParam* pMirrorReflectedParam = (const ElementRender::MirrorReflectedRenderParam*)pArg;
	pMirrorReflectedParam->pElementRenderParam->pSelf->RenderForMirrorReflected(pMirrorReflectedParam, pViewport);
}
