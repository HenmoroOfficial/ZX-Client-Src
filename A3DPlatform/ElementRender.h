#ifndef _A3D_PLATFORM_H_
#define _A3D_PLATFORM_H_

#include "A3D.h"
#include "ElementRenderParam.h"
#include "ECRObjBase.h"


class A3DHLSL;
class A3DGFXWarp;
class A3DSSAO;
class A3DTerrainWaterFFT;
class A3DMirrorPlaneRender;

class ElementRender : public A3DDevObject
{
public:
    // 整个游戏过程中不会变的对象
    struct GameObjects
    {
        A3DGFXExMan*		pA3DGFXExMan;
        ECRObjBase*			pGFXCaster;
        ECRObjBase*			pImgRes;
        ECRObjBase*			pUIManager;				//	UI manager
        A3DSkinRenderBase*	pSkinRender;
        A3DSkinRenderBase*	pSkinRender2;			// 折射反射用的
		A3DSkinRenderBase*	pSkinRenderShadow;		//渲染阴影用的
		A3DSkinRenderBase*	pSkinRenderSilhouette;	//渲染勾边用的
    };

    // 每个世界各自的对象
    struct WorldObjects
    {
        A3DSkySphere*		pA3DSky;
        A3DSunMoon*			pSunMoon;
        A3DTerrainOutline2* pTerrainOutline;
        A3DTerrain2*		pA3DTerrain;
        //A3DTerrainWater*	pA3DTerrainWater; //FIXME!!
		A3DTerrainWater2*	pA3DTerrainWater2;
		A3DTerrainWaterFFT* pA3DTerrainWaterFFT;
        ECRObjBase*			pScene;
        A3DGrassLand*		pGrassLand;
        A3DRain*			pRain;
        A3DSnow*			pSnow;
        ECRObjBase*         pMaskModifier;
        ECRObjBase*         pCloudRender;
    };

	// Reflected rendering parameters for mirror.
	struct MirrorReflectedRenderParam 
	{
		ECRENDERPARAM*				pECRenderParam;
		const ELEMENT_RENDER_PARAM*	pElementRenderParam;
	};

protected:
    // 禁止创建 ElementRender 对象，使用AfxGetElementRender()
    ElementRender();
    virtual ~ElementRender();

public:

	bool Init(A3DDevice* pA3DDevice);
	void Release();

	A3DDevice* GetA3DDevice() { return m_pA3DDevice; }

	bool Render(ECViewport*	pViewport, const ELEMENT_RENDER_PARAM* pParam, A3DRenderTarget* pFrameBuffer = NULL);

	bool Tick(DWORD dwDeltaTime);
    
    void RegisterManager(ECManager* pManager);
    void UnRegisterManager(ECManager* pManager);
    
    void RegisterGameObjects(const GameObjects& objs);
    void RegisterWorldObjects(const WorldObjects& objs);
    
    const GameObjects& GetGameObjs() const { return m_GameObjs; }
    const WorldObjects& GetWorldObjs() const { return m_WorldObjs; }
    const ELEMENT_RENDER_PARAM* GetParams() const { return m_pParams; }

	A3DMirrorPlaneRender* GetMirrorPlaneRender() { return m_pMirrorPlaneRender; }

    //	获取深度RT
	A3DRenderTarget* GetLinearZRT(){ return m_pZRt; }
	A3DRenderTarget* GetUnLinearZRT() { return m_pUnLinearZRt; }

	//	线性深度RT是否准备完成
	bool IsLinearZReady(){ return m_bIsLinearZReady; }

    void SetWaterReflectMap(const char* szFileName);
    const char* GetWaterReflectMap() const { return m_strWaterCubeMap; }
	
	//根据一组参数生成体积纹理.
	// szOutputTextureFileName: 输出的体积纹理文件路径名
	// nBright:		亮度,		范围是[-100~100]
	// nConstrast:	对比度,		范围是[-100~100]
	// nHue:		色度,		范围是[-180~180]
	// nSaturation:	饱和度,		范围是[-100~100]
	// bGrey:		是否转换为灰度图
	// bOpposition:	是否反色
	bool CreateCCVolumeTexture(const char* szOutputTextureFileName,
		const A3DPOST_EFFECT_CC_PARAM::ManualParam& manualParam);

	bool IsAlphaModelWriteZ();
	void SetAlphaModelWriteZ(bool bAlphaModelWriteZ);

    bool RenderSilhouetteByAlpha(A3DViewport* pViewport, A3DRenderTarget* pTargetRT);

    void SetPostEffectParam(const A3DPOST_EFFECT_PARAM& paramPost);
private:
	//	Before device reset
	virtual bool BeforeDeviceReset();
	//	After device reset
	virtual bool AfterDeviceReset();
	
    void RenderManagers(ECViewport* pViewport, bool bPost);
	bool RenderUI(const ELEMENT_RENDER_PARAM* pParam);

	bool ZPrePass(const ZPREPASS_PARAM* pParam);
	bool ShadowPass(SHADOWPASS_PARAM* pParam);

	bool RenderLinearZ(A3DViewport* pViewport, bool bUseIntZ);

	bool RenderOnSky(const ELEMENT_RENDER_PARAM* pParam);
	bool RenderGrasses(const ELEMENT_RENDER_PARAM* pParam, bool bRenderAlpha);
	bool RenderNatureObjects(const ELEMENT_RENDER_PARAM* pParam);

	bool RenderForRefracted(const ELEMENT_RENDER_PARAM* pParam, A3DViewport* pRefractedViewport);
	bool RenderForReflected(const ELEMENT_RENDER_PARAM* pParam, A3DViewport* pReflectedViewport);

	bool RenderForMirrorReflected(const MirrorReflectedRenderParam* pMirrorReflectedParam, A3DViewport* pReflectedViewport);

	bool InitFrameBuffer();

	friend void WorldRenderOnSky(A3DViewport * pViewport, void * pArg);
	friend void WorldRenderForReflected(A3DViewport * pViewport, void * pArg);
	friend void WorldRenderForRefracted(A3DViewport * pViewport, void * pArg);
	friend void WorldRenderForMirrorReflected(A3DViewport * pViewport, void * pArg);

    void ReloadWaterCubeMap(const AString& strFileName);

    bool RenderAlphaObjects(A3DViewport* pViewport, const ELEMENT_RENDER_PARAM* pParam, bool bUnderWater,
		float fWaterHeight, bool bEnableClipPlane);

	void RenderWireFlat();

    bool RenderToRenderTarget(ECViewport* pViewport, const ELEMENT_RENDER_PARAM* pParam, A3DRenderTarget* pFrameBuffer, bool bIsScreen);

	bool RenderPlayerDepthForSilhouette(A3DSkinRenderBase* pSkinRender, ECViewport* pECViewport, A3DRenderTarget* pTargetRT);

	bool RenderBloomSkinModel(ECViewport* pViewport);
private:
	A3DDevice*					m_pA3DDevice;
	A3DPostSystem*				m_pPostSystem;
	A3DRenderTarget*			m_pFrameBuffer;
	A3DRenderTarget*			m_pZRt;						//深度纹理,内存0-1范围的线性深度
	A3DRenderTarget*			m_pUnLinearZRt;				//非线性深度RT
	bool						m_bIsLinearZReady;
	bool						m_bNeedReset;
	A3DGFXWarp*         		m_pGfxWarp;

	A3DHLSL*					m_pHLSLGBuffer;				//不考虑纹理的AlphaTest, 用在地形.
	A3DHLSL*					m_pHLSLGBufferAlpha;		//考虑纹理的AlphaTest
	A3DHLSL*					m_pHLSLGBufferSkinMesh;		//渲染SKinMesh
	A3DHLSL*					m_pHLSLGBufferRigidMesh;	//渲染SKinRigidMesh
	A3DHLSL*					m_pHLSLGBufferSkinMesh_LinearDepth;		//渲染SKinMesh, 颜色输出线性深度
	A3DHLSL*					m_pHLSLGBufferRigidMesh_LinearDepth;	//渲染SKinRigidMesh, 颜色输出线性深度
	AArray<ECManager*>			m_aManagers;
	A3DRenderTarget*			m_pTerrainRT;
	GameObjects         		m_GameObjs;
	WorldObjects        		m_WorldObjs;

	A3DLitModelRender*			m_pLitModelRenderExtra;
	A3DMirrorPlaneRender*		m_pMirrorPlaneRender;		// The renderer used to draw mirror plane objects.

	ECViewport*         		m_pViewport;
	A3DTexture*         		m_pWaterCubeMap;
	bool						m_bSupportSM20;
	AString             		m_strWaterCubeMap;

	A3DSSAO*					m_pSSAO;
	const ELEMENT_RENDER_PARAM*	m_pParams;
    bool                        m_bInitialized;

	bool						m_bAlphaModelWriteZ;

	bool						m_bUnLinearOnRT;
};

ElementRender* AfxGetElementRender();

#endif // _A3D_PLATFORM_H_