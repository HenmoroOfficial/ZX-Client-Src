/*
 * FILE: ElementMap.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/6/15
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once


#include "EditObject.h"
#include "Terrain.h"
#include "Global.h"
#include "AArray.h"
#include "vector.h"
#include "A3DTrace.h"
#include "EL_CloudSceneRender.h"
#include "SPOctree\SpacePassableOctree.h"

class CELBuilding;
class CPlayerPassMapGenerator;
class CPlayerAirPassMapGenerator;

#ifdef _ANGELICA22_FEATURE
struct RAYTRACE_CONFIG;
#endif

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////
//	Load flag

enum
{
	LOAD_EXPTERRAIN = 0,	//	Load for exporting terrain
	LOAD_EXPLIGHTMAP,		//	Load for exporting lightmap
	LOAD_EXPSCENE,			//	Load for exporting scene data
	LOAD_NORMAL,            //  Load for normal render
	LOAD_HISTORY_COMPARE,	//  Load for history compare
};

///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

class CLight;
class CSceneObjectManager;
class CTerrainWater;
class CELBitmap;
class CBrushTree;
class CHostPlayer;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////
#pragma pack (push, 1)

//	Map file header
struct MAPFILEHEADER
{
	DWORD	dwIdentity;		//	Map file flag, EPRJFILE_IDENTIFY
	DWORD	dwVersion;		//	Map file version, EPRJFILE_VERSION
	BYTE	aReserved[64];	//	Reserved
};


//	Edit record file data
struct EDITRECORDFILEDATA
{
	DWORD	dwTrnRndUpdate2;	//	Terrain render update counter 2
	BYTE	aReserved[64];		//	Reserved
};

#ifdef _ANGELICA22_FEATURE
// post effect data for saving and exporting
struct EDITOR_DATA_POST_EFFECT_PARAM
{
	enum EDITOR_DATA_POST_EFFECT_ENABLE_FLAGS
	{
		ENABLE_NONE = 0x0,
		ENABLE_DOF = 1 << 0,
		ENABLE_BLOOM_DAY = 1 << 1,
		ENABLE_BLOOM_NIGHT = 1 << 2,
		ENABLE_FULLGLOW_DAY = 1 << 3,
		ENABLE_FULLGLOW_NIGHT = 1 << 4,
		ENABLE_UNITED_WARP = 1 << 5,

		ENABLE_ALL = -1
	} FLAGS_ENABLE;
	struct EDITOR_DATA_POST_EFFECT_DOF_PARAM
	{
		float			fNearBlur;				//近处最模糊位置
		float			fNearFocus;				//近处焦点位置
		float			fFarFocus;				//远处焦点位置
		float			fFarBlur;				//远处最模糊位置
		float			fClampBlurFar;			//最远处模糊的上限
		EDITOR_DATA_POST_EFFECT_DOF_PARAM()
		{
			fNearBlur = 1.0f;
			fNearFocus = 20.f;
			fFarFocus = 50.f;
			fFarBlur = 100.f;
			fClampBlurFar = 1.f;
		}
		EDITOR_DATA_POST_EFFECT_DOF_PARAM(const A3DPOST_EFFECT_DOF_PARAM& rA3DParam)
		{
			fNearBlur = rA3DParam.fNearBlur;
			fNearFocus = rA3DParam.fNearFocus;
			fFarFocus = rA3DParam.fFarFocus;
			fFarBlur = rA3DParam.fFarBlur;
			fClampBlurFar = rA3DParam.fClampBlurFar;
		}
		EDITOR_DATA_POST_EFFECT_DOF_PARAM& operator=(const A3DPOST_EFFECT_DOF_PARAM& rA3DParam)
		{
			fNearBlur = rA3DParam.fNearBlur;
			fNearFocus = rA3DParam.fNearFocus;
			fFarFocus = rA3DParam.fFarFocus;
			fFarBlur = rA3DParam.fFarBlur;
			fClampBlurFar = rA3DParam.fClampBlurFar;
			return *this;
		}
		void CopyParamTo(A3DPOST_EFFECT_DOF_PARAM& rA3DParam) const
		{
			rA3DParam.fNearBlur = fNearBlur;
			rA3DParam.fNearFocus = fNearFocus;
			rA3DParam.fFarFocus = fFarFocus;
			rA3DParam.fFarBlur = fFarBlur;
			rA3DParam.fClampBlurFar = fClampBlurFar;
		}
	} PARAM_DOF;
	struct EDITOR_DATA_POST_EFFECT_BLOOM_PARAM
	{
		float			fBlurSize;				//模糊大小, 占屏幕大小的百分比，0~0.1
		float			fBrightThreshold;		//高光阈值	0~1
		float			fBrightScale;			//亮度	0~5
		EDITOR_DATA_POST_EFFECT_BLOOM_PARAM()					
		{
			fBlurSize = 0.05f;
			fBrightThreshold = 0.8f;
			fBrightScale = 0.5f;
		}
		EDITOR_DATA_POST_EFFECT_BLOOM_PARAM(const A3DPOST_EFFECT_BLOOM_PARAM& rA3DParam)					
		{
			fBlurSize = rA3DParam.fBlurSize;
			fBrightThreshold = rA3DParam.fBrightThreshold;
			fBrightScale = rA3DParam.fBrightScale;
		}
		EDITOR_DATA_POST_EFFECT_BLOOM_PARAM& operator=(const A3DPOST_EFFECT_BLOOM_PARAM& rA3DParam)
		{
			fBlurSize = rA3DParam.fBlurSize;
			fBrightThreshold = rA3DParam.fBrightThreshold;
			fBrightScale = rA3DParam.fBrightScale;
			return *this;
		}
		void CopyParamTo(A3DPOST_EFFECT_BLOOM_PARAM& rA3DParam) const
		{
			rA3DParam.fBlurSize = fBlurSize;
			rA3DParam.fBrightThreshold = fBrightThreshold;
			rA3DParam.fBrightScale = fBrightScale;
		}
	};
	EDITOR_DATA_POST_EFFECT_BLOOM_PARAM PARAM_BLOOM_DAY;
	EDITOR_DATA_POST_EFFECT_BLOOM_PARAM PARAM_BLOOM_NIGHT;
	struct EDITOR_DATA_POST_EFFECT_FULLGLOW_PARAM
	{
		float			fBlurSize;				//模糊距离，占屏幕大小的百分比，0~0.1
		float			fGlowLevel;
		float			fGlowPower;
		A3DVECTOR4		vGlowColor;
		EDITOR_DATA_POST_EFFECT_FULLGLOW_PARAM()
		{
			fBlurSize = 0.05f;
			fGlowLevel = 0.25f;
			fGlowPower = 0.2f;
			vGlowColor = A3DVECTOR4(0.0f, 0.0f, 1.f, 0.5);
		}
		EDITOR_DATA_POST_EFFECT_FULLGLOW_PARAM(const A3DPOST_EFFECT_FULLGLOW_PARAM& rA3DParam)
		{
			fBlurSize = rA3DParam.fBlurSize;
			fGlowLevel = rA3DParam.fGlowLevel;
			fGlowPower = rA3DParam.fGlowPower;
			vGlowColor = rA3DParam.vGlowColor;
		}
		EDITOR_DATA_POST_EFFECT_FULLGLOW_PARAM& operator=(const A3DPOST_EFFECT_FULLGLOW_PARAM& rA3DParam)
		{
			fBlurSize = rA3DParam.fBlurSize;
			fGlowLevel = rA3DParam.fGlowLevel;
			fGlowPower = rA3DParam.fGlowPower;
			vGlowColor = rA3DParam.vGlowColor;
			return *this;
		}
		void CopyParamTo(A3DPOST_EFFECT_FULLGLOW_PARAM& rA3DParam) const
		{
			rA3DParam.fBlurSize = fBlurSize;
			rA3DParam.fGlowLevel = fGlowLevel;
			rA3DParam.fGlowPower = fGlowPower;
			rA3DParam.vGlowColor = vGlowColor;
		}
	};
	EDITOR_DATA_POST_EFFECT_FULLGLOW_PARAM PARAM_FULLGLOW_DAY;
	EDITOR_DATA_POST_EFFECT_FULLGLOW_PARAM PARAM_FULLGLOW_NIGHT;
	struct EDITOR_DATA_POST_EFFECT_UNITED_WARP_PARAM
	{
		float	fScale;			//	扭曲大小
		float	fSpeed;			//	扭曲速度
		EDITOR_DATA_POST_EFFECT_UNITED_WARP_PARAM()
		{
			fScale = 0.01f;
			fSpeed = 0.5f;
		}
		EDITOR_DATA_POST_EFFECT_UNITED_WARP_PARAM(const A3DPOST_EFFECT_WARP_PARAM& rA3DParam)
		{
			fScale = rA3DParam.fScale;
			fSpeed = rA3DParam.fSpeed;
		}
		EDITOR_DATA_POST_EFFECT_UNITED_WARP_PARAM& operator=(const A3DPOST_EFFECT_WARP_PARAM& rA3DParam)
		{
			fScale = rA3DParam.fScale;
			fSpeed = rA3DParam.fSpeed;
			return *this;
		}
		void CopyParamTo(A3DPOST_EFFECT_WARP_PARAM& rA3DParam) const
		{
			rA3DParam.fScale = fScale;
			rA3DParam.fSpeed = fSpeed;
		}
	} PARAM_UNITED_WARP;
	struct EDITOR_DATA_POST_EFFECT_UNITED_WARP_EX1_PARAM
	{
		bool	bEnableMask;	//	是否开启Mask
		AString strMaskTextureName;
		EDITOR_DATA_POST_EFFECT_UNITED_WARP_EX1_PARAM()
		{
			bEnableMask = false;
			strMaskTextureName = "";
		}
		EDITOR_DATA_POST_EFFECT_UNITED_WARP_EX1_PARAM(const A3DPOST_EFFECT_WARP_PARAM& rA3DParam)
		{
			bEnableMask = rA3DParam.bEnableMask;
			strMaskTextureName = rA3DParam.strMaskTextureName;
		}
		EDITOR_DATA_POST_EFFECT_UNITED_WARP_EX1_PARAM& operator=(const A3DPOST_EFFECT_WARP_PARAM& rA3DParam)
		{
			bEnableMask = rA3DParam.bEnableMask;
			strMaskTextureName = rA3DParam.strMaskTextureName;
			return *this;
		}
		void CopyParamTo(A3DPOST_EFFECT_WARP_PARAM& rA3DParam) const
		{
			rA3DParam.bEnableMask = bEnableMask;
			rA3DParam.strMaskTextureName = strMaskTextureName;
		}
	} PARAM_UNITED_WARP_EX1;
	struct EDITOR_DATA_POST_EFFECT_CC_PARAM
	{
		float			fFactor;				//在两张映射贴图间过渡的值, 0~1
		bool			bUseManualParam1;		//是否使用人工设置的参数生成映射贴图
		bool			bUseManualParam2;		//是否使用人工设置的参数生成映射贴图
		bool			bAfterUI;
		EDITOR_DATA_POST_EFFECT_CC_PARAM()
		{
			fFactor = 0;
			bUseManualParam1 = false;
			bUseManualParam2 = false;
			bAfterUI = true;
		}
		EDITOR_DATA_POST_EFFECT_CC_PARAM(const A3DPOST_EFFECT_CC_PARAM& rA3DParam)
		{
			fFactor =			rA3DParam.fFactor;
			bUseManualParam1 =	rA3DParam.bUseManualParam1;
			bUseManualParam2 =	rA3DParam.bUseManualParam2;
			bAfterUI =			rA3DParam.bAfterUI;
		}
		EDITOR_DATA_POST_EFFECT_CC_PARAM& operator=(const A3DPOST_EFFECT_CC_PARAM& rA3DParam)
		{
			fFactor =			rA3DParam.fFactor;
			bUseManualParam1 =	rA3DParam.bUseManualParam1;
			bUseManualParam2 =	rA3DParam.bUseManualParam2;
			bAfterUI =			rA3DParam.bAfterUI;
			return *this;
		}
		void CopyParamTo(A3DPOST_EFFECT_CC_PARAM& rA3DParam) const
		{
			rA3DParam.fFactor =				fFactor;
			rA3DParam.bUseManualParam1 =	bUseManualParam1;
			rA3DParam.bUseManualParam2 =	bUseManualParam2;
			rA3DParam.bAfterUI =			bAfterUI;
		}
	};
	EDITOR_DATA_POST_EFFECT_CC_PARAM PARAM_CC_DAY;
	EDITOR_DATA_POST_EFFECT_CC_PARAM PARAM_CC_NIGHT;
	struct EDITOR_DATA_POST_EFFECT_CC_EX1_PARAM
	{
		AString			szTex1;					//第一张映射贴图的文件名		(仅当bUseManualParam1为false的时候才起效果)
		AString			szTex2;					//第二章映射贴图的文件名		(仅当bUseManualParam2为false的时候才起效果)
		EDITOR_DATA_POST_EFFECT_CC_EX1_PARAM()
		{
			szTex1 = "Shaders\\Textures\\ccdefault.dds";
			szTex2 = "Shaders\\Textures\\ccdefault.dds";
		}
		EDITOR_DATA_POST_EFFECT_CC_EX1_PARAM(const A3DPOST_EFFECT_CC_PARAM& rA3DParam)
		{
			szTex1 =			rA3DParam.szTex1;
			szTex2 =			rA3DParam.szTex2;
		}
		EDITOR_DATA_POST_EFFECT_CC_EX1_PARAM& operator=(const A3DPOST_EFFECT_CC_PARAM& rA3DParam)
		{
			szTex1 =			rA3DParam.szTex1;
			szTex2 =			rA3DParam.szTex2;
			return *this;
		}
		void CopyParamTo(A3DPOST_EFFECT_CC_PARAM& rA3DParam) const
		{
			rA3DParam.szTex1 =				szTex1;
			rA3DParam.szTex2 =				szTex2;
		}
	};
	EDITOR_DATA_POST_EFFECT_CC_EX1_PARAM PARAM_CC_EX1_DAY;
	EDITOR_DATA_POST_EFFECT_CC_EX1_PARAM PARAM_CC_EX1_NIGHT;
	struct EDITOR_DATA_POST_EFFECT_CC_MANUAL_PARAM
	{
		int nBright;						//亮度,		范围是[-100~100]
		int nContrast;						//对比度,	范围是[-100~100]
		int nHue;							//色相,		范围是[-180~180]
		int nSaturation;					//饱和度,		范围是[-100~100]
		bool bGrey;							//是否转换为灰度图
		bool bOpposition;					//是否反色
		EDITOR_DATA_POST_EFFECT_CC_MANUAL_PARAM()
		{
			nBright = 0;
			nContrast = 0;
			nHue = 0;
			nSaturation = 0;
			bGrey = false;
			bOpposition = false;
		}
		EDITOR_DATA_POST_EFFECT_CC_MANUAL_PARAM(const A3DPOST_EFFECT_CC_PARAM::ManualParam& rA3DParam)
		{
			nBright =		rA3DParam.nBright;
			nContrast =		rA3DParam.nContrast;
			nHue =			rA3DParam.nHue;
			nSaturation =	rA3DParam.nSaturation;
			bGrey =			rA3DParam.bGrey;
			bOpposition =	rA3DParam.bOpposition;
		}
		EDITOR_DATA_POST_EFFECT_CC_MANUAL_PARAM& operator=(const A3DPOST_EFFECT_CC_PARAM::ManualParam& rA3DParam)
		{
			nBright =		rA3DParam.nBright;
			nContrast =		rA3DParam.nContrast;
			nHue =			rA3DParam.nHue;
			nSaturation =	rA3DParam.nSaturation;
			bGrey =			rA3DParam.bGrey;
			bOpposition =	rA3DParam.bOpposition;
			return *this;
		}
		void CopyParamTo(A3DPOST_EFFECT_CC_PARAM::ManualParam& rA3DParam) const
		{
			rA3DParam.nBright =		nBright;
			rA3DParam.nContrast =	nContrast;
			rA3DParam.nHue =		nHue;
			rA3DParam.nSaturation =	nSaturation;
			rA3DParam.bGrey =		bGrey;
			rA3DParam.bOpposition =	bOpposition;
		}
	};
	EDITOR_DATA_POST_EFFECT_CC_MANUAL_PARAM PARAM_CC_MANUAL1;
	EDITOR_DATA_POST_EFFECT_CC_MANUAL_PARAM PARAM_CC_MANUAL2;
	void CopyToGlobal() const
	{
		PARAM_DOF.CopyParamTo(g_PostEffectParam.dofParam);
		PARAM_BLOOM_DAY.CopyParamTo(g_PostEffectParam.bloomParam);
		PARAM_BLOOM_NIGHT.CopyParamTo(g_PostEffectParamNight.bloomParam);
		PARAM_FULLGLOW_DAY.CopyParamTo(g_PostEffectParam.fullGlowParam);
		PARAM_FULLGLOW_NIGHT.CopyParamTo(g_PostEffectParamNight.fullGlowParam);
		PARAM_UNITED_WARP.CopyParamTo(g_PostEffectParam.warpParam);
		PARAM_UNITED_WARP_EX1.CopyParamTo(g_PostEffectParam.warpParam);
		PARAM_CC_DAY.CopyParamTo(g_PostEffectParam.ccParam);
		PARAM_CC_NIGHT.CopyParamTo(g_PostEffectParamNight.ccParam);
		PARAM_CC_EX1_DAY.CopyParamTo(g_PostEffectParam.ccParam);
		PARAM_CC_EX1_NIGHT.CopyParamTo(g_PostEffectParamNight.ccParam);
		PARAM_CC_MANUAL1.CopyParamTo(g_PostEffectParam.ccParam.manualParam1);
		PARAM_CC_MANUAL2.CopyParamTo(g_PostEffectParam.ccParam.manualParam2);
		if(FLAGS_ENABLE & ENABLE_UNITED_WARP)
			g_dwPostEffectAdditionalFlags |= PostEffectAdditionalFlag_UnitedWarp;
		else
			g_dwPostEffectAdditionalFlags &= ~PostEffectAdditionalFlag_UnitedWarp;
	}
	void CopyFromGlobal()
	{
		PARAM_DOF = g_PostEffectParam.dofParam;
		PARAM_BLOOM_DAY = g_PostEffectParam.bloomParam;
		PARAM_BLOOM_NIGHT = g_PostEffectParamNight.bloomParam;
		PARAM_FULLGLOW_DAY = g_PostEffectParam.fullGlowParam;
		PARAM_FULLGLOW_NIGHT = g_PostEffectParamNight.fullGlowParam;
		PARAM_UNITED_WARP = g_PostEffectParam.warpParam;
		PARAM_UNITED_WARP_EX1 = g_PostEffectParam.warpParam;
		PARAM_CC_DAY = g_PostEffectParam.ccParam;
		PARAM_CC_NIGHT = g_PostEffectParamNight.ccParam;
		PARAM_CC_EX1_DAY = g_PostEffectParam.ccParam;
		PARAM_CC_EX1_NIGHT = g_PostEffectParamNight.ccParam;
		PARAM_CC_MANUAL1 = g_PostEffectParam.ccParam.manualParam1;
		PARAM_CC_MANUAL2 = g_PostEffectParam.ccParam.manualParam2;
		FLAGS_ENABLE = ENABLE_ALL;
		if(!(g_dwPostEffectAdditionalFlags & PostEffectAdditionalFlag_UnitedWarp))
			FLAGS_ENABLE = (EDITOR_DATA_POST_EFFECT_ENABLE_FLAGS)(FLAGS_ENABLE & ~ENABLE_UNITED_WARP);
	}
	EDITOR_DATA_POST_EFFECT_PARAM()
	{
		CopyFromGlobal();
	}
};
#endif

#pragma pack (pop)

///////////////////////////////////////////////////////////////////////////
//	
//	Class CElementMap
//	
///////////////////////////////////////////////////////////////////////////

class CElementMap : public CEditObject
{
public:
	
	class CFloatShader
	{
	public:

		float cur;		//	Current value
		float target;	//	Target value
		float delta;	//	Value change delta

	public:

		void Reset(float v) { cur = target = v; delta = 0.0f; }

		void SetTarget(float t, float fInvTime)
		{
			if (!fInvTime)
				Reset(t);
			else
			{
				target = t;
				delta  = (t - cur) * fInvTime;
			}
		}

		static void Shade(float& f, float f1, float fDelta, float fTime)
		{
			if (!fDelta || f == f1)
				return;

			f += fDelta * fTime;
			if ((fDelta > 0.0f && f > f1) || (fDelta < 0.0f && f < f1))
				f = f1;
		}

		void Shade(float fTime) { Shade(cur, target, delta, fTime); }
	};

	class CColorShader
	{
	public:

		A3DCOLORVALUE cur;		//	Current value
		A3DCOLORVALUE target;	//	Target value
		A3DCOLORVALUE delta;	//	Value change delta

	public:

		void Reset(const A3DCOLORVALUE& v) { cur = target = v; delta.Set(0.0f, 0.0f, 0.0f, 0.0f); }

		void SetTarget(const A3DCOLORVALUE& t, float fInvTime)
		{
			if (!fInvTime)
				Reset(t);
			else
			{
				target = t;
				delta  = (t - cur) * fInvTime;
			}
		}

		void Shade(float fTime)
		{
			CFloatShader::Shade(cur.r, target.r, delta.r, fTime);
			CFloatShader::Shade(cur.g, target.g, delta.g, fTime);
			CFloatShader::Shade(cur.b, target.b, delta.b, fTime);
			CFloatShader::Shade(cur.a, target.a, delta.a, fTime);
		}
	};

public:		//	Types

public:		//	Constructor and Destructor

	CElementMap();
	virtual ~CElementMap();

public:		//	Attributes

public:		//	Operations

	//	Create an empty map
	virtual bool CreateEmpty(const char* szMapName, const CTerrain::INITDATA& TrnInitData);
	//	Release map object
	virtual void Release();

	//	Change map name. Note: this funciton also change map resources directory
	//	and map file name !!
	void ChangeMapName(const char* szMapName);

	//	Load data
	virtual bool Load(CELArchive& ar, bool bOnlyTerrain=false);
	//	Save data
	virtual bool Save(CELArchive& ar);
	//	Load data from file
	bool Load(const char* szFile, bool bOnlyTerrain=false);
	//	Save data to file
	bool Save(const char* szFile);
	//	Save edit record data to file
	bool SaveEditRecord();

	//	Tick routine
	bool FrameMove(DWORD dwDeltaTime);
	//	render routine
	bool Render(A3DViewport* pA3DViewport);
	//render neighborTerrain
	void RenderNeighborTerrain(A3DViewport* pA3DViewport);
	//  render for map's light
	bool RenderForLight(A3DViewport* pA3DViewport);
	//	render for Light map shadow generation routine
	bool RenderForLightMapShadow(A3DViewport * pA3DViewport);
	//	render for reflect pass
	bool RenderObjectsForReflect(A3DViewport * pA3DViewport);
	//	render for refract pass
	bool RenderObjectsForRefract(A3DViewport * pA3DViewport);

	//	Get lights which infect specified position, don't consider sun and environment ambient
	bool GetInfectiveLight(const A3DVECTOR3& vPos, APtrArray<CLight*>& aLights);
	//	Do ray trace in map
	bool RayTrace(const A3DVECTOR3& vStart, const A3DVECTOR3& vVel, float fTime, RAYTRACERT* pTraceRt);

	//	Recreate terrain render. This function is often called when terrain's 
	//	height map size or layers changed
	bool RecreateTerrainRender(bool bHMChanged, bool bLighting);
	//	Update terrain's heights. This function is often called when terrain's 
	//	height map content but not size changed.
	bool UpdateTerrainHeight(const ARectF& rcArea);

	//	Set / Get modified flag
	void SetModifiedFlag(bool bFlag) { m_bModified = bFlag; }
	bool GetModifiedFlag() { return m_bModified; }
	//	Set map offset
	void SetMapOffset(float x, float z);
	//	Get map offset
	void GetMapOffset(float& x, float& z) { x = m_fOffsetX; z = m_fOffsetZ; }

	//	Get resources directory
	const char* GetResDir() { return m_strResDir; }
	//	Get map file name
	const char* GetMapFile() { return m_strMapFile; }
	//	Get map name
	const char* GetMapName() { return m_strMapName; }
	//	Get terrain object
	CTerrain* GetTerrain() { return m_pTerrain; }
	//  Get terrain object of neighbor
	CTerrain* GetNeighborTerrain() { return m_pNeighborTerrain; };
	//	Get terrain water object
	CTerrainWater * GetTerrainWater() { return m_pTerrainWater; }
	//	Get scene object manager
	CSceneObjectManager* GetSceneObjectMan() { return m_pSceneObjectMan; }
	//	Get lighting parameters
	const LIGHTINGPARAMS& GetLightingParams();
	//  Get night lighting parameters;
	const LIGHTINGPARAMS& GetNightLightingParams();
	//	Set lighting parameters for day
	void SetLightingParams(const LIGHTINGPARAMS& Params) { m_Lighting = Params; }
	//  Set lighting parameters for night
	void SetNightLightingParams(const LIGHTINGPARAMS& Params){ m_NightLighting = Params; };
	//  Export water area map	
	bool ExpWaterAreaMap(const char *szFile);
	//  Render to bitmap
	bool ExpTopView(CString expPath ,int nWidth,bool bNight);
	
	// Exports precinct map
	CELBitmap *ExpPrecinctMap();

	//	Load map general data
	virtual bool LoadGeneralMapData(CELArchive& ar);

#ifdef _ANGELICA22
#else
	// Merge trees
	void MergeTreeType();
#endif // #ifdef _ANGELICA22
	// Interface for cloud
	inline CELCloudSceneRender *GetCloudRender(){ return m_pCloudRender; };
	inline CELCloudCreator     *GetCloudCreator(){ return m_pCloudCreator; }; 
	bool RecreateCloud();
	bool RecreateCloudGroup(int nGroup, float size = -1 ,int nSpriteNum = -1);
	bool RecreateAllCloudGroup();
	//Render for neighbor map
	bool LoadNeighborTerrain(int nNeighbor);
	bool LoadTerrainForNeighbor(CELArchive& ar);

	bool LoadNeighborModels(int nNeighbor);
	bool ReleaseNeighborModels();
	bool RenderNeighborModels(A3DViewport* pA3DViewport);

	CPassMapGenerator*  GetPassMapGen(){ return m_pPassMapGenerator; }
	CPlayerPassMapGenerator*  GetPlayerPassMapGen(){ return m_pPlayerPassMapGenerator; }
	CPlayerAirPassMapGenerator*  GetPlayerAirPassMapGen(){ return m_pPlayerAirPassMapGenerator; }
	SPOctree::CSpacePassableOctree* GetSpacePass(){  return m_pSpacePassGenerator; }
	CBrushTree* GetBrushTree(){ return m_pBrushTree; }


	bool GetTerrainHoleState(int r, int c) { return m_aTerrainHoles[r*16+c];}
	void ChangeTerrainHoleState(int r, int c) { m_aTerrainHoles[r*16+c] = !m_aTerrainHoles[r*16+c]; }

	bool SaveSpacePassMap();
	bool SaveLitModel();
	bool SaveAllPassMap();
	//  Load terrain pass map
	bool LoadPassMap();
	//  Load player pass map(character)
	bool LoadPassMap2();
	//  Save terrain pass map
	bool SavePassMap();
	bool SavePassMap2();
	bool LoadPassMap3();
	bool SavePassMap3();
	bool SaveBrushTree();
	bool LoadBrushTree();

	//  Save terrain hole 
	bool SaveTerrainHole();
	bool LoadTerrainHole();
	bool ExportTerrainHole(const char* strFile);

	inline bool IsEnableLitmap()	{return m_bLitmap;}
	inline void EnableLitmap(bool bEnable) {m_bLitmap = bEnable;}
	inline bool IsRealTimeLight()	{return m_bRealTimeLight;}
	inline void EnableRealTimeLight(bool bEnable) { m_bRealTimeLight = bEnable;  }
	inline bool IsEnableDetail()	{return m_bUseDetailMap;}
	inline void EnableDetail(bool bEnable) {m_bUseDetailMap = bEnable;}
#ifdef _ANGELICA22
	A3DSunMoon* GetSunMoon() { return m_pSunMoon; }
#endif

	CHostPlayer* GetHostPlayer() { return m_pHostPlayer; }

protected:	//	Attributes
	int		    m_iLoadFlag;
	CString		m_strMapName;		//	Map name
	CString		m_strMapFile;		//	Map file name relative to g_szWorkDir
	CString		m_strResDir;		//	Resources directory relative to g_szWorkDir
	bool		m_bModified;		//	Map data modified flag
	float		m_fOffsetX;			//	Map offset on x and z axis
	float		m_fOffsetZ;

	CTerrain*						m_pTerrain;			 //	Terrain object
	CTerrain*						m_pNeighborTerrain;  //  Terrain for render
	abase::vector<CELBuilding *>    m_pNeighborModelList;
	CTerrainWater*					m_pTerrainWater;	 //	Terrain water object		
	CSceneObjectManager*			m_pSceneObjectMan;	 //	Scene Object manager
	LIGHTINGPARAMS					m_Lighting;			 //	Lighting parameters for day
	LIGHTINGPARAMS					m_NightLighting;     //  Lighting paramerers for night
	MAPFILEHEADER					m_Header;
	CELCloudSceneRender*			m_pCloudRender;
	CELCloudCreator*				m_pCloudCreator; 
	CPassMapGenerator*				m_pPassMapGenerator;
	CPlayerPassMapGenerator*        m_pPlayerPassMapGenerator;
	SPOctree::CSpacePassableOctree*   m_pSpacePassGenerator;
	CPlayerAirPassMapGenerator*     m_pPlayerAirPassMapGenerator;
	CBrushTree*                     m_pBrushTree;

	bool							m_aTerrainHoles[256];

	CHostPlayer*					m_pHostPlayer;
	bool							m_bOnlyTerrain;
	
#ifdef _ANGELICA22
	A3DSunMoon*                     m_pSunMoon;
#endif
	bool							m_bLitmap;
	bool                            m_bRealTimeLight;
	bool                            m_bUseDetailMap;
protected:	//	Operations

	//	Make map file name
	void MakeMapFileName();
	//	Create map objects
	virtual bool CreateMapObjects();
	//	Release map objects
	virtual void ReleaseMapObjects();
	//	Load terrain data
	virtual bool LoadTerrain(CELArchive& ar);
	//	Save terrain data
	virtual bool SaveTerrain(CELArchive& ar);
	//	Load terrain water data
	virtual bool LoadTerrainWater(CELArchive& ar);
	//	Save terrain water data
	virtual bool SaveTerrainWater(CELArchive& ar);
	//	Load scene object
	virtual bool LoadSceneObjects(CELArchive& ar, bool bOnlyTerrain=false);
	//	Save scene object
	virtual bool SaveSceneObjects(CELArchive& ar);
	//	Load edit record from file
	bool LoadEditRecord();
	//  Case for repair a error
	int GetGrassMaskAreaID(A3DVECTOR3 vCenter);
	//  Save for cloud option
	bool SaveCloudOption(CString pathName, DWORD dwVersion);
	//  Read for cloud option
	bool LoadCloudOption(CString pathName);
	//  Helps function,only seek file position
	bool ReadGeneralMapData(CELArchive& ar);
    void ComputeCurrentLightingParam(LIGHTINGPARAMS& light);
public:
	// transition of scene areas
	//Fog color
	CColorShader		m_FogColor;
	CFloatShader		m_FogStart;
	CFloatShader		m_FogEnd;
	CFloatShader		m_FogDensity;

	CColorShader		m_FogColorNight;
	CFloatShader		m_FogStartNight;
	CFloatShader		m_FogEndNight;
	CFloatShader		m_FogDensityNight;

	CFloatShader		m_FogHeightStart;
	CFloatShader		m_FogHeightEnd;
	CFloatShader		m_FogDensityStart;
	CFloatShader		m_FogDensityEnd;
	CFloatShader		m_FogColorDensityStart;
	CFloatShader		m_FogColorDensityEnd;
	CFloatShader		m_FogHorzeDensity;
	CFloatShader		m_FogHorzePower;
	CColorShader		m_FogColor2;

	CFloatShader		m_FogHeightStartNight;
	CFloatShader		m_FogHeightEndNight;
	CFloatShader		m_FogDensityStartNight;
	CFloatShader		m_FogDensityEndNight;
	CFloatShader		m_FogColorDensityStartNight;
	CFloatShader		m_FogColorDensityEndNight;
	CFloatShader		m_FogHorzeDensityNight;
	CFloatShader		m_FogHorzePowerNight;
	CColorShader		m_FogColor2Night;

	CColorShader		m_FogColorUW;
	CFloatShader		m_FogStartUW;
	CFloatShader		m_FogEndUW;
	CFloatShader		m_FogDensityUW;
	
	CColorShader		m_FogColorUWNight;
	CFloatShader		m_FogStartUWNight;
	CFloatShader		m_FogEndUWNight;
	CFloatShader		m_FogDensityUWNight;

	//Direction light
	CColorShader		m_LitDir;
	CColorShader		m_LitDirNight;
	
	//Ambient light
	CColorShader		m_LitAmb;
	CColorShader		m_LitAmbNight;

	// cloud light
	CColorShader		m_CloudDir;
	CColorShader		m_CloudDirNight;

	CColorShader		m_CloudAmb;
	CColorShader		m_CloudAmbNight;

	CFloatShader        m_BloomThredhold;
	CFloatShader        m_BloomThredholdNight;

	float m_fUoff;
	float m_fVoff;
	bool bShowSun;
	bool bShowMoon;
	
#ifdef _ANGELICA22_FEATURE
	RAYTRACE_CONFIG*                m_pRayTraceConfig;
#endif
};

void MapRenderForReflected(A3DViewport * pViewport, void * pArg);
void MapRenderForRefracted(A3DViewport * pViewport, void * pArg);
	
// Get 8 neighbor information /xqf
bool GetProjectListInfor(const CString strCurrentPrj, int pNeiIndices[], CString pNeiName[]);
///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

