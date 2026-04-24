#ifdef _ANGELICA22
#define _PLATFORM_RENDER
#include "stdafx.h"
#include "AF.h"
#include "A3D.h"
#include "MainFrm.h"
#include "LogDlg.h"
#include "A3DGFXExMan.h"
#include "A3DSkinRenderBase.h"
#include "PlatformRenderer.h"
#include "Global.h"
#include "ElementRender.h"
#include "ECViewport.h"
#include "Render.h"
#include "SceneObjectManager.h"
#include "TerrainRender.h"
#include "TerrainWater.h"
#include "SceneBoxArea.h"
#include "EL_CloudSceneRender.h"
#include "ECRObjBase.h"
#include "MaskModifier.h"
#include "EL_CloudSceneRender.h"
#include "A3DTerrain2Env.h"


class MaskModifierWrapper : public ECRObjBase
{
public:
    CMaskModifier* m_pMaskModifier;
    virtual bool Render(const ECRENDERPARAM* pRenderParam)
    {
        return m_pMaskModifier->Render(pRenderParam->pViewport->GetA3DViewport());
    }

};

class CloudRenderWrapper : public ECRObjBase
{
public:
    CELCloudSceneRender* m_pCloudRender;
    virtual bool Render(const ECRENDERPARAM* pRenderParam)
    {
        m_pCloudRender->Render(pRenderParam->pViewport->GetA3DViewport());
        return true;
    }
};

class SceneObjectManagerWrapper : public ECManager
{
public:
    CSceneObjectManager* m_pObjMan;
    SceneObjectManagerWrapper() : ECManager(NULL) {}
    virtual bool Render(CECViewport* pViewport)
    {
        m_pObjMan->RenderObjects(pViewport->GetA3DViewport());
        return true;
    }
    virtual bool RenderPost(CECViewport* pViewport)
    {
        m_pObjMan->PostRenderObjects(pViewport->GetA3DViewport());
        return true;
    }

    virtual bool RenderForReflect(ECViewport * pViewport)
    {
        m_pObjMan->RenderObjects(pViewport->GetA3DViewport());
        return true;
    }

    virtual bool RenderForRefract(ECViewport * pViewport)
    {
        m_pObjMan->RenderObjects(pViewport->GetA3DViewport());
        return true;
    }

};

PlatformRenderer::PlatformRenderer()
{
    m_pMap = NULL;
    m_pElementRender = NULL;
    m_pSkinRender2 = NULL;
}

PlatformRenderer::~PlatformRenderer()
{
    Release();
}

bool PlatformRenderer::Render(A3DViewport* pViewport, CElementMap* pMap)
{
    m_pMap = pMap;

    if (!m_pMap)
        return false;

	bool bRealTime = m_pMap->IsRealTimeLight();

	g_Render.GetA3DEngine()->GetA3DLitModelRender()->SetRealTimeLight(bRealTime);
	g_Render.GetA3DEngine()->GetA3DLitModelRender()->SetDetailMap(m_pMap->IsEnableDetail());
	g_Render.GetA3DEngine()->GetA3DLitModelRender()->SetClipParam(10000, 0.0f);
	A3D::g_pA3DTerrain2Env->SetRealTimeLight(bRealTime);


/*	//	Get specified pixel shader
	A3DPixelShader* m_pps3LayersLM = A3D::g_pA3DTerrain2Env->GetPS3LayersLM();
	A3DPixelShader* m_pps3LayersLMAlphaShadow =A3D::g_pA3DTerrain2Env->GetPS3LayersLMAlphaShadow();
	A3DPixelShader* m_pps3LayersVL = A3D::g_pA3DTerrain2Env->GetPS3LayersVL();
	A3DPixelShader* m_ppsTexMerge = A3D::g_pA3DTerrain2Env->GetPSTexMerge();
	A3DPixelShader* m_ppsTMLM = A3D::g_pA3DTerrain2Env->GetPSTMLM();

	//	A3D::g_pA3DTerrain2Env->Get specified hlsl
	A3DHLSL* pHlsl3 =		A3D::g_pA3DTerrain2Env->GetHLSL3LayersLM(false);//(bool bShadowMap) const;
	A3DHLSL* pHlsl6 =		A3D::g_pA3DTerrain2Env->GetHLSL6LayersLM(false);//(bool bShadowMap) const;
	A3DVertexDecl* m_pvsDecl =	A3D::g_pA3DTerrain2Env->GetVertexDecl();

	//Render Terrain with HDR LightMap
	A3DPixelShader* m_pps3LayersNoLM = A3D::g_pA3DTerrain2Env->GetPS3LayersNoLM();
	A3DPixelShader* m_ppsLightMapHDR = A3D::g_pA3DTerrain2Env->GetPSLightMapHDR();
	A3DVertexShader* m_pvsLightMapHDR = A3D::g_pA3DTerrain2Env->GetVSLightMapHDR();
	A3DTexture*	m_pLightMapLookup =	A3D::g_pA3DTerrain2Env->GetHDRLookup();

	//Render Terrain one pass hdr lightmap
	A3DPixelShader* m_pps3LayersOnePassHDRLM = A3D::g_pA3DTerrain2Env->GetGetPS3LayersOnePassHDRLM();

	//	A3D::g_pA3DTerrain2Env->Get vertex shader
	A3DVertexShader* m_pvsTerrain = A3D::g_pA3DTerrain2Env->GetVertexShader();
	//	A3D::g_pA3DTerrain2Env->Get vertex shader for water refract rendering
	A3DVertexShader* m_pvsRefract = A3D::g_pA3DTerrain2Env->GetRefractVertexShader();
	//	A3D::g_pA3DTerrain2Env->Get mask texture format
	A3DFORMAT m_MaskMapFmt = A3D::g_pA3DTerrain2Env->GetMaskMapFormat();
	//	A3D::g_pA3DTerrain2Env->Get light map texture format
	A3DFORMAT m_LightMapFmt = A3D::g_pA3DTerrain2Env->GetLightMapFormat() ;
	//	Is light map 32 bit
	bool m_bLightMap32Bit = A3D::g_pA3DTerrain2Env->IsLightMap32Bit();
	//	A3D::g_pA3DTerrain2Env->Get support pixel shader flag
	bool m_bSupportPS = A3D::g_pA3DTerrain2Env->GetSupportPSFlag();
	//	A3D::g_pA3DTerrain2Env->Get pure white texture
	A3DTexture* m_pWhiteTex = A3D::g_pA3DTerrain2Env->GetWhiteTexture();
	//	A3D::g_pA3DTerrain2Env->Get unit white texture for color map (used in mask area)
	A3DTexture* m_pUnitWhiteTex = A3D::g_pA3DTerrain2Env->GetUnitWhiteTexture();

	//A3D::g_pA3DTerrain2Env->Get shadow shaders
	A3DVertexShader* m_pShadowMapVS = A3D::g_pA3DTerrain2Env->GetShadowMapVS();
	A3DPixelShader*	m_pShadowMapPS = A3D::g_pA3DTerrain2Env->GetShadowMapPS();
	A3DTexture* m_pLookupMap = A3D::g_pA3DTerrain2Env->GetLookupMap();

	A3DVertexShader* m_pTerrainShadowVS = A3D::g_pA3DTerrain2Env->GetShadowVS();
	A3DPixelShader*	m_pTerrainShadowPS = A3D::g_pA3DTerrain2Env->GetShadowPS();

	A3DVertexShader* m_pShadowMapL8VS = A3D::g_pA3DTerrain2Env->GetShadowMapL8VS();
	A3DPixelShader*	m_pShadowMapL8PS = A3D::g_pA3DTerrain2Env->GetShadowMapL8PS();
	A3DVertexShader* m_pShadowL8VS = A3D::g_pA3DTerrain2Env->GetShadowL8VS();
	A3DPixelShader*	m_pShadowL8PS=  A3D::g_pA3DTerrain2Env->GetShadowL8PS();

	//A3DHLSL* A3D::g_pA3DTerrain2Env->GetHLSLNewShader(int iLayer, bool bShadowMap, bool bShadowHigh) const;
*/
    g_Render.GetA3DEngine()->GetA3DEnvironment()->SetViewport(pViewport);
    CSceneObjectManager* pObjMan = m_pMap->GetSceneObjectMan();
    SceneObjectManagerWrapper objManWrapper;

    if (pObjMan)
    {
        objManWrapper.m_pObjMan = pObjMan;
        m_pElementRender->RegisterManager(&objManWrapper);
    }

    //g_pA3DConfig->RT_SetUseOcclusionCullingFlag(false);


    CSceneBoxArea* pCurBox = pObjMan->GetCurrentArea();
    
    
    g_Render.GetA3DDevice()->SetFillMode(A3DFILL_SOLID);
    ELEMENT_RENDER_PARAM renderParam;
    
    ECViewport ECViewportWrapper;

    ECViewportWrapper.InitFromA3D(pViewport);

    renderParam.bRenderZPressPass = 0;
    renderParam.bRenderZToRenderTarget = 0;
    renderParam.bAnimationWorld = false;
#ifdef _ANGELICA22_FEATURE
	renderParam.bModelLightMap = m_pMap->IsEnableLitmap();
	renderParam.bRenderGFX = g_Configs.bShowSpecially;
	renderParam.bRenderGrass = g_Configs.bShowGrass;
	renderParam.bRenderLitModel = g_Configs.bShowStaticModel;
#else
	renderParam.bModelLightMap = false;
	renderParam.bRenderGFX = g_Configs.bShowSpecially;
	renderParam.bRenderGrass = g_Configs.bShowGrass;
	renderParam.bRenderLitModel = g_Configs.bShowStaticModel;
#endif // #ifdef _ANGELICA22_FEATURE
	renderParam.bRenderPostProcess = g_Configs.bShowPostEffect;
    renderParam.bRenderScene = true;
    renderParam.bRenderSceneObject = true;
//    renderParam.bRenderShadowMapShadow = false;
    renderParam.bRenderShadow = false;
    renderParam.bRenderSkinModel = g_Configs.bShowSkinModel || g_Configs.bShowFixedNpc;
    renderParam.bRenderSky = g_Configs.bEnableSky;
    renderParam.bRenderTerrain = g_Configs.bShowTerrain;
    renderParam.bRenderUI = false;
    renderParam.bRenderWater = g_Configs.bShowWater;
    renderParam.bRenderZPressPass = false;
    renderParam.bRenderZToRenderTarget = false;
    renderParam.bUnderWater = false;
    renderParam.bWireframeMode = g_Configs.bWireFrame;
#ifdef _ANGELICA22_FEATURE
	renderParam.bRenderSunMoon = g_Configs.bShowSunMoon;
	renderParam.bRenderMirrorPlane = g_Configs.bShowMirrorPlane;
#else
	renderParam.bRenderSunMoon = true;
	renderParam.bRenderMirrorPlane = false;
#endif // #ifdef _ANGELICA22_FEATURE

    if (pMap->GetTerrainWater() && pMap->GetTerrainWater()->GetA3DTerrainWater())
    {
        renderParam.bUnderWater = pMap->GetTerrainWater()->GetA3DTerrainWater()->IsUnderWater(pViewport->GetCamera()->GetPos());
    }
    bool bUseOcc = GetAsyncKeyState('L') < 0;
    g_pA3DConfig->RT_SetUseOcclusionCullingFlag(bUseOcc);
    renderParam.fDNFactor = g_fNightFactor;
#ifdef _ANGELICA22_FEATURE
    renderParam.nWaterEffect = g_Configs.nWaterEffect;
#else
    renderParam.nWaterEffect = g_Configs.bSampleWater ? 0 : 2;
#endif // #ifdef _ANGELICA22_FEATURE
    renderParam.vHostPos = pViewport->GetCamera()->GetPos();
    renderParam.pSelf = m_pElementRender;
    renderParam.crClear = g_Render.GetA3DEngine()->GetA3DEnvironment()->GetFogParam()->crFog2;
    renderParam.paramLight = g_Render.GetDirLight()->GetLightparam();
#ifdef _ANGELICA22_FEATURE
    if (renderParam.bRenderPostProcess)
    {
        renderParam.paramPostEffect = g_PostEffectParam;
		// 调整需要应用夜间参数的PostEffectParam
		renderParam.paramPostEffect.bloomParam.fBlurSize = g_PostEffectParam.bloomParam.fBlurSize * (1 - g_fNightFactor) + g_PostEffectParamNight.bloomParam.fBlurSize * g_fNightFactor;
		renderParam.paramPostEffect.bloomParam.fBrightScale = g_PostEffectParam.bloomParam.fBrightScale * (1 - g_fNightFactor) + g_PostEffectParamNight.bloomParam.fBrightScale * g_fNightFactor;
		renderParam.paramPostEffect.bloomParam.fBrightThreshold = g_PostEffectParam.bloomParam.fBrightThreshold * (1 - g_fNightFactor) + g_PostEffectParamNight.bloomParam.fBrightThreshold * g_fNightFactor;
		renderParam.paramPostEffect.fullGlowParam.fBlurSize = g_PostEffectParam.fullGlowParam.fBlurSize * (1 - g_fNightFactor) + g_PostEffectParamNight.fullGlowParam.fBlurSize * g_fNightFactor;
		renderParam.paramPostEffect.fullGlowParam.fGlowLevel = g_PostEffectParam.fullGlowParam.fGlowLevel * (1 - g_fNightFactor) + g_PostEffectParamNight.fullGlowParam.fGlowLevel * g_fNightFactor;
		renderParam.paramPostEffect.fullGlowParam.fGlowPower = g_PostEffectParam.fullGlowParam.fGlowPower * (1 - g_fNightFactor) + g_PostEffectParamNight.fullGlowParam.fGlowPower * g_fNightFactor;
		renderParam.paramPostEffect.fullGlowParam.vGlowColor.x = g_PostEffectParam.fullGlowParam.vGlowColor.x * (1 - g_fNightFactor) + g_PostEffectParamNight.fullGlowParam.vGlowColor.x * g_fNightFactor;
		renderParam.paramPostEffect.fullGlowParam.vGlowColor.y = g_PostEffectParam.fullGlowParam.vGlowColor.y * (1 - g_fNightFactor) + g_PostEffectParamNight.fullGlowParam.vGlowColor.y * g_fNightFactor;
		renderParam.paramPostEffect.fullGlowParam.vGlowColor.z = g_PostEffectParam.fullGlowParam.vGlowColor.z * (1 - g_fNightFactor) + g_PostEffectParamNight.fullGlowParam.vGlowColor.z * g_fNightFactor;
		renderParam.paramPostEffect.fullGlowParam.vGlowColor.w = g_PostEffectParam.fullGlowParam.vGlowColor.w * (1 - g_fNightFactor) + g_PostEffectParamNight.fullGlowParam.vGlowColor.w * g_fNightFactor;

		if(g_fNightFactor>0.5f)
		{
			renderParam.paramPostEffect.ccParam.szTex1 = g_PostEffectParamNight.ccParam.szTex1;
			renderParam.paramPostEffect.ccParam.szTex2 = g_PostEffectParamNight.ccParam.szTex2;
			renderParam.paramPostEffect.ccParam.fFactor = g_PostEffectParamNight.ccParam.fFactor;
		}
        
        if (!renderParam.bUnderWater)
        {
            renderParam.paramPostEffect.dwPostEffectMask &= ~A3DPOST_EFFECT_Warp;
        }

		if (g_Configs.bShowSunMoon)
        {
			// 打开引擎会崩，待处理
		    renderParam.paramPostEffect.dwPostEffectMask &= ~A3DPOST_EFFECT_SunMoon;
            renderParam.paramPostEffect.dwPostEffectMask &= ~A3DPOST_EFFECT_Flare;
        }
        else
        {
            renderParam.paramPostEffect.dwPostEffectMask &= ~A3DPOST_EFFECT_SunMoon;
            renderParam.paramPostEffect.dwPostEffectMask &= ~A3DPOST_EFFECT_Flare;
        }
    }
#endif // #ifdef _ANGELICA22_FEATURE
    ElementRender::GameObjects gameObjs = {0};
    gameObjs.pA3DGFXExMan = AfxGetGFXExMan();
    gameObjs.pSkinRender = g_Render.GetA3DEngine()->GetA3DSkinMan()->GetCurSkinRender();
    gameObjs.pSkinRender2 = m_pSkinRender2;

    ElementRender::WorldObjects worldObjs = {0};
    worldObjs.pA3DSky = m_pMap->GetSceneObjectMan()->GetSky();
    worldObjs.pA3DTerrain = m_pMap->GetTerrain()->GetRender();
    worldObjs.pA3DTerrainWater2 = m_pMap->GetTerrainWater()->GetA3DTerrainWater();
    worldObjs.pGrassLand = m_pMap->GetSceneObjectMan()->GetElGrassLand();
#ifdef _ANGELICA22_FEATURE
    worldObjs.pSunMoon = m_pMap->GetSunMoon();
    if (pCurBox)
    {
        worldObjs.pSnow = g_Configs.bShowSnow ? pCurBox->GetSnow() : NULL;
        worldObjs.pRain = g_Configs.bShowRain ? pCurBox->GetRain() : NULL;
    }
    else
    {
        worldObjs.pSnow = NULL;
        worldObjs.pRain = NULL;
    }
#else
	worldObjs.pSnow = NULL;
	worldObjs.pRain = NULL;
#endif // #ifdef _ANGELICA22_FEATURE
    MaskModifierWrapper maskWrapper;
    if (pMap->GetTerrain() && pMap->GetTerrain()->GetMaskModifier())
    {
        maskWrapper.m_pMaskModifier = pMap->GetTerrain()->GetMaskModifier();
        worldObjs.pMaskModifier = &maskWrapper;
    }

    // Render for cloud
    CloudRenderWrapper cloudWrapper;
    if (pMap->GetCloudRender() && g_Configs.bShowCloudBox)
    {
        pMap->GetCloudRender()->SetRenderLine(g_Configs.bShowCloudLine);
        cloudWrapper.m_pCloudRender = pMap->GetCloudRender();
        worldObjs.pCloudRender = &cloudWrapper;
    }

    m_pElementRender->RegisterGameObjects(gameObjs);
    m_pElementRender->RegisterWorldObjects(worldObjs);
    m_pElementRender->Render(&ECViewportWrapper, &renderParam);
    m_pElementRender->UnRegisterManager(&objManWrapper);

#ifdef _ANGELICA22_FEATURE
    pMap->RenderNeighborTerrain(pViewport);
    //if (pObjMan)
    //{
    //    pObjMan->PostRenderObjects(pViewport);
    //}
    if (pMap->GetSpacePass()) pMap->GetSpacePass()->Render(g_Render.GetA3DEngine()->GetA3DWireCollector());
    // Render readonly flags
    if (g_bReadOnly ) 
    {
        A3DVIEWPORTPARAM *param = g_Render.GetA3DEngine()->GetActiveViewport()->GetParam();
        g_Render.TextOut(param->Width/2,param->Height/2,"**Read Only**",A3DCOLORRGB(255,0,0));
    }
    //pMap->GetPerfGraph()->Render();
#endif // #ifdef _ANGELICA22_FEATURE
    return true;
}

bool PlatformRenderer::Init()
{
	m_pElementRender = AfxGetElementRender();

	if( !m_pElementRender->Init( g_Render.GetA3DDevice() ) )
	{
		g_Log.Log( __FUNCTION__, ", Failed to initialize element render." );
		return false;
	}

	m_pSkinRender2 = g_Render.GetA3DEngine()->CreateSkinRender(true);
	return true;
}

void PlatformRenderer::Release()
{
    if (m_pElementRender)
    {
        m_pElementRender->Release();
        m_pElementRender = NULL;
    }
    A3DRELEASE(m_pSkinRender2);
}
#endif // #ifdef _ANGELICA22