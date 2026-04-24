/*
 * FILE: A3DPostGodRay.h
 *
 * DESCRIPTION: 天光后处理
 *
 * CREATED BY: Panyupin, 2012/2/2
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 Archosaur Studio, All Rights Reserved.
 */

#ifndef _A3D_POST_GODRAY_H_
#define _A3D_POST_GODRAY_H_

#include "A3DPost.h"
#include "A3DVertex.h"
#include "A3DMacros.h"

class A3DPostSystem;

class A3DPostGodRay : public A3DPost
{
public:
	virtual ~A3DPostGodRay();
	// 释放
	virtual void Release();
	// 渲染结果
	virtual bool Render(A3DViewport* pViewport);
	// 特效类型
	virtual A3DPOST_EFFECT GetEffectType() const;
	// 硬件
	static bool IsHardwareSupport(A3DDevice* pA3DDev);
    // 渲染结果
    virtual A3DRenderTarget* ApplyEffect(A3DViewport* pViewport, A3DRenderTarget* pSrcRT);

public:
	// 初始化
	bool Init(A3DPostSystem* pSystem);

	//每帧都得调用
	void SetLightDir(const A3DVECTOR3& vLightDir){ m_vLightDir = vLightDir; }

	void		SetVisibleAngle(float fAngle);
	float		GetVisibleAngle(){ return m_fVisibleAngle; }
	void		SetOcclusionDepthRange(float fRange);
	float		GetOcclusionDepthRange();
	void		SetBloomScale(float fBloomScale);
	float		GetBloomScale();
	void		SetBloomThreshold(float fBloomThreshold);
	float		GetBloomThreshold();
	void		SetScreenBlendThreshold(float fScreenBlendThreshold);
	float		GetScreenBlendThreshold();
	void		SetBloomTintColor(const A3DVECTOR3& vColor);
	A3DVECTOR3	GetBloomTintColor();
	void		SetOcclusionDarkness(float fOcclusionDarkness);
	float		GetOcclusionDarkness();

	A3DPostGodRay()
	{
		m_pSunPS = NULL;
		m_pBlurPS = NULL;
		m_pMergePS = NULL;

		m_fVisibleAngle = A3D_PI / 3.f;

		m_fOcclusionDepthRange = 1000.f;
		m_fBloomScale = 1.f;
		m_fBloomThreshold = 0.f;
		m_fScreenBlendThreshold = 1.0;
		m_vBloomTint = A3DVECTOR3(1,1,1);
		m_fOcclusionDarkness = 0.3f;
	}
private:
	bool CreateSunRT(int nQuality);
	bool LoadShader();

	A3DIBLVERTEX		m_aSunQuadVertex[4];
	A3DHLSL*			m_pSunPS;
	A3DHLSL*			m_pBlurPS;
	A3DHLSL*			m_pMergePS;
	A3DHLSL*			m_pDownSampPS;
	float				m_fVisibleAngle;//可见角度,0 ~ 2*A3D_PI之间

	float				m_fOcclusionDepthRange;		//大于此深度的区域颜色会被保留进行Bloom,小于此深度的区域颜色会变为黑
	float				m_fBloomScale;				//Bloom叠加强度. 0为不叠加Bloom, 无光束
	float				m_fBloomThreshold;			//亮度大于此部分才会被保留进行Bloom
	float				m_fScreenBlendThreshold;	//屏幕亮度必须小于这个值才会接受Bloom的叠加, 越小叠加越多
	A3DVECTOR3			m_vBloomTint;				//场景颜色会乘以这个值之后再进行bloom
	float				m_fOcclusionDarkness;		//根据模糊后的OcclusionMask对屏幕颜色进行黑化, 0则最大程度黑化,1则不黑化


	A3DVECTOR4			m_vBlurCenter;				//模糊中心. 在RenderSun的时候算好, 在blur的时候用到
	float				m_fAngleScale;

	A3DVECTOR3			m_vLightDir;
};

#endif