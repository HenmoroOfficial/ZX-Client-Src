/*
 * FILE: A3DPostBloom.h
 *
 * DESCRIPTION: Bloom后处理
 *
 * CREATED BY: Panyupin, 2012/2/2
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 Archosaur Studio, All Rights Reserved.
 */
#ifndef _A3D_POST_BLOOM_H_
#define _A3D_POST_BLOOM_H_

#include "A3DPost.h"

class A3DPostSystem;

class A3DPostBloom : public A3DPost
{
public:
	virtual ~A3DPostBloom();
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
	bool Init(A3DPostSystem* pSystem, bool bLocalBloom = false);

	void	SetBlurSize(float fBlurSize);
	float	GetBlurSize(){ return m_fBlurSize; }
	void	SetBrightThreshold(float fThreshold);
	float	GetBrightThreshold(){ return m_fBrightThreshold; }
	void	SetBrightScale(float fScale);
	float	GetBrightScale(){ return m_fBrightScale; }

	A3DPostBloom()
	{
		m_pThresholdPS = NULL;
		//m_pMaskPS = NULL;
		m_pBlurXPS = NULL;
		m_pBlurYPS = NULL;
		m_pMergePS = NULL;
		m_pDownSampPS = NULL;
		m_pAddBlurPS = NULL;

		m_fBlurSize = 1.f;
		m_iQuality = 1;
		m_fBrightThreshold = 0.7f;
		m_fBrightScale = 0.8f;
	}
private:
	A3DHLSL*			m_pThresholdPS;
	//A3DHLSL*			m_pMaskPS;
	A3DHLSL*			m_pBlurXPS;
	A3DHLSL*			m_pBlurYPS;
	A3DHLSL*			m_pMergePS;
	A3DHLSL*			m_pDownSampPS;
	A3DHLSL*			m_pAddBlurPS;

	float m_fBlurSize;				//模糊距离0~1
	int m_iQuality;					//质量 0为粗糙, 1为高 //现在只有一种选择
	float m_fBrightThreshold;		//阈值 0~1
	float m_fBrightScale;			//缩放 0~1
};


#endif