#ifndef _ELEMENT_RENDER_PARAM_H_
#define _ELEMENT_RENDER_PARAM_H_
#include "A3DMacros.h"

class A3DViewport;
class A3DDevice;
class A3DGFXExMan;
class A3DSunMoon;
class A3DPostSystem;
class A3DTerrainOutline2;

class CELTerrainOutline2;
class CECScene;
class ECManager;
class ElementRender;

class CECGFXCaster;
class ECRObjBase;
class ECViewport;

class A3DGrassLand;
class A3DRain;
class A3DSnow;
class A3DLitModelRender;
class A3DGFXExMan;
class ECRObjBase;
class A3DRenderTarget;
class A3DOrthoCamera;

class A3DShadowMap;
class A3DSkinRenderBase;
class A3DTerrainWater2;

#include "ECManager.h"

//struct PLATFORM_RENDERINFO;

enum A3DPOST_EFFECT
{
    A3DPOST_EFFECT_None				= 0,			//效果全关
    A3DPOST_EFFECT_CC				= (1 << 0),		//色彩校正
    A3DPOST_EFFECT_GodRay			= (1 << 1),		//天光
    A3DPOST_EFFECT_Bloom			= (1 << 2),		//Bloom
    A3DPOST_EFFECT_AA				= (1 << 3),		//反走样
    A3DPOST_EFFECT_Dof				= (1 << 4),		//景深
    A3DPOST_EFFECT_RadialBlur		= (1 << 5),		//径向模糊
    A3DPOST_EFFECT_MotionBlur		= (1 << 6),		//运动模糊
    A3DPOST_EFFECT_Warp				= (1 << 7),		//空间扭曲
    A3DPOST_EFFECT_FullGlow			= (1 << 8),		//全屏泛光
    A3DPOST_EFFECT_SunMoon			= (1 << 9),		//太阳月亮
    A3DPOST_EFFECT_Flare			= (1 << 10),	//光圈
	A3DPOST_EFFECT_RadialBlurFast   = (1 << 11),	//快速径向模糊
	A3DPOST_EFFECT_Silhouette		= (1 << 12),	//勾边
	A3DPOST_EFFECT_BloomLocal		= (1 << 13),	// Bloom for specified skinmodel
    A3DPOST_EFFECT_All				= 0xffffffff,	//效果全开
};

struct A3DPOST_EFFECT_BLOOM_PARAM			//全屏泛光参数
{	
    float			fBlurSize;				//模糊大小, 占屏幕大小的百分比，0~0.1
	float			fLocalBlurSize;			//局部模糊大小, 占屏幕大小的百分比，0~0.1
    float			fBrightThreshold;		//高光阈值	0~1
    float			fBrightScale;			//亮度	0~5
    A3DPOST_EFFECT_BLOOM_PARAM()					
    {
        fBlurSize = 0.05f;
		fLocalBlurSize = 0.2f;
        fBrightThreshold = 0.8f;
        fBrightScale = 0.5f;
    }
};
struct A3DPOST_EFFECT_CC_PARAM				//色彩校正参数
{
	struct ManualParam
	{
		int nBright;						//亮度,		范围是[-100~100]
		int nContrast;						//对比度,	范围是[-100~100]
		int nHue;							//色相,		范围是[-180~180]
		int nSaturation;					//饱和度,		范围是[-100~100]
		bool bGrey;							//是否转换为灰度图
		bool bOpposition;					//是否反色
		ManualParam()
		{
			nBright = 0;
			nContrast = 0;
			nHue = 0;
			nSaturation = 0;
			bGrey = false;
			bOpposition = false;
		}
		bool EqualTo(const ManualParam& other)
		{
			return nBright == other.nBright && nContrast == other.nContrast && nHue == other.nHue
					&& nSaturation == other.nSaturation && bGrey == other.bGrey && bOpposition == other.bOpposition;
		}
	};
    float			fFactor;				//在两张映射贴图间过渡的值, 0~1
    bool			bUseManualParam1;		//是否使用人工设置的参数生成映射贴图
	bool			bUseManualParam2;		//是否使用人工设置的参数生成映射贴图
	ManualParam		manualParam1;			//第一张贴图的人工设置参数值	(仅当bUseManualParam1为true的时候才起效果)
	ManualParam		manualParam2;			//第二章贴图的人工设置参数值	(仅当bUseManualParam2为true的时候才起效果)
	AString			szTex1;					//第一张映射贴图的文件名		(仅当bUseManualParam1为false的时候才起效果)
    AString			szTex2;					//第二章映射贴图的文件名		(仅当bUseManualParam2为false的时候才起效果)
	bool			bAfterUI;
    A3DPOST_EFFECT_CC_PARAM()
    {
        fFactor = 0;
		bUseManualParam1 = false;
		bUseManualParam2 = false;
        szTex1 = "Shaders\\Textures\\ccdefault.dds";
        szTex2 = "Shaders\\Textures\\ccdefault.dds";
		bAfterUI = true;
    }
};
struct A3DPOST_EFFECT_DOF_PARAM				//景深参数
{
    float			fNearBlur;				//近处最模糊位置
    float			fNearFocus;				//近处焦点位置
    float			fFarFocus;				//远处焦点位置
    float			fFarBlur;				//远处最模糊位置
    float			fClampBlurFar;			//最远处模糊的上限
    A3DPOST_EFFECT_DOF_PARAM()
    {
        fNearBlur = 1.0f;
        fNearFocus = 20.f;
        fFarFocus = 50.f;
        fFarBlur = 100.f;
        fClampBlurFar = 1.f;
    }
};
struct A3DPOST_EFFECT_GODRAY_PARAM			//天光参数
{
	float			fOcclusionDepthRange;	//大于此深度的区域颜色会被保留进行Bloom,小于此深度的区域颜色会变为黑. [0 ~ 1]
	float			fBloomScale;			//Bloom叠加强度. 0为不叠加Bloom, 无光束. [0 ~ 5]
	float			fBloomThreshold;		//亮度大于此部分才会被保留进行Bloom. [0 ~ 1]
	float			fScreenBlendThreshold;	//屏幕亮度必须小于这个值才会接受Bloom的叠加, 越小叠加越多 [0 ~ 1]
	A3DVECTOR3		vBloomTintColor;		//场景颜色会乘以这个值之后再进行bloom. [三个0 ~ 1,分别表示RGB]
	float			fOcclusionDarkness;		//根据模糊后的OcclusionMask对屏幕颜色进行黑化, 0则最大程度黑化,1则不黑化 [0 ~ 1]
	float			fVisibleAngle;			//可见角度 [0 ~ 3.14]

    A3DPOST_EFFECT_GODRAY_PARAM()
    {
		fOcclusionDepthRange = 500.f;
		fBloomScale = 1.2f;
		fBloomThreshold = 0.37f;
		fScreenBlendThreshold = 1.0;
		vBloomTintColor = A3DVECTOR3(1,1,1);
		fOcclusionDarkness = 0.62f;
	    fVisibleAngle = 1.75f; //A3D_PI / 3.f;	
    }
};
struct A3DPOST_EFFECT_RADIUSBLUR_PARAM		//径向模糊参数
{
    float			fBlurCenterX;			//焦点, (0.5, 0.5)是屏幕中间, (0,0)是左上角, (1,1)是右下角
    float			fBlurCenterY;			
    float			fBlurLength;			//模糊长度 0~1
    float			fInFocusRadius;			//焦点半径,在该半径内的画面不会被模糊 0~2
    float			fOutFocusRadius;		//模糊半径,在该范围外模糊度最大. 0~3
    A3DPOST_EFFECT_RADIUSBLUR_PARAM()
    {		
        fBlurCenterX = 0.5f;
        fBlurCenterY = 0.5f;
        fBlurLength = 0.2f;
        fInFocusRadius = 0.2f;
        fOutFocusRadius = 0.7f;
    }
};

struct A3DPOST_EFFECT_RADIUSBLUR_FAST_PARAM		//快速径向模糊参数
{
	float			fBlurCenterX;			//焦点, (0.5, 0.5)是屏幕中间, (0,0)是左上角, (1,1)是右下角
	float			fBlurCenterY;			
	float			fBlurLength;			//模糊长度 0~1
	float			fInFocusRadius;			//焦点半径,在该半径内的画面不会被模糊 0~2
	float			fOutFocusRadius;		//模糊半径,在该范围外模糊度最大. 0~3
	A3DPOST_EFFECT_RADIUSBLUR_FAST_PARAM()
	{		
		fBlurCenterX = 0.5f;
		fBlurCenterY = 0.5f;
		fBlurLength = 0.2f;
		fInFocusRadius = 0.2f;
		fOutFocusRadius = 0.7f;
	}
};


struct A3DPOST_EFFECT_MOTIONBLUR_PARAM
{
    float fBlurMax;	       				//模糊量的最大值,0~1之间,一般0.03左右
    float fBlurScale;        				//模糊量控制，即模糊量与摄像机运动速度之间的关系,0
    float fBlurScalePosition;				//摄像机位置移动造成的模糊量的权重 1~10
    float fBlurScaleRotation;				//摄像机旋转造成的模糊的权重 1~10
    float fInFocusDistance;  				//在该深度范围内的像素不会被模糊
    float fOutFocusDistance; 				//在该深度范围之外的像素会被最大限度模糊

    A3DPOST_EFFECT_MOTIONBLUR_PARAM()
    {
        fBlurMax = 0.03f;
        fBlurScale = 0.05f;
        fBlurScalePosition = 3.0f;
        fBlurScaleRotation = 10.0f;
        fInFocusDistance = 15.f;
        fOutFocusDistance = 80.f;
    }
};

struct A3DPOST_EFFECT_FULLGLOW_PARAM
{
    float			fBlurSize;				//模糊距离，占屏幕大小的百分比，0~0.1
    float			fGlowLevel;
    float			fGlowPower;
    A3DVECTOR4		vGlowColor;
    A3DPOST_EFFECT_FULLGLOW_PARAM()
    {
        fBlurSize = 0.05f;
        fGlowLevel = 0.25f;
        fGlowPower = 0.2f;
        vGlowColor = A3DVECTOR4(0.8f, 0.7f, 0.5f, 0.5f);
    }
};

struct A3DPOST_EFFECT_WARP_PARAM
{
    float	fScale;			//	扭曲大小
    float	fSpeed;			//	扭曲速度
	bool	bEnableMask;	//	是否开启Mask
	AString strMaskTextureName;
    A3DPOST_EFFECT_WARP_PARAM()
    {
        fScale = 0.01f;
        fSpeed = 0.5f;
		bEnableMask = false;
		strMaskTextureName = "";
    }
};

struct A3DPOST_EFFECT_AA_PARAM
{
	bool bAfterUI;

	A3DPOST_EFFECT_AA_PARAM()
	{
		bAfterUI = false;
	}

};

struct A3DPOST_EFFECT_SILHOUETTE_PARAM
{
	float fHardness;
	float fBlurDist;
	A3DVECTOR3 vLineColor;

	A3DPOST_EFFECT_SILHOUETTE_PARAM()
	{
		fHardness = 0.8f;
		fBlurDist = 0.3f;
		vLineColor = A3DVECTOR3(0,0,0);
	}
};

struct A3DPOST_EFFECT_PARAM								//后处理系统参数
{
    DWORD							dwPostEffectMask;	//标记开启的效果
    A3DPOST_EFFECT_BLOOM_PARAM		bloomParam;			//Bloom
    A3DPOST_EFFECT_CC_PARAM			ccParam;			//色彩校正
    A3DPOST_EFFECT_DOF_PARAM		dofParam;			//景深
    A3DPOST_EFFECT_GODRAY_PARAM		godRayParam;		//GodRay
    A3DPOST_EFFECT_RADIUSBLUR_PARAM radiusBlurParam;	//径向模糊
    A3DPOST_EFFECT_MOTIONBLUR_PARAM motionBlurParam;	//运动模糊
    A3DPOST_EFFECT_WARP_PARAM		warpParam;			//水下扭曲
    A3DPOST_EFFECT_FULLGLOW_PARAM   fullGlowParam;		//FullGlow
	A3DPOST_EFFECT_AA_PARAM			aaParam;
	A3DPOST_EFFECT_RADIUSBLUR_FAST_PARAM radiusBlurFastParam;
	A3DPOST_EFFECT_SILHOUETTE_PARAM silhouetteParam;
    A3DPOST_EFFECT_PARAM()
    {
        dwPostEffectMask = 0;
    }
};

enum A3D_SHADOW_MAP_SIZE
{
	A3D_SHADOW_MAP_SIZE_SMALL = 0,
	A3D_SHADOW_MAP_SIZE_MEDIUM,
	A3D_SHADOW_MAP_SIZE_LARGE
};

enum A3D_SHADOW_MAP_SAMPLE_NUM
{
	A3D_SHADOW_MAP_SAMPLE_NUM_LOW = 0,
	A3D_SHADOW_MAP_SAMPLE_NUM_NORMAL,
	A3D_SHADOW_MAP_SAMPLE_NUM_HIGH
};

struct ELEMENT_RENDER_PARAM
{
    DWORD                   crClear;
    A3DLIGHTPARAM			paramLight;
    float					fDNFactor;
    A3DVECTOR3				vHostPos;
    ElementRender*			pSelf;				// ElementRender自己的指针

    unsigned int nWaterEffect; // 0 - 纹理动画， 1 - cube map反射， 2 - 真反射不带NPC和草，3 - 最高效果

	bool bUnderWater;
    bool bModelLightMap;
    bool bRenderScene;
    bool bRenderSky;
    bool bRenderTerrain;
    bool bRenderSceneObject;
    bool bAnimationWorld;
    bool bRenderLitModel;
    bool bRenderSkinModel;
    bool bRenderGrass;
    bool bRenderShadow;
    bool bRenderWater;
	bool bRenderMirrorPlane;
    bool bRenderGFX;
    bool bRenderPostProcess;
	bool bRenderSSAO;
    bool bRenderZPressPass;
    bool bRenderZToRenderTarget;
    bool bRenderSunMoon;
    bool bRenderUI;
	bool bWireframeMode;

	float fShadowDistance;							//人物阴影覆盖距离，建议5~30
	A3D_SHADOW_MAP_SIZE	eShadowMapSize; 
	A3D_SHADOW_MAP_SAMPLE_NUM eShadowSampleNum;
	bool bUseCustomShadowCenter;
	A3DVECTOR3 vCustomShadowCenter;

    A3DPOST_EFFECT_PARAM paramPostEffect;			//后处理参数

    ELEMENT_RENDER_PARAM()
    {
        pSelf = NULL;
		crClear = 0xFFFFFFFF;
		fDNFactor = 0.0f;
        nWaterEffect = 0;
        bUnderWater = true;
        bModelLightMap = true;
        bRenderScene = true;
        bRenderSky = true;
        bRenderTerrain= true;
        bRenderSceneObject = true;
        bAnimationWorld = true;
        bRenderLitModel = true;
        bRenderSkinModel = true;
        bRenderGrass = true;
        bRenderShadow = true;
        bRenderWater = true;
		bRenderMirrorPlane = true;
        bRenderGFX = true;
        bRenderPostProcess = true;
		bRenderSSAO = true;
        bRenderZPressPass = true;
        bRenderZToRenderTarget = true;
        bRenderUI= true;
        bRenderSunMoon = true;
		bWireframeMode = false;
		fShadowDistance = 10.f;
		eShadowMapSize = A3D_SHADOW_MAP_SIZE_MEDIUM;
		eShadowSampleNum = A3D_SHADOW_MAP_SAMPLE_NUM_NORMAL;
		bUseCustomShadowCenter = false;
    }
};

struct ZPREPASS_PARAM
{
    ECViewport*	pViewport;
    A3DSkinRenderBase*	pSkinRender;

    unsigned int bRenderScene: 1;
    unsigned int bRenderSkin : 1;
    unsigned int bRenderLitmodel : 1;
    unsigned int bRenderTerrain : 1;
};

struct SHADOWPASS_PARAM
{
    ECViewport*	pViewport;
    A3DSkinRenderBase*	pSkinRender;
    unsigned int bRenderSkin : 1;
    unsigned int bRenderLitmodel : 1;
    unsigned int bRenderTerrain : 1;
};

#endif