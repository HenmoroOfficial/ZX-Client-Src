/*
 * FILE: A3DPostSilhouette.h
 *
 * DESCRIPTION: 勾边后处理
 *
 * CREATED BY: Panyupin, 2013/1/30
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 Archosaur Studio, All Rights Reserved.
 */

#ifndef _A3D_POST_SILHOUETTE_H_
#define _A3D_POST_SILHOUETTE_H_

#include "A3DPost.h"

class A3DPostSystem;

class A3DPostSilhouette : public A3DPost
{
public:
	virtual ~A3DPostSilhouette();
	// 释放
	virtual void Release();
	// 渲染结果
	virtual bool Render(A3DViewport* pViewport);
	// 特效类型
	virtual A3DPOST_EFFECT GetEffectType() const;
	// 硬件
	static bool IsHardwareSupport(A3DDevice* pA3DDev);

	float GetHardness() const;
	void SetHardness(float fHardness);

	float GetBlurDist() const;
	void SetBlurDist(float fBlurDist);

	const A3DVECTOR3& GetLineColor() const;
	void SetLineColor(const A3DVECTOR3& vLineColor);

    bool RenderForRT(A3DViewport* pViewport, A3DRenderTarget* pSrcRT, A3DRenderTarget* pDstRT, const A3DVECTOR4& vMask);
    // 渲染结果
    virtual A3DRenderTarget* ApplyEffect(A3DViewport* pViewport, A3DRenderTarget* pSrcRT);

public:
	// 初始化
	bool Init(A3DPostSystem* pSystem);
	
	A3DRenderTarget* GetPlayerSilhousetteRT() const { return m_pPlayerDepthRT; }
	A3DPostSilhouette()
	{
		m_pHLSL_Edge = NULL;
		m_pHLSL_Blur = NULL;
		m_pPlayerDepthRT = NULL;
        m_pBlackTexture = NULL;
		m_vLineColor = A3DVECTOR3(0,0,0);
	}
private:
	A3DHLSL*					m_pHLSL_Edge;
	A3DHLSL*					m_pHLSL_Blur;
	A3DRenderTarget*			m_pPlayerDepthRT;
    A3DTexture*                 m_pBlackTexture;
	float m_fHardness;
	float m_fBlurDist;
	A3DVECTOR3 m_vLineColor;
	void ResetParam();
    bool DoRender(A3DViewport* pViewport, bool bUseSceneDepth, A3DRenderTarget* pSrcTexture, A3DRenderTarget* pDstTexture, const A3DVECTOR4& vMask );
};


#endif